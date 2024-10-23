#ifndef R_WIN32_FILE_DIALOG_HPP
#define R_WIN32_FILE_DIALOG_HPP

#include <Windows.h>
#include <r-common.hpp>

#include "r-win32-window.hpp"

typedef r_handle RFileDialogHandle;

namespace r_win32 {

    r_external const RFileDialogHandle file_dialog_create  (const RWin32WindowHandle parent_window_handle);
    r_external const r_b8              file_dialog_destroy (const RFileDialogHandle  file_dialog_handle);
};


#endif //R_WIN32_FILE_DIALOG_HPP