#ifndef R_WIN32_INTERNAL_FILE_DIALOG_HPP
#define R_WIN32_INTERNAL_FILE_DIALOG_HPP

#include <shlobj_core.h>

#include <r-memory.hpp>

#include "r-win32-file-dialog.hpp"

struct RWin32FileDialog {
    RMemoryArenaHandle arena_handle;
    IFileDialog*       win32_file_dialog_ptr;
    IShellItem*        win32_shell_item;
    LPWSTR             win32_shell_item_display_name;
    HWND               win32_parent_window_handle;
};

namespace r_win32_internal {

    r_internal       RWin32FileDialog* file_dialog_commit   (r_void);    
    r_internal const r_b8              file_dialog_decommit (RWin32FileDialog* file_dialog_ptr);    

};

#endif //R_WIN32_INTERNAL_FILE_DIALOG_HPP