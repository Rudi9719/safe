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

#ifndef _safe_constants_h
#define _safe_constants_h

#include <safe/product_name.h>
#include <safe/version.h>

#define SAFE_APP_STARTED_COOKIE_FILENAME "AppStarted"
#define SAFE_RECENTLY_USED_PATHS_DB_FILE_NAME "RecentPaths.db"
#define SAFE_REMEMBER_PASSWORD_COOKIE_FILENAME "RememberPasswordChecked"
#define SAFE_DONT_SHOW_SYSTEM_CHANGES_INSTALL_COOKIE_FILENAME "DontShowSystemChangesInstall"
#define SAFE_DONT_SHOW_WINXP_WARNING_COOKIE_FILENAME "DontShowWindowsXPWarning"

enum {
  SAFE_RECENTLY_USED_PATHS_MENU_NUM_ITEMS=10,
};

#define SAFE_SOURCE_CODE_WEBSITE "http://getsafe.org/app/get_source_code"
#define SAFE_SEND_FEEDBACK_WEBSITE "http://getsafe.org/app/send_feedback"
#define SAFE_WINDOWS_SYSTEM_CHANGES_INFO_WEBSITE "http://getsafe.org/app/windows_system_changes_more_info"
#define SAFE_MAC_SYSTEM_CHANGES_INFO_WEBSITE "http://getsafe.org/app/mac_system_changes_more_info"
#define SAFE_REPORT_EXCEPTION_WEBSITE "http://getsafe.org/app/report_exception"
#define SAFE_VISIT_WEBSITE_WEBSITE "http://getsafe.org/app/visit_website"
#define SAFE_WINXP_MORE_INFO_WEBSITE "http://getsafe.org/app/winxp_more_info"

/* UI Strings follow */

#define SAFE_TRAY_ICON_TOOLTIP PRODUCT_NAME_A

#define SAFE_PROGRESS_CREATING_TITLE ("Creating New " ENCRYPTED_STORAGE_NAME_A "...")
#define SAFE_PROGRESS_CREATING_MESSAGE ("Creating new " ENCRYPTED_STORAGE_NAME_A "...")

#define SAFE_PROGRESS_MOUNTING_TITLE ("Mounting New " ENCRYPTED_STORAGE_NAME_A "...")
#define SAFE_PROGRESS_MOUNTING_MESSAGE ("Mounting new " ENCRYPTED_STORAGE_NAME_A "...")

#define SAFE_PROGRESS_MOUNTING_EXISTING_TITLE ("Mounting Existing " ENCRYPTED_STORAGE_NAME_A "...")
#define SAFE_PROGRESS_MOUNTING_EXISTING_MESSAGE ("Mounting existing " ENCRYPTED_STORAGE_NAME_A "...")

#define SAFE_PROGRESS_VERIFYING_PASS_TITLE ("Verifying " ENCRYPTED_STORAGE_NAME_A " Password...")
#define SAFE_PROGRESS_VERIFYING_PASS_MESSAGE ("Verifying " ENCRYPTED_STORAGE_NAME_A " password...")

#define SAFE_PROGRESS_READING_CONFIG_TITLE ("Reading " ENCRYPTED_STORAGE_NAME_A " Configuration...")
#define SAFE_PROGRESS_READING_CONFIG_MESSAGE ("Reading " ENCRYPTED_STORAGE_NAME_A " configuration...")

#define SAFE_DIALOG_PASS_INCORRECT_TITLE "Password is Incorrect"
#define SAFE_DIALOG_PASS_INCORRECT_MESSAGE "The password you have entered is incorrect."

#define SAFE_DIALOG_NO_CONFIG_EXISTS_TITLE ("Not a " ENCRYPTED_STORAGE_NAME_A)
#define SAFE_DIALOG_NO_CONFIG_EXISTS_MESSAGE ("The location you selected is not a " ENCRYPTED_STORAGE_NAME_A ".")

#define SAFE_DIALOG_QUIT_CONFIRMATION_TITLE ("Quit " PRODUCT_NAME_A " Now?")
#define SAFE_DIALOG_QUIT_CONFIRMATION_MESSAGE ("You currently have one or more " ENCRYPTED_STORAGE_NAME_A "s mounted, if you quit they will not be accessible until you run " PRODUCT_NAME_A " again. Quit " PRODUCT_NAME_A " Now?")

#define SAFE_DIALOG_UNKNOWN_CREATE_ERROR_TITLE "Unknown Error"
#define SAFE_DIALOG_UNKNOWN_CREATE_ERROR_MESSAGE ("Unknown error occurred while creating new " ENCRYPTED_STORAGE_NAME_A ".")

