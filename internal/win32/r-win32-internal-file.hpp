#ifndef R_WIN32_INTERNAL_FILE_HPP
#define R_WIN32_INTERNAL_FILE_HPP

#include "r-win32.hpp"

struct RWin32File {
    r_address file_table_offset;
    r_index   file_index;
};

struct RWin32FileTable {
    RHNDMemoryArena  arena_handle;
    RWin32FileTable* next;
    RWin32FileTable* previous;
    r_size           row_count;
    struct {
        RWin32File*           file;
        HANDLE*               win32_handle;
        r_size*               size;
        RWin32FilePermission* permission;
    } columns;
};

struct RWin32FileManager {
    RWin32FileTable* file_table_list;
};

namespace r_win32_internal {

    r_internal r_void CALLBACK
    file_io_completion_routine(
        DWORD        error_code,
        DWORD        bytes_transferred,
        LPOVERLAPPED overlapped_ptr);

    r_internal const r_size file_table_count_open_files(RWin32FileTable* file_table_ptr);
    r_internal const r_b8   file_table_can_add_file    (RWin32FileTable* file_table_ptr);

    r_internal RWin32FileTable* file_table_next_available (RWin32FileManager& file_manager_ref);
    r_internal RWin32FileTable* file_table_commit         (RWin32FileManager& file_manager_ref);
    r_internal const r_b8       file_table_decommit       (RWin32FileManager& file_manager_ref, RWin32FileTable* file_table_ptr);

    r_internal const r_b8 
    file_next_available(
        RWin32FileTable* in_file_table_ptr,
        RWin32File&     out_file_ref);

    r_internal const RWin32FileHandle
    file_initialize(
        const HANDLE               win32_file_handle,
        const RWin32FilePermission file_permission);

    inline RWin32FileTable*
    file_table_from_file(RWin32File* file_ptr) {

        if (!file_ptr) {
            return(NULL);
        }

        //get the file table memory
        const r_memory file_memory       = (r_memory)file_ptr;
        const r_memory file_table_memory = file_memory - file_ptr->file_table_offset;
        
        //cast it to a pointer
        RWin32FileTable* file_table_ptr = (RWin32FileTable*)file_table_memory; 

        //we're done
        return(file_table_ptr);
    }

    inline const HANDLE
    file_table_read_win32_handle(
        RWin32File* file_ptr) {

        //get the file table
        RWin32FileTable* file_table_ptr = r_win32_internal::file_table_from_file(file_ptr); 

        //get the handle
        const HANDLE win32_handle = file_table_ptr->columns.win32_handle[file_ptr->file_index];

        //we're done
        return(win32_handle);
    }

    inline const RWin32FilePermission
    file_table_read_permission(
        RWin32File* file_ptr) {

        //get the file table
        RWin32FileTable* file_table_ptr = r_win32_internal::file_table_from_file(file_ptr); 

        //get the permission
        const RWin32FilePermission file_permission = 
            file_table_ptr->columns.permission[file_ptr->file_index];

        //we're done
        return(file_permission);
    }
    
    inline const r_size
    file_table_read_size(
        RWin32File* file_ptr) {

        //get the file table
        RWin32FileTable* file_table_ptr = r_win32_internal::file_table_from_file(file_ptr); 

        //get the size
        const r_size file_size = 
            file_table_ptr->columns.size[file_ptr->file_index];

        //we're done
        return(file_size);
    }

};

#endif //R_WIN32_INTERNAL_FILE_HPP