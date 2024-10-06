#pragma once

#include "r-win32-internal-file.hpp"
#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const r_b8 file_exists (const r_cstr file_path);

r_external const RWin32FileHandle file_open_existing (const RWin32FilePermission file_permission, const r_cstr file_path);
r_external const RWin32FileHandle file_create_new    (const r_cstr file_path);

r_external const r_b8   file_close     (const RWin32FileHandle file_handle);
r_external const r_b8   file_is_open   (const RWin32FileHandle file_handle);
r_external const r_b8   file_can_write (const RWin32FileHandle file_handle);
r_external const r_size file_size      (const RWin32FileHandle file_handle);
r_external const r_cstr file_path      (const RWin32FileHandle file_handle);

r_external const r_b8
file_read(
    const RWin32FileHandle in_file_handle,
    const r_size           in_file_read_start,
    const r_size           in_file_read_length,
            r_memory        out_file_read_buffer);

r_external const r_b8
file_write(
    const RWin32FileHandle in_file_handle,
    const r_size           in_file_write_start,
    const r_size           in_file_write_length,
            r_memory        out_file_write_buffer);

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal const r_size 
r_win32::file_table_count_open_files(
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
r_win32::file_table_can_add_file(
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
r_win32::file_table_next_available(
    RWin32FileManager& file_manager_ref) {

    RWin32FileTable* next_available_table_ptr = NULL;
    for (
        RWin32FileTable* file_table_ptr = file_manager_ref.file_table_list;
        file_table_ptr != NULL;
        file_table_ptr = file_table_ptr->next) {

        if (r_win32::file_table_can_add_file(file_table_ptr)) {
            next_available_table_ptr = file_table_ptr;
            break;            
        }
    }

    return(next_available_table_ptr);
}

r_internal RWin32FileTable* 
r_win32::file_table_commit(
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

    //add the file table to the list
    file_table_ptr->next = file_manager_ref.file_table_list; 
    if (file_manager_ref.file_table_list) {
        file_manager_ref.file_table_list->previous = file_table_ptr;
    }
    file_manager_ref.file_table_list = file_table_ptr;

    //we're done
    return(file_table_ptr);
}