#define SAFE_DIALOG_UNKNOWN_MOUNT_ERROR_TITLE "Unknown Error"
#define SAFE_DIALOG_UNKNOWN_MOUNT_ERROR_MESSAGE ("Unknown error occurred while mounting existing " ENCRYPTED_STORAGE_NAME_A ".")

#define SAFE_DIALOG_ABOUT_TITLE ("About " PRODUCT_NAME_A)
#define SAFE_DIALOG_WELCOME_TITLE ("Welcome to " PRODUCT_NAME_A "!")

#define SAFE_TRAY_ICON_WELCOME_TITLE (PRODUCT_NAME_A " is now Running!")
#define SAFE_TRAY_ICON_WELCOME_MSG                                   \
  (                                                                     \
   "If you need to use "                                                \
   PRODUCT_NAME_A                                                       \
   ", just right-click on this icon."                                   \
                                                      )
#define SAFE_TRAY_ICON_MAC_WELCOME_MSG ("If you need to use " PRODUCT_NAME_A ", just click on the " PRODUCT_NAME_A " menu bar icon.")

#define SAFE_DIALOG_WELCOME_TEXT \
  (PRODUCT_NAME_A " is now running. You can use it by clicking "        \
   "the " PRODUCT_NAME_A " icon in the tray. Get started by clicking "  \
   "one of the actions below:")
#define SAFE_DIALOG_WELCOME_TEXT_POST_DRIVER_INSTALL \
  ("Congrats! Your system is now configured to use " PRODUCT_NAME_A ". " \
   "Get started by clicking one of the actions below:")

#define SAFE_DIALOG_WELCOME_CREATE_BUTTON \
  ("Create New " ENCRYPTED_STORAGE_NAME_A "...")
#define SAFE_DIALOG_WELCOME_MOUNT_BUTTON \
  ("Mount Existing " ENCRYPTED_STORAGE_NAME_A "...")

#define SAFE_NOTIFICATION_TEST_TITLE "Short Title"
#define SAFE_NOTIFICATION_TEST_MESSAGE \
  ("Very long message full of meaningful info that you " \
   "will find very interesting because you love to read " \
   "tray icon bubbles. Don't you? Don't you?!?!")

#define SAFE_DIALOG_ABOUT_TAGLINE "Protect your files."
#define SAFE_DIALOG_ABOUT_VERSION "Version " SAFE_VERSION_STR

#define SAFE_DIALOG_ABOUT_BYLINE (PRODUCT_NAME_A " is free software.")

#define SAFE_TRAY_ICON_SEND_FEEDBACK "Send Feedback..."

#define SAFE_DIALOG_REBOOT_CONFIRMATION_TITLE ("Reboot Now?")
#define SAFE_DIALOG_REBOOT_CONFIRMATION_MESSAGE \
  ("Your PC must be rebooted before it is secure enough to use " \
   PRODUCT_NAME_A ". If you cancel you can reboot on your own later.")
#define SAFE_DIALOG_REBOOT_CONFIRMATION_MESSAGE_MAC \
  ("Your computer must be rebooted before it is secure enough to use " \
   PRODUCT_NAME_A ". You can also quit " PRODUCT_NAME_A " and reboot on " \
   "your own later. Reboot now?")

#define SAFE_DIALOG_SYSTEM_CHANGES_TITLE "System Changes Required"
#define SAFE_DIALOG_SYSTEM_CHANGES_HEADER \
  "Before using Safe, we recommend making the following changes to your system:"
#define SAFE_DIALOG_SYSTEM_CHANGES_BULLET "•"
#define SAFE_DIALOG_SYSTEM_CHANGES_FOOTER \
  "These changes are critical for ensuring your privacy when using Safe but "\
  "may not be necessary for your use case. If in doubt, we strongly recommend "\
  "making these changes."
#define SAFE_DIALOG_SYSTEM_CHANGES_MAKE_CHANGES "Make Changes"
#define SAFE_DIALOG_SYSTEM_CHANGES_DONT_MAKE_CHANGES "Don't Make Changes"
#define SAFE_DIALOG_SYSTEM_CHANGES_MORE_INFO "More Info"

#define SAFE_PROGRESS_SYSTEM_CHANGES_TITLE "Making System Changes"
#define SAFE_PROGRESS_SYSTEM_CHANGES_MESSAGE "Making System Changes..."

#endif
