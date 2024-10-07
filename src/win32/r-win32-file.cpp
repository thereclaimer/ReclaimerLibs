#pragma once

#include "r-win32-internal-file.hpp"
#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const r_b8 
r_win32::file_exists (
    const r_cstr file_path) {

    return(true);
}

r_external const RWin32FileHandle 
r_win32::file_open_existing(
    const RWin32FilePermission file_permission, 
    const r_cstr               file_path) {

    //set the access and share mode
    const DWORD win32_file_access = 
        file_permission == RWin32FilePermission_ReadOnly 
        ? GENERIC_READ
        : GENERIC_READ | GENERIC_WRITE;

    const DWORD win32_file_share_mode = 
        file_permission == RWin32FilePermission_ReadOnly 
        ? FILE_SHARE_READ
        : FILE_SHARE_READ | FILE_SHARE_WRITE;

    //first, attempt to open the file
    HANDLE win32_file_handle = 
        CreateFile(
            file_path,
            win32_file_access,
            win32_file_share_mode,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL);

    //if that failed, we're done
    if (!win32_file_handle) {
        return(NULL);
    }

    //initialize the file
    const RWin32FileHandle r_file_handle = 
        r_win32_internal::file_initialize(
            win32_file_handle,
            file_permission);

    //if that failed, close the handle we opened
    if (!r_file_handle) {
        CloseHandle(win32_file_handle);
    }

    //we're done
    return(r_file_handle);
}

r_external const RWin32FileHandle 
r_win32::file_create_new(
    const r_cstr file_path) {

    //first, attempt to open the file
    HANDLE win32_file_handle = 
        CreateFile(
            file_path,
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL);

    //if that failed, we're done
    if (!win32_file_handle) {
        return(NULL);
    }

    //initialize the file
    const RWin32FileHandle r_file_handle = 
        r_win32_internal::file_initialize(
            win32_file_handle,
            RWin32FilePermission_ReadWrite);

    //if that failed, close the handle we opened
    if (!r_file_handle) {
        CloseHandle(win32_file_handle);
    }

    //we're done
    return(r_file_handle);
}

r_external const r_b8 
r_win32::file_close(
    const RWin32FileHandle file_handle) {

    return(true);
}

r_external const r_b8 
r_win32::file_is_open(
    const RWin32FileHandle file_handle) {

    //get the file
    RWin32File* file_ptr = (RWin32File*)file_handle;

    //get the win32 handle
    const HANDLE win32_handle = r_win32_internal::file_table_read_win32_handle(file_ptr); 

    //if the win32 handle is valid, its open
    const r_b8 file_is_open = win32_handle != NULL; 

    //we're done
    return(file_is_open);
}

r_external const r_b8 
r_win32::file_can_write(
    const RWin32FileHandle file_handle) {

    //get the file
    RWin32File* file_ptr = (RWin32File*)file_handle;

    //get the file permission
    const RWin32FilePermission file_permission = r_win32_internal::file_table_read_permission(file_ptr);

    //if its read write, they can write
    //also, water is wet [citation needed]
    const r_b8 file_can_write = file_permission == RWin32FilePermission_ReadWrite;

    //we're done
    return(file_can_write);
}

r_external const r_size 
r_win32::file_size(
    const RWin32FileHandle file_handle) {

    //get the file
    RWin32File* file_ptr = (RWin32File*)file_handle;

    //get the file size
    const r_size file_size = r_win32_internal::file_table_read_size(file_ptr);

    //we're done
    return(file_size);
}

r_external const r_cstr 
r_win32::file_path(
    const RWin32FileHandle file_handle) {

    return(NULL);
}

r_external const r_b8
r_win32::file_read(
    const RWin32FileHandle in_file_handle,
    const r_size           in_file_read_start,
    const r_size           in_file_read_length,
          r_memory        out_file_read_buffer) {

    return(true);
}

