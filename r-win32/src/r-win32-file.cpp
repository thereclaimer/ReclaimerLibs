#pragma once

#include "r-win32-internal-file.hpp"
#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

const r_b8 
r_win32::file_exists(
    const r_cstr file_path) {
    
}

const RWin32FileIndex
r_win32::file_open_existing(
    const RWin32FilePermission file_permission, 
    const r_cstr               file_path) {
    
    r_index file_index;
    if (!r_win32_internal::file_table_next_availabe(file_index)) {
        return(false);
    }


    
}

const RWin32FileIndex
r_win32::file_create_new(
    const r_cstr file_path) {

}

const r_b8
r_win32::file_close(
    const RWin32FileIndex file_handle) {

}

const r_b8
r_win32::file_is_open(
    const RWin32FileIndex file_handle) {
    



}

const r_b8
r_win32::file_can_write(
    const RWin32FileIndex file_handle) {

}

const r_size
r_win32::file_size(
    const RWin32FileIndex file_handle) {

}

const r_cstr
r_win32::file_path(
    const RWin32FileIndex file_handle) {

}

const r_b8
r_win32::file_read(
    const RWin32FileIndex file_handle,
    const r_size           read_start,
    const r_size           read_length,
          r_memory         read_buffer) {

}

const r_b8
r_win32::file_write(
    const RWin32FileIndex file_handle,
    const r_size           write_start,
    const r_size           write_length,
          r_memory         write_buffer) {

}


/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal  const r_b8 
r_win32::file_table_create(
    const r_size file_count) {

    RWin32FileTable& file_table = r_win32_internal::context_get_file_table();

    //calculate sizes
    const r_size column_size_win32_handle = file_count * sizeof(HANDLE);
    const r_size column_size_size         = file_count * sizeof(r_size);
    const r_size column_size_permission   = file_count * sizeof(RWin32FilePermission);

    //allocate memory
    const r_memory column_memory_win32_handle = r_win32_internal::context_stack_push(column_size_win32_handle);
    const r_memory column_memory_size         = r_win32_internal::context_stack_push(column_size_size);
    const r_memory column_memory_permission   = r_win32_internal::context_stack_push(column_size_permission);

    //sanity check
    if (
        !column_memory_win32_handle ||
        !column_memory_size         ||
        !column_memory_permission) {

        return(false);
    }

    //initialize the table
    file_table.columns.win32_handle = (HANDLE*)column_memory_win32_handle;
    file_table.columns.size         = (r_size*)column_memory_size;
    file_table.columns.permission   = (RWin32FilePermission*)column_memory_permission;

    for (
        r_index file_index = 0;
        file_index < file_count;
        ++file_index) {

        file_table.columns.win32_handle[file_index] = NULL;
        file_table.columns.size        [file_index] = 0;
        file_table.columns.permission  [file_index] = 0;
    }

    return(false);
}

r_internal const r_b8 
r_win32_internal::file_table_next_availabe(
    r_index& file_index) {

    RWin32FileTable& file_table = r_win32_internal::context_get_file_table();

    for (
        file_index = 0;
        file_index < file_count;
        ++file_index) {

        if (!file_table.columns.win32_handle[file_index]) {
            return(true);
        }
    }

    return(false);
}
