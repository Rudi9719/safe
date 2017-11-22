# Safe: Encrypted File System
# Copyright (C) 2013 Rian Hunter <rian@alum.mit.edu>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

EXE_NAME := Safe.exe
DEBUG_EXE_NAME := $(basename $(EXE_NAME))-Debug.exe

OUT_DIR ?= out

DAVFUSE_ROOT := $(CURDIR)/../davfuse
ENCFS_ROOT := $(CURDIR)/../encfs
BOTAN_ROOT := $(CURDIR)/dependencies/botan
TINYXML_ROOT := $(CURDIR)/dependencies/tinyxml
PROTOBUF_ROOT := $(CURDIR)/../protobuf
SAFE_RAMDISK_ROOT := $(CURDIR)/dependencies/safe_ramdisk

RESOURCES_ROOT = $(CURDIR)/resources
DYN_RESOURCES_ROOT = $(CURDIR)/$(OUT_DIR)/resources
HEADERS_ROOT = $(CURDIR)/$(OUT_DIR)/headers
DEPS_INSTALL_ROOT = $(CURDIR)/$(OUT_DIR)/deps

ifdef IS_WIN_CROSS
 WINDRES = $(IS_WIN_CROSS)-windres
 RANLIB = $(IS_WIN_CROSS)-ranlib
 AR = $(IS_WIN_CROSS)-ar
 CC = $(IS_WIN_CROSS)-gcc
 CXX = $(IS_WIN_CROSS)-g++
 DLLTOOL = $(IS_WIN_CROSS)-dlltool
 STRIP = $(IS_WIN_CROSS)-strip
 OBJDUMP = $(IS_WIN_CROSS)-objdump
 BINUTILS_PREFIX = $(IS_WIN_CROSS)-
else
 RANLIB ?= ranlib
 WINDRES ?= windres
 DLLTOOL ?= dlltool
 STRIP ?= strip
 OBJDUMP ?= objdump
endif

IS_WIN := $(shell uname | grep -i mingw)

IS_WIN_TARGET = $(or $(IS_WIN_CROSS),$(IS_WIN))
IS_WIN64_TARGET := $(if $(IS_WIN_TARGET),$(shell echo 'int main() {return 0;}' | ( $(CC) $(CFLAGS) -x c - -o test.exe && ( $(OBJDUMP) -x test.exe; rm test.exe) ) | head -n 2 | grep x86-64),)
IS_MAC_TARGET = $(if $(IS_WIN_TARGET),,$(shell test `uname` = Darwin && echo 1))

PROCS := $(if $(IS_WIN),1,$(if $(shell `which nproc 2>/dev/null`),$(shell nproc),$(if $(shell which sysctl),$(shell sysctl hw.ncpu | awk '{print $$2}'),1)))

WEBDAV_SERVER_STATIC_LIBRARY = $(DEPS_INSTALL_ROOT)/lib/libwebdav_server_fs.a
ENCFS_STATIC_LIBRARY = $(DEPS_INSTALL_ROOT)/lib/libencfs.a

# we build for windows xp and above
# we require comctl32.dll >= v6.0 (available in XP)
# we require normaliz.dll (available in XP SP2)
# NB: A Windows 2000 port wouldn't be too hard to do
GLOBAL_WINDOWS_DEFINES = -D_UNICODE -DUNICODE -D_WIN32_IE=0x0600 -D_WIN32_WINNT=0x501 -DWINVER=0x501 -DNTDDI_VERSION=0x05010000

CPPFLAGS_RELEASE = -DNDEBUG
CXXFLAGS_RELEASE = -g -Os $(if $(IS_WIN_TARGET),-flto,,)
CFLAGS_RELEASE = -g -Os $(if $(IS_WIN_TARGET),-flto,,)
LDFLAGS_RELEASE = -fuse-linker-plugin
CXXFLAGS_DEBUG = -g
CFLAGS_DEBUG = -g

ASLR_LINK_FLAGS := -Wl,--dynamicbase=true -Wl,--nxcompat=true
WINDOWS_SUBSYS_LINK_FLAGS := -mwindows

# default global configuration flags
CPPFLAGS ?= $(if $(RELEASE),$(CPPFLAGS_RELEASE),$(CPPFLAGS_DEBUG))
CXXFLAGS ?= $(if $(RELEASE),$(CXXFLAGS_RELEASE),$(CXXFLAGS_DEBUG))
CFLAGS ?= $(if $(RELEASE),$(CFLAGS_RELEASE),$(CFLAGS_DEBUG))

