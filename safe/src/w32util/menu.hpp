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

#ifndef __w32util_windows_menu_hpp
#define __w32util_windows_menu_hpp

#include <string>

#include <safe/lean_windows.h>

namespace w32util {

void
menu_append_separator(HMENU menu_handle);

int
menu_append_string_item(HMENU menu_handle, bool is_default,
                        std::string text, UINT id);

void
menu_clear(HMENU menu_handle);

}

#endif
