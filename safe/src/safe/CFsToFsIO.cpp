/*
  Safe: Encrypted File System
  Copyright (C) 2013 Rian Hunter <rian@alum.mit.edu>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "CFsToFsIO.hpp"

#include <safe/util.hpp>

#include <encfs/fs/FsIO.h>
#include <encfs/base/Interface.h>
#include <encfs/base/optional.h>

#include <davfuse/logging.h>
#include <davfuse/fs.h>
#include <davfuse/util_fs.h>

#include <limits>

#include <cstring>

namespace std {
template <>
struct is_error_code_enum<fs_error_t> : public true_type {};
}

namespace safe {

class fs_error_category_cls : public std::error_category {
public:
  fs_error_category_cls() {}

  std::error_condition
  default_error_condition(int __i) const noexcept
  {
    // TODO: implement bool equivalent(int code, const std::error_condition &)
    //       if we are equal to other conditions
    switch ((fs_error_t) __i) {
    case FS_ERROR_SUCCESS: assert(false);
    case FS_ERROR_DOES_NOT_EXIST: return std::errc::no_such_file_or_directory;
    case FS_ERROR_NOT_DIR: return std::errc::not_a_directory;
    case FS_ERROR_IS_DIR: return std::errc::is_a_directory;
    case FS_ERROR_IO: return std::errc::io_error;
    case FS_ERROR_NO_SPACE: return std::errc::no_space_on_device;
    case FS_ERROR_PERM: return std::errc::operation_not_permitted;
    case FS_ERROR_EXISTS: return std::errc::file_exists;
    case FS_ERROR_ACCESS: return std::errc::permission_denied;
#ifndef _WIN32
      // deal with bad win32 libstdc++ headers
    case FS_ERROR_CROSS_DEVICE: return std::errc::cross_device_link;
#else
    case FS_ERROR_CROSS_DEVICE: return std::errc::io_error;
#endif
    case FS_ERROR_INVALID_ARG: return std::errc::invalid_argument;
    case FS_ERROR_NO_MEM: return std::errc::not_enough_memory;
    default: assert(false);
    }
    /* should never happen */
    assert(false);
    return std::errc::io_error;
  }

  virtual const char *name() const noexcept {
    return "fs_error";
  }

  virtual std::string message(int cond) const {
#define SC(f) case f: return #f
    switch ((fs_error_t) cond) {
    case FS_ERROR_SUCCESS: assert(false);
      SC(FS_ERROR_DOES_NOT_EXIST);
      SC(FS_ERROR_NOT_DIR);
      SC(FS_ERROR_IS_DIR);
      SC(FS_ERROR_IO);
      SC(FS_ERROR_NO_SPACE);
      SC(FS_ERROR_PERM);
      SC(FS_ERROR_EXISTS);
      SC(FS_ERROR_ACCESS);
      SC(FS_ERROR_CROSS_DEVICE);
      SC(FS_ERROR_INVALID_ARG);
      SC(FS_ERROR_NO_MEM);
    default:
      assert(false);
    }
    /* should never happen */
    return "";
#undef SC
  }
};

const std::error_category &fs_error_category() noexcept {
  static const fs_error_category_cls fs_error_category_instance;
  return fs_error_category_instance;
}

std::error_code make_error_code(fs_error_t e) {
  assert(e);
  return std::error_code(static_cast<int>(e), fs_error_category());
}

std::error_condition make_error_condition(fs_error_t e) {
  assert(e);
  return std::error_condition(static_cast<int>(e), fs_error_category());
}

static std::system_error fs_error(fs_error_t err) {
  assert(err);
  return std::system_error(make_error_code(err));
}

static encfs::FsFileAttrs c_fs_attrs_to_encfs_file_attrs(const FsAttrs & attrs) {
  return (encfs::FsFileAttrs) {
    .type = (attrs.is_directory ?
             encfs::FsFileType::DIRECTORY :
             encfs::FsFileType::REGULAR),
    .mtime = attrs.modified_time,
    .size = attrs.size,
    .file_id = attrs.file_id,
    .volume_id = attrs.volume_id,
    .posix = opt::nullopt
  };
}

class CFsToFsIOPath final : public encfs::PathPoly {
protected:
  fs_handle_t _fs;
  std::string _str;

public:
  CFsToFsIOPath(fs_handle_t fs, std::string str)
    : _fs(fs)
    , _str(std::move(str)) {
    if (!fs_path_is_valid(fs, this->c_str())) {
      throw std::runtime_error("invalid path: " + (const std::string &) *this);
    }
  }

  virtual operator const std::string&() const {
    return _str;
  }

  virtual std::unique_ptr<PathPoly> join(std::string name) const override {
    auto new_cstr = fs_path_join(_fs, this->c_str(), name.c_str());
    if (!new_cstr) throw std::runtime_error("couldn't join");
    auto _defer_free = safe::create_deferred(free, new_cstr);
    return safe::make_unique<CFsToFsIOPath>(_fs, new_cstr);
  }

