#ifndef R_WIN32_FILE_HPP
#define R_WIN32_FILE_HPP

#include <Windows.h>
#include <r-common.hpp>

typedef r_index RWin32FileIndex;

enum RWin32FilePermission_ {
    RWin32FilePermission_ReadOnly  = 0,
    RWin32FilePermission_ReadWrite = 1
};

typedef r_u32 RWin32FilePermission;

namespace r_win32 {

    const r_b8 file_exists (const r_cstr file_path);
    
    const RWin32FileIndex file_open_existing (const RWin32FilePermission file_permission, const r_cstr file_path);
    const RWin32FileIndex file_create_new    (const r_cstr file_path);
    
    const r_b8   file_close     (const RWin32FileIndex file_index);
    const r_b8   file_is_open   (const RWin32FileIndex file_index);
    const r_b8   file_can_write (const RWin32FileIndex file_index);
    const r_size file_size      (const RWin32FileIndex file_index);
    const r_cstr file_path      (const RWin32FileIndex file_index);

    const r_b8
    file_read(
        const RWin32FileIndex file_index,
        const r_size          read_start,
        const r_size          read_length,
              r_memory        read_buffer);

    const r_b8
    file_write(
        const RWin32FileIndex file_handle,
        const r_size          write_start,
        const r_size          write_length,
              r_memory        write_buffer);
};

#endif //R_WIN32_FILE_HPP