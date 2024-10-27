#pragma once

#include "r-win32-internal-file-dialog.hpp"
#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const RWin32FileDialogHandle
r_win32::file_dialog_create(
    const RWin32WindowHandle parent_window_handle) {

    //sanity check
    if (!parent_window_handle) {
        return(NULL);
    }

    //commit a dialog
    RWin32FileDialog* file_dialog_ptr = r_win32_internal::file_dialog_commit();
    if (!file_dialog_ptr) {
        return(NULL);
    }

    //set the parent window handle
    file_dialog_ptr->win32_parent_window_handle = r_win32::window_win32_handle(parent_window_handle);

    //we're done
    return(file_dialog_ptr);
}

r_external const r_b8
r_win32::file_dialog_destroy(
    const RWin32FileDialogHandle file_dialog_handle) {

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

r_external const r_b8 
r_win32::file_dialog_reset(
    const RWin32FileDialogHandle file_dialog_handle) {

    //sanity check
    if (!file_dialog_handle) {
        return(false);
    }

    //cast the file dialog
    RWin32FileDialog* file_dialog_ptr = (RWin32FileDialog*)file_dialog_handle;

    //clear the dialog data and release it
    if (file_dialog_ptr->win32_file_dialog_ptr) {
        file_dialog_ptr->win32_file_dialog_ptr->ClearClientData();
        file_dialog_ptr->win32_file_dialog_ptr->Release();    
    }

    //cache the arena
    const RMemoryArenaHandle file_dialog_arena = file_dialog_ptr->arena_handle;
    if (!file_dialog_arena) {
        return(false);
    }

    //calculate the size to pull
    const r_size file_dialog_size_aligned = r_align_size_struct(RWin32FileDialog);
    const r_size arena_size_used          = r_mem::arena_size_used(file_dialog_arena);
    const r_size arena_size_to_pull       = arena_size_used - file_dialog_size_aligned; 
    
    //reset the arena up to the memory for the dialog struct
    const r_memory arena_memory = r_mem::arena_pull(file_dialog_arena,arena_size_to_pull);
    const r_b8     result       = arena_memory != NULL;

    //we're done
    return(result);
}

r_external const r_b8 
r_win32::file_dialog_open(
    const RWin32FileDialogHandle file_dialog_handle,
    const r_cstr                 file_dialog_starting_directory,
    const r_size                 file_type_count,
    const r_wstr*                file_type_name_wstr_ptr,
    const r_wstr*                file_type_spec_wstr_ptr) {

    //sanity check
    if (
        file_dialog_handle             == NULL ||
        file_dialog_starting_directory == NULL ||
        file_type_count                == 0    ||
        file_type_name_wstr_ptr        == NULL ||
        file_type_spec_wstr_ptr        == NULL) {

        return(false);
    }

    //reset the dialog
    if (!r_win32::file_dialog_reset(file_dialog_handle)) {
        return(false);
    }

    //cast the file dialog
    RWin32FileDialog* file_dialog_ptr = (RWin32FileDialog*)file_dialog_handle;

    //get a win32 dialog pointer
    HRESULT result = 
        CoCreateInstance(
            CLSID_FileOpenDialog, 
            NULL, 
            CLSCTX_ALL, 
            IID_IFileDialog, 
            (void**)(&file_dialog_ptr->win32_file_dialog_ptr));

    //if that didn't work, we're done
    if (!SUCCEEDED(result)) {
        return(false);
    }

    //cache needed references
    const HWND               win32_parent_window = file_dialog_ptr->win32_parent_window_handle;
    const RMemoryArenaHandle file_dialog_arena   = file_dialog_ptr->arena_handle;
          IFileDialog*       win32_dialog_ptr    = file_dialog_ptr->win32_file_dialog_ptr;

    //push the win32 file type struct on the arena
    COMDLG_FILTERSPEC* win32_file_types_ptr = r_mem_arena_push_array(file_dialog_arena,file_type_count,COMDLG_FILTERSPEC);
    if (!win32_file_types_ptr) {
        return(false);
    }

    //initialize the file types
    for (
        r_index file_type_index = 0;
        file_type_index < file_type_count;
        ++file_type_index) {

        //cache the string arguments
        const r_wstr in_file_type_name_current = file_type_name_wstr_ptr[file_type_index];
        const r_wstr in_file_type_spec_current = file_type_spec_wstr_ptr[file_type_index];        

        //push the strings onto the arena
        const r_wstr dialog_file_type_name_current = r_mem::arena_push_wstr(file_dialog_arena,R_WIN32_FILE_DIALOG_WSTR_MAX_SIZE,in_file_type_name_current);
        const r_wstr dialog_file_type_spec_current = r_mem::arena_push_wstr(file_dialog_arena,R_WIN32_FILE_DIALOG_WSTR_MAX_SIZE,in_file_type_spec_current);

        //sanity check, make sure we pushed the strings
        if (
            dialog_file_type_name_current == NULL ||
            dialog_file_type_spec_current == NULL) {

            return(false);
        }

        //add the strings to the file type properties
        win32_file_types_ptr[file_type_index].pszName = dialog_file_type_name_current; 
        win32_file_types_ptr[file_type_index].pszSpec = dialog_file_type_spec_current; 
    }

    //clear the dialog data
    win32_dialog_ptr->ClearClientData();

    //set the file types in the dialog
    win32_dialog_ptr->SetFileTypes(file_type_count,win32_file_types_ptr);

    //set the options to open file
    win32_dialog_ptr->SetOptions(FOS_FILEMUSTEXIST);

    //set filter index
    win32_dialog_ptr->SetFileTypeIndex(1);

    //show the dialog
    win32_dialog_ptr->Show(win32_parent_window);

    //whatever happens, we're done
    return(true);
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
