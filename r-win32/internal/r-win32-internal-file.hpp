#ifndef R_WIN32_INTERNAL_FILE_HPP
#define R_WIN32_INTERNAL_FILE_HPP

#include "r-win32.hpp"

struct RWin32File {
    r_index              index;
    HANDLE               win32_handle;
    r_size               size;
    RWin32FilePermission permission;
};

struct RWin32FileTable {
    r_size row_count;
    struct {
        HANDLE*               win32_handle;
        r_size*               size;
        RWin32FilePermission* permission;
    } columns;
};

namespace r_win32_internal {

    r_internal const r_b8 file_table_create(const r_size file_count);
    r_internal const r_b8 file_table_next_availabe(r_index& file_index);
};

#endif //R_WIN32_INTERNAL_FILE_HPP