#ifndef R_WIN32_FILE_DIALOG_HPP
#define R_WIN32_FILE_DIALOG_HPP

#include <Windows.h>
#include <r-common.hpp>

#include "r-win32-window.hpp"

typedef r_handle RWin32FileDialogHandle;

namespace r_win32 {

    r_external const RWin32FileDialogHandle file_dialog_create  (const RWin32WindowHandle     parent_window_handle);
    r_external const r_b8                   file_dialog_destroy (const RWin32FileDialogHandle file_dialog_handle);

    r_external const r_b8 
    file_dialog_select_file(
        const RWin32FileDialogHandle in_file_dialog_handle,
        const r_cstr                 in_starting_directory,
        const r_size                 in_extension_count,
        const r_size                 in_extension_stride,
        const r_cstr                 in_extension_buffer,
        const r_size                 in_selected_file_buffer_size,
              r_cstr                out_selected_file_buffer);
};


#endif //R_WIN32_FILE_DIALOG_HPP