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

#ifndef __windows_gui_util_hpp
#define __windows_gui_util_hpp

#include <encfs/cipher/MemoryPool.h>
#include <encfs/base/optional.h>

#include <string>

#include <safe/lean_windows.h>

namespace w32util {

void
quick_alert(HWND owner,
            const std::string &msg,
            const std::string &title);

BOOL
SetClientSizeInLogical(HWND hwnd, bool set_pos,
                       int x, int y,
                       int w, int h);

void
center_window_in_monitor(HWND hwnd);

LOGFONT
get_message_font();

void
set_default_dialog_font(HWND hwnd);

void
cleanup_default_dialog_font(HWND hwnd);

void
clear_text_field(HWND text_hwnd, size_t num_chars);

std::string
read_text_field(HWND text_hwnd);

encfs::SecureMem
securely_read_text_field(HWND text_wnd, bool clear = true);

opt::optional<std::string>
get_folder_dialog(HWND owner);

void
open_url_in_browser(HWND owner, std::string url);

LONG
compute_point_size_of_logfont(HWND hwnd, const LOGFONT & lplf);

SIZE
compute_base_units_of_logfont(HWND hwnd, const LOGFONT & lplf);

LONG
get_text_height(HWND hwnd, const LOGFONT & lf,
                const std::string & text, LONG width_in_logical_units);

}

#endif
