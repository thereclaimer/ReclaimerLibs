#pragma once

#include "r-win32-internal-file-dialog.hpp"
#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const RFileDialogHandle
r_win32::file_dialog_create(
    const RWin32WindowHandle parent_window_handle) {

    //sanity check
    if (!parent_window_handle) {
        return(NULL);
    }

    //get a win32 dialog pointer
    IFileDialog* win32_dialog_ptr;
    HRESULT result = 
        CoCreateInstance(
            CLSID_FileOpenDialog, 
            NULL, 
            CLSCTX_ALL, 
            IID_IFileDialog, 
            reinterpret_cast<void**>(&win32_dialog_ptr));

    //if that didn't work, we're done
    if (!SUCCEEDED(result)) {
        return(NULL);
    }

    //commit a dialog
    RWin32FileDialog* file_dialog_ptr = r_win32_internal::file_dialog_commit();
    if (!file_dialog_ptr) {
        return(NULL);
    }

    //initialize the file dialog
    file_dialog_ptr->win32_file_dialog_ptr      = win32_dialog_ptr;
    file_dialog_ptr->win32_parent_window_handle = r_win32::window_win32_handle(parent_window_handle);

    //we're done
    return(file_dialog_ptr);
}

r_external const r_b8
r_win32::file_dialog_destroy(
    const RFileDialogHandle  file_dialog_handle) {

    //sanity check
    if (!file_dialog_handle) {
        return(false);
    }

    //cast the pointer
    RWin32FileDialog* file_dialog_ptr = (RWin32FileDialog*)file_dialog_handle;

    //decommit the dialog
    const r_b8 result = r_win32_internal::file_dialog_decommit(file_dialog_ptr);

    //we're done
    return(result);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal RWin32FileDialog* 
r_win32_internal::file_dialog_commit(
    r_void) {

    //get an arena
    const RMemoryArenaHandle arena_handle = r_win32_internal::context_arena_commit();
    if (!arena_handle) {
        return(NULL);
    }

    //push the dialog struct
    RWin32FileDialog* file_dialog_ptr = r_mem_arena_push_struct(arena_handle,RWin32FileDialog);
    if (!file_dialog_ptr) {
        r_win32_internal::context_arena_decommit(arena_handle);
        return(NULL);        
    }

    //initialize the struct
    file_dialog_ptr->arena_handle                  = arena_handle;
    file_dialog_ptr->win32_file_dialog_ptr         = NULL;
    file_dialog_ptr->win32_shell_item              = NULL;
    file_dialog_ptr->win32_shell_item_display_name = NULL;

    //we're done
    return(file_dialog_ptr);
}    

r_internal const r_b8
r_win32_internal::file_dialog_decommit(
    RWin32FileDialog* file_dialog_ptr) {

    //sanity check
    if (!file_dialog_ptr) {
        return(false);
    }

    //decommit the arena
    const r_b8 result = r_win32_internal::context_arena_decommit(file_dialog_ptr->arena_handle);

    //we're done
    return(result);
}