# these are flags specific to our source files (everything in safe-app/src, not our deps)
MY_CPPFLAGS = $(CPPFLAGS) -I$(CURDIR)/src -I$(HEADERS_ROOT) \
 -I$(DEPS_INSTALL_ROOT)/include -I$(DEPS_INSTALL_ROOT)/include/encfs \
 -I$(DEPS_INSTALL_ROOT)/include/encfs/base -I$(DAVFUSE_ROOT)/src \
 $(if $(IS_WIN_TARGET),$(GLOBAL_WINDOWS_DEFINES),)
MY_CXXFLAGS = $(CXXFLAGS) -Wall -Wextra -Werror -std=c++11 \
 $(if $(IS_WIN_TARGET),`$(DEPS_INSTALL_ROOT)/bin/botan-config-1.10 --cflags`,)

# encfs on mac makes use of the Security framework
MAC_EXTRA_LIBRARIES := -framework Security -framework Foundation
WIN_EXTRA_LIBRARIES := -lws2_32
DEPS_LIBRARIES = -lwebdav_server_fs -lencfs \
 $(if $(IS_WIN_TARGET),`$(DEPS_INSTALL_ROOT)/bin/botan-config-1.10 --libs`,) \
 -lprotobuf \
 -ltinyxml
DEPS_EXTRA_LIBRARIES := $(if $(IS_MAC_TARGET),$(MAC_EXTRA_LIBRARIES),) $(if $(IS_WIN_TARGET),$(WIN_EXTRA_LIBRARIES),)

all: $(if $(IS_WIN_TARGET),$(EXE_NAME),) test_encfs_main

libwebdav_server_fs: clean
	@rm -rf "$(DAVFUSE_ROOT)/out"
	@cd $(DAVFUSE_ROOT); rm config.mk; cp $(if $(IS_WIN_TARGET),config-nt-msvcrt-mingw.mk,config-xnu-bsdlibc-clang.mk) config.mk
	@cd $(DAVFUSE_ROOT); AR="$(AR)" CC="$(CC)" CXX="$(CXX)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CFLAGS="$(CFLAGS)" make -j$(PROCS) RELEASE= USE_DYNAMIC_FS=1 libwebdav_server_fs.a
	@mkdir -p $(DEPS_INSTALL_ROOT)/lib
	@cp "$(DAVFUSE_ROOT)/out/targets/$(notdir $(WEBDAV_SERVER_STATIC_LIBRARY))" $(WEBDAV_SERVER_STATIC_LIBRARY)
	@mkdir -p $(DEPS_INSTALL_ROOT)/include/davfuse
	@find $(DAVFUSE_ROOT)/src -name '*.h' | (while read F; do cp $$F $(DEPS_INSTALL_ROOT)/include/davfuse/$$(basename $$F); done)
	@find $(DAVFUSE_ROOT)/out/libwebdav_server_fs/headers -name '*.h' | (while read F; do cp $$F $(DEPS_INSTALL_ROOT)/include/davfuse/$$(basename $$F); done)

libencfs: clean
#	TODO: don't require fuse when configuring encfs
	@cd $(ENCFS_ROOT); rm -rf CMakeCache.txt CMakeFiles
	@cd $(ENCFS_ROOT); \
         cmake . -DCMAKE_BUILD_TYPE=Debug \
               $(if $(AR),-DCMAKE_AR=`which $(AR)`,) \
               $(if $(RANLIB),-DCMAKE_RANLIB=`which $(RANLIB)`,) \
               $(if $(CXX),-DCMAKE_CXX_COMPILER=$(CXX),) \
               $(if $(CC),-DCMAKE_C_COMPILER=$(CC),) \
               $(if $(IS_WIN),-G"MSYS Makefiles",) \
               -DCMAKE_PREFIX_PATH=$(DEPS_INSTALL_ROOT) \
               "-DCMAKE_CXX_FLAGS=$(CPPFLAGS) $(CXXFLAGS)" \
               "-DCMAKE_C_FLAGS=$(CPPFLAGS) $(CFLAGS)" \
               "-DCMAKE_BUILD_TYPE=None" \
               $(if $(IS_WIN_TARGET),-DCMAKE_INCLUDE_PATH=$(DEPS_INSTALL_ROOT)/include/botan-1.10,)
	@cd $(ENCFS_ROOT); make clean
	@cd $(ENCFS_ROOT); PATH="$(DEPS_INSTALL_ROOT)/bin:$$PATH" make -j$(PROCS) encfs-base encfs-cipher encfs-fs