  virtual std::string basename() const override {
    auto new_cstr = fs_path_basename(_fs, this->c_str());
    if (!new_cstr) throw std::runtime_error("couldn't create basename");
    auto _defer_free = safe::create_deferred(free, new_cstr);
    return std::string(new_cstr);
  }

  virtual std::unique_ptr<encfs::PathPoly> dirname() const override {
    auto dirpath = fs_path_dirname(_fs, this->c_str());
    if (!dirpath) throw std::runtime_error("couldn't create dirname!");
    auto _free_dispath = safe::create_deferred(free, dirpath);
    return safe::make_unique<CFsToFsIOPath>(_fs, dirpath);
  }

  virtual bool is_root() const override {
    return fs_path_is_root(_fs, this->c_str());
  }

  bool operator==(const PathPoly &p) const override {
    auto p2 = dynamic_cast<const CFsToFsIOPath *>(&p);
    if (!p2) return false;
    return (const std::string &)*this == (const std::string &)*p2;
  }

  virtual std::unique_ptr<encfs::PathPoly> copy() const override {
    return safe::make_unique<CFsToFsIOPath>(_fs, _str);
  }
};

class CFsToFsIODirectoryIO final : public encfs::DirectoryIO {
private:
  fs_handle_t _fs;
  fs_directory_handle_t _dh;

  CFsToFsIODirectoryIO(fs_handle_t fs, fs_directory_handle_t dh) noexcept
    : _fs(fs), _dh(dh) {}

public:
  virtual ~CFsToFsIODirectoryIO() override;
  virtual opt::optional<encfs::FsDirEnt> readdir() override;

  friend class CFsToFsIO;
};

class CFsToFsIOFileIO : public encfs::FileIO {
  fs_handle_t _fs;
  fs_file_handle_t _fh;
  bool _open_for_write;

public:
  CFsToFsIOFileIO(fs_handle_t fs, fs_file_handle_t fh, bool open_for_write)
    : _fs(fs)
    , _fh(fh)
    , _open_for_write(open_for_write) {}

  // can't copy CFsToFsIOFileIO unless we dup fd
  CFsToFsIOFileIO(const CFsToFsIOFileIO &) = delete;
  CFsToFsIOFileIO &operator=(const CFsToFsIOFileIO &) = delete;

  virtual ~CFsToFsIOFileIO() override;

  virtual encfs::Interface interface() const override;

  virtual encfs::FsFileAttrs get_attrs() const override;

  virtual size_t read(const encfs::IORequest & req) const override;
  virtual void write(const encfs::IORequest &req) override;

  virtual void truncate( encfs::fs_off_t size ) override;

  virtual bool isWritable() const override;

  virtual void sync(bool datasync) override;
};

CFsToFsIODirectoryIO::~CFsToFsIODirectoryIO() {
  auto err = fs_closedir(_fs, _dh);
  if (err) log_error("couldn't close directory handle, leaking...");
}

opt::optional<encfs::FsDirEnt> CFsToFsIODirectoryIO::readdir() {
  char *cname;
  bool attrs_is_filled;
  FsAttrs attrs;
  auto err = fs_readdir(_fs, _dh, &cname, &attrs_is_filled, &attrs);
  if (err) throw fs_error( err );

  if (!cname) return opt::nullopt;

  auto name = std::string(cname);
  free(cname);

  // TODO: fix this, make fs_readdir interface congruent
  //       to DirectoryIO::readdir interface
  //       (i.e. make file_id required in both or neither)
  if (!attrs_is_filled) throw std::runtime_error("bad readdir");

  return encfs::FsDirEnt(std::string(name), attrs.file_id,
                         attrs.is_directory
                         ? encfs::FsFileType::DIRECTORY
                         : encfs::FsFileType::REGULAR);
}

CFsToFsIOFileIO::~CFsToFsIOFileIO() {
  auto err = fs_close(_fs, _fh);
  if (err) log_error("couldn't close file handle, leaking...");
}

static encfs::Interface CFsToFsIOFileIO_iface = encfs::makeInterface("FileIO/CFsToFsIO", 1, 0, 0);

encfs::Interface CFsToFsIOFileIO::interface() const {
  return CFsToFsIOFileIO_iface;
}

encfs::FsFileAttrs CFsToFsIOFileIO::get_attrs() const {
  FsAttrs attrs;
  auto err = fs_fgetattr(_fs, _fh, &attrs);
  if (err) throw fs_error(err);
  return c_fs_attrs_to_encfs_file_attrs(attrs);
}