r_external const r_b8
r_win32::file_write(
    const RWin32FileHandle in_file_handle,
    const r_size           in_file_write_start,
    const r_size           in_file_write_length,
          r_memory        out_file_write_buffer) {

    return(true);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal const r_size 
r_win32_internal::file_table_count_open_files(
    RWin32FileTable* file_table_ptr) {

    if (!file_table_ptr) {
        return(0);
    }

    r_size count = 0;
    for (
        r_index file_table_row = 0;
        file_table_row < file_table_ptr->row_count;
        ++file_table_row) {

        count += file_table_ptr->columns.win32_handle[file_table_row] != NULL ? 1 : 0;
    }

    return(count);
}

r_internal const r_b8
r_win32_internal::file_table_can_add_file(
    RWin32FileTable* file_table_ptr) {
    
    r_b8 can_open = false;

    for (
        r_index file_table_row = 0;
        file_table_row < file_table_ptr->row_count;
        ++file_table_row) {

        can_open |= !file_table_ptr->columns.win32_handle[file_table_row];
    }

    return(can_open);
}

r_internal RWin32FileTable* 
r_win32_internal::file_table_next_available(
    RWin32FileManager& file_manager_ref) {

    RWin32FileTable* next_available_table_ptr = NULL;
    for (
        RWin32FileTable* file_table_ptr = file_manager_ref.file_table_list;
        file_table_ptr != NULL;
        file_table_ptr = file_table_ptr->next) {

        if (r_win32_internal::file_table_can_add_file(file_table_ptr)) {
            next_available_table_ptr = file_table_ptr;
            break;            
        }
    }

    return(next_available_table_ptr);
}

r_internal RWin32FileTable* 
r_win32_internal::file_table_commit(
    RWin32FileManager& file_manager_ref) {

    //get the file table ptr
    RWin32FileTable* file_table_ptr = r_win32_internal::context_file_table_commit();
    if (!file_table_ptr) {
        return(NULL);
    }

    //now we need to push the column memory onto the arena
    //the largest column is the file struct
    const r_size file_struct_size = sizeof(RWin32File);

    //we need to get how much remaining memory we have in the arena
    const r_size memory_remaining = r_mem::arena_size_free(file_table_ptr->arena_handle);
    
    //this is the amount of columns we have in the table
    const r_size column_count = 4;

    //this is the total size of each column
    const r_size column_size = memory_remaining / column_count;

    //the row count is the column size divided by the file struct size
    file_table_ptr->row_count = column_size / file_struct_size;

    //now we can create the columns
    file_table_ptr->columns.file         =           (RWin32File*)r_mem::arena_push(file_table_ptr->arena_handle, column_size); 
    file_table_ptr->columns.win32_handle =               (HANDLE*)r_mem::arena_push(file_table_ptr->arena_handle, column_size); 
    file_table_ptr->columns.size         =               (r_size*)r_mem::arena_push(file_table_ptr->arena_handle, column_size); 
    file_table_ptr->columns.permission   = (RWin32FilePermission*)r_mem::arena_push(file_table_ptr->arena_handle, column_size); 

    //calculate the initial offset
    const r_address file_table_starting_offset = (r_memory)file_table_ptr->columns.file - (r_memory)file_table_ptr;

    //initialize the file indexes
    for (
        r_index file_index = 0;
        file_index < file_table_ptr->row_count;
        ++file_index) {

        file_table_ptr->columns.file[file_index].file_index        = file_index;
        file_table_ptr->columns.file[file_index].file_table_offset = file_table_starting_offset + (sizeof(RWin32File) * file_index);
    }

    //add the file table to the list
    file_table_ptr->next = file_manager_ref.file_table_list; 
    if (file_manager_ref.file_table_list) {
        file_manager_ref.file_table_list->previous = file_table_ptr;
    }
    file_manager_ref.file_table_list = file_table_ptr;

    //we're done
    return(file_table_ptr);
}

r_internal const r_b8 
r_win32_internal::file_next_available(
    RWin32FileTable* in_file_table_ptr,
    RWin32File&     out_file_ref) {

    //loop through the file table for an available index
    for (
        r_index file_index = 0;
        file_index < in_file_table_ptr->row_count;
        ++file_index) {

        //theres no file handle, its available and we're done
        if (!in_file_table_ptr->columns.win32_handle[file_index]) {
            out_file_ref = in_file_table_ptr->columns.file[file_index];
            return(true);
        }
    }

    //we didn't find an available index, we're done
    return(false);
}

r_internal const RWin32FileHandle
r_win32_internal::file_initialize(
    const HANDLE               win32_file_handle,
    const RWin32FilePermission file_permission) {

    //get the file manager
    RWin32FileManager& file_manager_ref = r_win32_internal::context_get_file_manager();

    //get the next available file table
    RWin32FileTable* file_table_ptr = r_win32_internal::file_table_next_available(file_manager_ref);

    //if there isn't an existing table, commit a new one
    if (!file_table_ptr) {
        file_table_ptr = r_win32_internal::file_table_commit(file_manager_ref);

        //if we still don't have a table, we're done
        if (!file_table_ptr) {
            return(NULL);
        }
    }

    //get the index of the available file
    RWin32File file;
    if (!r_win32_internal::file_next_available(file_table_ptr,file)) {
        //this should never fail, but still...we're done
        return(NULL);
    }

    //initialize the file at the index
    file_table_ptr->columns.win32_handle[file.file_index] = win32_file_handle;
    file_table_ptr->columns.size        [file.file_index] = GetFileSize(win32_file_handle, NULL);
    file_table_ptr->columns.permission  [file.file_index] = RWin32FilePermission_ReadWrite;

    //the address of the file at the index is our handle
    RWin32FileHandle r_file_handle = (RWin32FileHandle)&file_table_ptr->columns.file[file.file_index];

    //were done
    return(r_file_handle);
}