#	copy all encfs headers into our build headers dir
	@find $(ENCFS_ROOT) -name '*.h' | (while read F; do ROOT=$(ENCFS_ROOT); NEWF=$$(echo $$F | sed "s|^$$ROOT|$(OUT_DIR)/deps/include/encfs|"); mkdir -p $$(dirname "$$NEWF"); cp $$F $$NEWF; done)
#       create archive
# TODO: it would probably be better if the encfs build system did this
	@mkdir -p $(DEPS_INSTALL_ROOT)/lib
	@TMPDIR=`mktemp -d /tmp/temp.XXXX`; \
         cd $$TMPDIR; \
         $(AR) -x $(ENCFS_ROOT)/cipher/libencfs-cipher.a; \
         $(AR) -x $(ENCFS_ROOT)/fs/libencfs-fs.a; \
         $(AR) -x $(ENCFS_ROOT)/base/libencfs-base.a; rm -f $(ENCFS_STATIC_LIBRARY); $(AR) rcs $(ENCFS_STATIC_LIBRARY) *.*;

libbotan: clean
	@mkdir -p /tmp/botan_path;
	@echo '#!/bin/sh' > /tmp/botan_path/ar; \
         echo "export PATH=\"$$PATH\"" >> /tmp/botan_path/ar; \
         echo 'exec $(AR) $$@' >> /tmp/botan_path/ar; \
         chmod a+x /tmp/botan_path/ar;
	@echo '#!/bin/sh' > /tmp/botan_path/ranlib; \
         echo "export PATH=\"$$PATH\"" >> /tmp/botan_path/ranlib; \
         echo 'exec $(RANLIB) $$@' >> /tmp/botan_path/ranlib; \
         chmod a+x /tmp/botan_path/ranlib;
	@echo '#!/bin/sh' > /tmp/botan_path/c++; \
         echo "export PATH=\"$$PATH\"" >> /tmp/botan_path/c++; \
         echo 'exec $(CXX) $(CPPFLAGS) $(CXXFLAGS) $$@' >> /tmp/botan_path/c++; \
         chmod a+x /tmp/botan_path/c++;
# the nr module has an issue with LTO
	@cd $(BOTAN_ROOT); PATH="/tmp/botan_path:$$PATH" ./configure.py \
	--disable-modules=nr \
         --no-optimizations --prefix=$(DEPS_INSTALL_ROOT) \
         --disable-shared \
         $(if $(shell test $(CXX) == clang++ && echo 1),--cc=clang,--cc-bin=c++) \
         $(if $(IS_WIN_TARGET),--os=mingw $(if $(IS_WIN64_TARGET),,--cpu=x86),) --with-tr1=none
	@cd $(BOTAN_ROOT); PATH="/tmp/botan_path:$$PATH" make clean
	@cd $(BOTAN_ROOT); PATH="/tmp/botan_path:$$PATH" make -j$(PROCS)
	@cd $(BOTAN_ROOT); PATH="/tmp/botan_path:$$PATH" make install

libprotobuf: clean
	@cd $(PROTOBUF_ROOT); ./autogen.sh
# first build protobuf protoc
	@cd $(PROTOBUF_ROOT); unset CC CXX CPPFLAGS CFLAGS CXXFLAGS AR RANLIB; ./configure --prefix=$(DEPS_INSTALL_ROOT) $(if $(IS_WIN_CROSS),--target $(IS_WIN_CROSS),) --disable-shared
	@cd $(PROTOBUF_ROOT); unset CC CXX CPPFLAGS CFLAGS CXXFLAGS AR RANLIB; make clean
	@cd $(PROTOBUF_ROOT); cd src; unset CC CXX CPPFLAGS CXXFLAGS AR RANLIB; make -j$(PROCS) $(if $(IS_WIN),protoc.exe,protoc)
	@mkdir -p $(DEPS_INSTALL_ROOT)/bin
	@cp $(PROTOBUF_ROOT)/src/$(if $(IS_WIN),protoc.exe,protoc) $(DEPS_INSTALL_ROOT)/bin
