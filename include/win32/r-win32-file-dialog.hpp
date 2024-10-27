#ifndef R_WIN32_FILE_DIALOG_HPP
#define R_WIN32_FILE_DIALOG_HPP

#include <Windows.h>
#include <r-common.hpp>

#include "r-win32-window.hpp"

typedef r_handle RWin32FileDialogHandle;

#define R_WIN32_FILE_DIALOG_WSTR_MAX_SIZE 128

namespace r_win32 {

    r_external const RWin32FileDialogHandle file_dialog_create  (const RWin32WindowHandle     parent_window_handle);
    r_external const r_b8                   file_dialog_destroy (const RWin32FileDialogHandle file_dialog_handle);
    r_external const r_b8                   file_dialog_reset   (const RWin32FileDialogHandle file_dialog_handle);

    r_external const r_b8 
    file_dialog_open(
        const RWin32FileDialogHandle file_dialog_handle,
        const r_cstr                 file_dialog_starting_directory,
        const r_size                 file_type_count,
        const r_wstr*                file_type_name_wstr_ptr,
        const r_wstr*                file_type_spec_wstr_ptr);

    r_external const r_wstr
    file_dialog_get_selection_as_path_wstr(
        const RWin32FileDialogHandle in_file_dialog_handle);
};

#endif //R_WIN32_FILE_DIALOG_HPP