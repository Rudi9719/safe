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

#ifndef _safe_fs_hpp
#define _safe_fs_hpp

#include <encfs/fs/FsIO.h>
#include <encfs/fs/FileUtils.h>
#include <encfs/cipher/MemoryPool.h>

#include <functional>
#include <memory>
#include <string>

#ifndef _CXX_STATIC_BUILD
#define CXX_STATIC_ATTR
#endif

namespace safe {

CXX_STATIC_ATTR
std::shared_ptr<encfs::FsIO>
create_native_fs();

CXX_STATIC_ATTR
std::shared_ptr<encfs::FsIO>
create_enc_fs(std::shared_ptr<encfs::FsIO> base_fs_io,
              encfs::Path encrypted_folder_path,
              const encfs::EncfsConfig & cfg,
              encfs::SecureMem password);

}

#ifdef _CXX_STATIC_BUILD
#include <safe/fs.cpp>
#else
#undef CXX_STATIC_ATTR
#endif

#endif