# now build libprotobuf.a
	@cd $(PROTOBUF_ROOT); AR="$(AR)" CC="$(CC)" CXX="$(CXX)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CFLAGS="$(CFLAGS)" ./configure --prefix=$(DEPS_INSTALL_ROOT) --with-protoc=$(DEPS_INSTALL_ROOT)/bin/$(if $(IS_WIN),protoc.exe,protoc) $(if $(IS_WIN_CROSS),--host $(IS_WIN_CROSS),) --disable-shared
	@cd $(PROTOBUF_ROOT); AR="$(AR)" CC="$(CC)" CXX="$(CXX)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CFLAGS="$(CFLAGS)" make clean
	@cd $(PROTOBUF_ROOT); AR="$(AR)" CC="$(CC)" CXX="$(CXX)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CFLAGS="$(CFLAGS)" make -j$(PROCS)
	@cd $(PROTOBUF_ROOT); AR="$(AR)" CC="$(CC)" CXX="$(CXX)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CFLAGS="$(CFLAGS)" make install

libtinyxml: clean
	@mkdir -p $(DEPS_INSTALL_ROOT)/lib
	@mkdir -p $(DEPS_INSTALL_ROOT)/include
	@cd $(TINYXML_ROOT); rm -f libtinyxml.a tinystr.o  tinyxml.o  tinyxmlerror.o  tinyxmlparser.o
	cd $(TINYXML_ROOT); \
 $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c tinystr.cpp  tinyxml.cpp  tinyxmlerror.cpp  tinyxmlparser.cpp
	@cd $(TINYXML_ROOT); $(AR) rcs libtinyxml.a tinystr.o  tinyxml.o  tinyxmlerror.o  tinyxmlparser.o
	@cd $(TINYXML_ROOT); mv libtinyxml.a $(DEPS_INSTALL_ROOT)/lib
	@cd $(TINYXML_ROOT); cp tinyxml.h tinystr.h $(DEPS_INSTALL_ROOT)/include

# we issue "clean" first because our source files use code in
# "safe_nlscheck.h"
NLSCHECK := $(CURDIR)/$(OUT_DIR)/deps/include/safe_nlscheck.h
$(NLSCHECK):
	@mkdir -p /tmp/nlschk && \
         echo '#include <windows.h>' > /tmp/nlschk/chk.c && \
         echo 'NORM_FORM a = NormalizationC;' >> /tmp/nlschk/chk.c && \
	 rm -f /tmp/nlschk/chk.o && \
         $(CC) -o /tmp/nlschk/chk.o -c /tmp/nlschk/chk.c 2>/dev/null >/dev/null; \
         echo > $(NLSCHECK); \
         if [ -e /tmp/nlschk/chk.o ]; then \
         echo '#define SAFE_HAVE_WINNLS' >> $(NLSCHECK); \
         fi

nlscheck: $(NLSCHECK) clean

NORMALIZ_DEP := $(CURDIR)/$(OUT_DIR)/deps/lib/libnormaliz.a
$(NORMALIZ_DEP): $(CURDIR)/normaliz.def
	$(DLLTOOL) -k -d $< -l $@

normaliz: $(NORMALIZ_DEP) clean

# this isn't run by default since we use the prebuild versions in "resources/"
# but can be run manually, then upon build of Safe.exe, we'll pull in
# the locally built version
SAFE_RAMDISK_TARGET = $(DYN_RESOURCES_ROOT)/safe_ramdisk$(if $(IS_WIN64_TARGET),_x64,).sys

safe_ramdisk:
	@echo "Building Safe RAMDisk"
	@cd $(SAFE_RAMDISK_ROOT); cp $(if $(IS_WIN),$(if $(IS_WIN64_TARGET),config-win64.mk,config-win.mk),config-mac.mk) config.mk
	@cd $(SAFE_RAMDISK_ROOT); CXX=$(CXX) CC=$(CC) make RELEASE=$(RELEASE) clean
	@cd $(SAFE_RAMDISK_ROOT); CXX=$(CXX) CC=$(CC) make -j$(PROCS) RELEASE=$(RELEASE)

	@mkdir -p $(DYN_RESOURCES_ROOT)
	@cp $(SAFE_RAMDISK_ROOT)/safe_ramdisk.inf $(DYN_RESOURCES_ROOT)
	@cp $(SAFE_RAMDISK_ROOT)/safe_ramdisk$(if $(IS_WIN64_TARGET),_x64,).sys $(DYN_RESOURCES_ROOT)

