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

// provides the necessary fs operations to make the file system
// behavor like a native win fs (e.g. case-insensitive compare)

#ifndef _unicode_fs_mac_hpp
#define _unicode_fs_mac_hpp

#include <string>

namespace safe { namespace unicode_fs { namespace win {

std::string
normalize_path_component_for_fs(const std::string & comp);

std::string
normalize_path_component_for_user(const std::string & comp);

bool
is_normalized_path_component(const std::string & comp);

bool
normalized_path_components_equal(const std::string & comp_a,
                                 const std::string & comp_b);

}}}

#endif
