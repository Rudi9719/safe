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

#ifndef __Safe__system_changes_mac__
#define __Safe__system_changes_mac__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace safe { namespace mac {

bool
full_disk_encryption_is_active_on_startup_disk();

typedef std::function<void(const char *, const char *const [])> ShellRun;

std::vector<std::string>
required_system_changes();

bool
make_required_system_changes_common(ShellRun shell_run);

}}

#endif /* defined(__Safe__system_changes_mac__) */