# we issue "clean" first because our source files use code in
# "ramdisk_ioctl.h"
safe_ramdisk_headers: clean
	@mkdir -p $(DEPS_INSTALL_ROOT)/include/safe_ramdisk
	cp $(SAFE_RAMDISK_ROOT)/ramdisk_ioctl.h $(DEPS_INSTALL_ROOT)/include/safe_ramdisk

UPDATE_DRIVER_SRCS := \
 src/update_driver/main.cpp \
 src/safe/win/ramdisk.cpp \
 src/safe/exception_backtrace.cpp \
 src/safe/win/exception_backtrace.cpp \
 src/w32util/file.cpp \
 src/w32util/sync.cpp \
 src/safe/win/helper_binary.cpp

# see note for safe_ramdisk: target above,
# basically this is not run automatically when building deps
$(DYN_RESOURCES_ROOT)/update_driver.exe: $(UPDATE_DRIVER_SRCS) \
 src/safe/*.h src/safe/*.hpp src/safe/win/*.hpp src/w32util/*.hpp
	@mkdir -p $(DYN_RESOURCES_ROOT)

	@echo "Build update_driver.exe"
	$(if $(IS_WIN64_TARGET),true,echo "Not a 64-bit compiler" && false)
	$(CXX) -o $@ -DSFX_EMBEDDED $(MY_CPPFLAGS) $(MY_CXXFLAGS) \
 $(ASLR_LINK_FLAGS) -Wl,--wrap,__cxa_throw -mconsole \
 $(if $(RELEASE),-static,) \
 $(UPDATE_DRIVER_SRCS) \
 -lsetupapi -lnewdev -lpsapi
	cp $@ $(DYN_RESOURCES_ROOT)/update_driver-Debug.exe
	$(if $(RELEASE),$(STRIP) -s $@,)
	$(if $(RELEASE),upx --best --all-methods --ultra-brute $@,)
	$(if $(RELEASE),signtool sign //v //s MY //n "Rian Hunter" //fd sha1 //t "http://timestamp.digicert.com" $@,)

update_driver: $(DYN_RESOURCES_ROOT)/update_driver.exe

MYPOWRPROF_DEP := $(CURDIR)/$(OUT_DIR)/deps/lib/libmypowrprof.a
$(MYPOWRPROF_DEP): $(CURDIR)/mypowrprof.def
	$(DLLTOOL) -k -d $< -l $@

mypowrprof: $(MYPOWRPROF_DEP) clean

dependencies: libprotobuf libtinyxml \
 $(if $(IS_WIN_TARGET),libbotan,) \
 libencfs \
 libwebdav_server_fs \
 $(if $(IS_WIN_TARGET),normaliz nlscheck safe_ramdisk_headers mypowrprof,)

clean-deps:
	rm -rf $(OUT_DIR)

clean:
	rm -f $(EXE_NAME) $(DEBUG_EXE_NAME)
	rm -f src/safe/*.o
	rm -f src/safe/win/*.o
	rm -f src/w32util/*.o
	rm -rf $(DYN_RESOURCES_ROOT)

FS_SRCS = fs_fsio.cpp CFsToFsIO.cpp fs.cpp \
	SecureMemPasswordReader.cpp UnicodeWrapperFsIO.cpp \
	$(if $(IS_WIN_TARGET),win/unicode_fs.cpp,) \
	$(if $(IS_MAC_TARGET),mac/unicode_fs.mm,)

TEST_ENCFS_MAIN_SRCS = test_encfs_main.cpp $(FS_SRCS)
TEST_ENCFS_MAIN_OBJS = $(patsubst %,src/safe/%.o,${TEST_ENCFS_MAIN_SRCS})

W32UTIL_SRCS = \
 gui_util.cpp \
 menu.cpp \
 file.cpp \
 shell.cpp \
 sync.cpp

APP_SRCS = \
 create_safe_dialog_logic.cpp \
 mount_safe_dialog_logic.cpp \
 report_exception.cpp \
 open_url.cpp \
 exception_backtrace.cpp \
 webdav_server.cpp \
 $(FS_SRCS)

WIN_APP_SRCS = \
 app.rc \
 mount.cpp \
 about_dialog.cpp \
 app_main.cpp \
 create_safe_dialog.cpp \
 mount_safe_dialog.cpp \
 welcome_dialog.cpp \
 general_safe_dialog.cpp \
 system_changes_dialog.cpp \
 ramdisk.cpp \
 guids.cpp \
 report_bug_dialog.cpp \
 util.cpp \
 exception_backtrace.cpp \
 helper_binary.cpp

WINDOWS_APP_MAIN_OBJS = \
 $(patsubst %,src/safe/%.o,${APP_SRCS}) \
 $(patsubst %,src/safe/win/%.o,${WIN_APP_SRCS}) \
 $(patsubst %,src/w32util/%.o,${W32UTIL_SRCS})

# dependencies

src/safe/*.o: GNUmakefile src/safe/*.hpp src/safe/*.h src/w32util/*.hpp src/safe/win/*.hpp
src/safe/win/*.o: GNUmakefile src/safe/*.hpp src/safe/*.h src/w32util/*.hpp src/safe/win/*.hpp
src/w32util/*.o: GNUmakefile src/safe/*.hpp src/safe/*.h src/w32util/*.hpp

src/safe/win/*.rc.o: $(RESOURCES_ROOT)/* $(wildcard $(DYN_RESOURCES_ROOT)/*)

test_encfs_main: $(TEST_ENCFS_MAIN_OBJS) $(ENCFS_STATIC_LIBRARY) \
	$(WEBDAV_SERVER_STATIC_LIBRARY) GNUmakefile

$(DEBUG_EXE_NAME): $(WINDOWS_APP_MAIN_OBJS) $(ENCFS_STATIC_LIBRARY) \
	$(WEBDAV_SERVER_STATIC_LIBRARY) GNUmakefile

# build instructions

%.cpp.o: %.cpp
	$(CXX) $(MY_CPPFLAGS) $(MY_CXXFLAGS) -c -o $@ $<

%.mm.o: %.mm
	$(CXX) $(MY_CPPFLAGS) $(MY_CXXFLAGS) -c -o $@ $<

%.rc.o: %.rc
	$(WINDRES) $(MY_CPPFLAGS) -I$(DYN_RESOURCES_ROOT) -I$(RESOURCES_ROOT) -I./src -i $< -o $@

test_encfs_main:
	$(CXX) -L$(DEPS_INSTALL_ROOT)/lib $(MY_CXXFLAGS) \
 -o $@ $(TEST_ENCFS_MAIN_OBJS) $(DEPS_LIBRARIES) $(DEPS_EXTRA_LIBRARIES)

# NB: when debugging on WINE make sure it can find libstdc++

$(DEBUG_EXE_NAME):
	$(CXX) $(ASLR_LINK_FLAGS) $(WINDOWS_SUBSYS_LINK_FLAGS) -Wl,--wrap,__cxa_throw \
        $(if $(RELEASE),$(LDFLAGS_RELEASE),$(LDFLAGS_DEBUG)) \
        $(if $(RELEASE),-static,) \
 -L$(DEPS_INSTALL_ROOT)/lib $(MY_CXXFLAGS) -o $@ $(WINDOWS_APP_MAIN_OBJS) \
 $(DEPS_LIBRARIES) $(DEPS_EXTRA_LIBRARIES) \
 -lole32 -lcomctl32 -lnormaliz -lsetupapi -lnewdev -lpsapi -lmypowrprof

$(EXE_NAME): $(DEBUG_EXE_NAME)
	cp $< $@.notsigned
ifneq ($(RELEASE),)
	$(STRIP) -s $@.notsigned
	upx --best --all-methods --ultra-brute $@.notsigned
ifneq ($(IS_WIN),)
	signtool sign //v //s MY //n "Rian Hunter" //fd sha1 //t "http://timestamp.digicert.com" $@.notsigned
	mv $@.notsigned $@
else
	@echo 'Signing...'; \
 if [ -z $$SAFE_PFX_PATH ]; then printf 'Path to pfx file:'; read SAFE_PFX_PATH; fi; \
 if [ -z $$SAFE_PFX_PASSWORD ]; then \
 stty -echo; printf 'Password:'; \
 read SAFE_PFX_PASSWORD; \
 stty echo; \
 echo; \
 fi; \
 osslsigncode sign -pass $$SAFE_PFX_PASSWORD -h sha1 -t "http://timestamp.digicert.com" -pkcs12 $$SAFE_PFX_PATH -in $@.notsigned -out $@
endif
endif

.PHONY: dependencies clean libbotan \
	libprotobuf libtinyxml libencfs libwebdav_server_fs nlscheck normaliz clean-deps safe_ramdisk clean-exe