size_t CFsToFsIOFileIO::read(const encfs::IORequest & req) const {
  size_t amt_read;
  auto err = fs_read(_fs, _fh, (char *) req.data,
                     req.dataLen, req.offset, &amt_read);
  if (err) throw fs_error(err);
  return amt_read;
}

void CFsToFsIOFileIO::write(const encfs::IORequest &req) {
  if (!_open_for_write) throw fs_error(FS_ERROR_PERM);

  size_t written = 0;
  while (written != req.dataLen) {
    size_t amt;
    auto err = fs_write(_fs, _fh, (char *) req.data + written,
                        req.dataLen - written, req.offset + written,
                        &amt);
    // NB: this is bad to fail a write() in the middle
    // we should change the interface of FileIO::write to
    // allow partial writes
    if (err) throw fs_error(err);

    written += amt;
  }
}

void CFsToFsIOFileIO::truncate(encfs::fs_off_t size) {
  if (!_open_for_write) throw fs_error(FS_ERROR_PERM);
  auto err = fs_ftruncate(_fs, _fh, size);
  if (err) throw fs_error(err);
}

bool CFsToFsIOFileIO::isWritable() const  {
  return _open_for_write;
}

void CFsToFsIOFileIO::sync(bool /*datasync*/) {
  // C Fs does not support this interface
}

CFsToFsIO::CFsToFsIO(fs_handle_t fs, bool destroy_on_delete)
  : _fs(fs)
  , _destroy_on_delete(destroy_on_delete) {
}

CFsToFsIO::~CFsToFsIO() {
  if (_destroy_on_delete) {
    const auto success = fs_destroy(_fs);
    if (!success) log_error("failure destroying fs");
  }
}

encfs::Path CFsToFsIO::pathFromString(const std::string &path) const {
  // in the fs C interface, paths are always UTF-8 encoded strings
  // CFsToFsIOPath does validity checking in constructor
  return std::unique_ptr<CFsToFsIOPath>(new CFsToFsIOPath(_fs, path));
}

encfs::Directory CFsToFsIO::opendir(const encfs::Path &path) const {
  fs_directory_handle_t dh;
  auto err = fs_opendir(_fs, path.c_str(), &dh);
  if (err) throw fs_error(err);
  return std::unique_ptr<CFsToFsIODirectoryIO>(new CFsToFsIODirectoryIO(_fs, dh));
}

encfs::File CFsToFsIO::openfile(const encfs::Path &path,
                                bool open_for_write,
                                bool create) {
  fs_file_handle_t fh;
  auto err = fs_open(_fs, path.c_str(), create, &fh, NULL);
  if (err) throw fs_error(err);
  return std::unique_ptr<CFsToFsIOFileIO>(new CFsToFsIOFileIO(_fs, fh, open_for_write));
}

void CFsToFsIO::mkdir(const encfs::Path &path) {
  auto err = fs_mkdir( _fs, path.c_str() );
  if (err) throw fs_error(err);
}

void CFsToFsIO::rename(const encfs::Path &pathSrc, const encfs::Path &pathDst) {
  auto err = fs_rename( _fs, pathSrc.c_str(), pathDst.c_str() );
  if (err) throw fs_error(err);
}

void CFsToFsIO::unlink(const encfs::Path &path) {
  auto err = fs_remove( _fs, path.c_str() );
  if (err) throw fs_error(err);
}

void CFsToFsIO::rmdir(const encfs::Path &path) {
  auto err = fs_remove( _fs, path.c_str() );
  if (err) throw fs_error(err);
}

encfs::FsFileAttrs CFsToFsIO::get_attrs(const encfs::Path &path) const {
  FsAttrs attrs;
  auto err = fs_getattr(_fs, path.c_str(), &attrs);
  if (err) throw fs_error(err);
  return c_fs_attrs_to_encfs_file_attrs(attrs);
}

void CFsToFsIO::set_times(const encfs::Path &path,
                          const opt::optional<encfs::fs_time_t> &atime,
                          const opt::optional<encfs::fs_time_t> &mtime) {
  if(atime &&
     (*atime > std::numeric_limits<::fs_time_t>::max() ||
      *atime < std::numeric_limits<::fs_time_t>::lowest())) {
    throw fs_error(FS_ERROR_INVALID_ARG);
  }

  if(mtime &&
     (*mtime > std::numeric_limits<::fs_time_t>::max() ||
      *mtime < std::numeric_limits<::fs_time_t>::lowest())) {
    throw fs_error(FS_ERROR_INVALID_ARG);
  }

  ::fs_time_t atime_in = atime ? (::fs_time_t) *atime : FS_INVALID_TIME;
  ::fs_time_t mtime_in = mtime ? (::fs_time_t) *mtime : FS_INVALID_TIME;

  auto err = fs_set_times(_fs, path.c_str(), atime_in, mtime_in);
  if (err) throw fs_error(err);
}

}
