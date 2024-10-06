#ifndef R_WIN32_INTERNAL_FILE_HPP
#define R_WIN32_INTERNAL_FILE_HPP

#include "r-win32.hpp"

struct RWin32File {
    r_index table_index;
    r_index file_index;
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

    r_internal const r_size file_table_count_open_files(RWin32FileTable* file_table_ptr);
    r_internal const r_b8   file_table_can_add_file    (RWin32FileTable* file_table_ptr);

    r_internal RWin32FileTable* file_table_next_available (RWin32FileManager& file_manager_ref);
    r_internal RWin32FileTable* file_table_commit         (RWin32FileManager& file_manager_ref);

};

#endif //R_WIN32_INTERNAL_FILE_HPP