/*
  Safe: Encrypted File System
  Copyright (C) 2014 Rian Hunter <rian@alum.mit.edu>

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

#include <safe/win/ramdisk.hpp>
#include <safe/win/helper_binary.hpp>

#include <w32util/error.hpp>
#include <w32util/string.hpp>

#include <windows.h>
#include <shellapi.h>

extern "C"
int main() {
  return safe::win::helper_binary_main
    ([] (std::vector<std::string> arguments) {
      auto hardware_id = arguments.at(0);
      auto inf_file_path = arguments.at(1);

      auto restart_required =
        safe::win::create_device_and_install_driver_native(hardware_id,
                                                           inf_file_path);

      return restart_required
        ? ERROR_SUCCESS_REBOOT_REQUIRED
        : ERROR_SUCCESS;
    });
}
