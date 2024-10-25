#ifndef R_WIN32_FILE_HPP
#define R_WIN32_FILE_HPP

#include <Windows.h>
#include <r-common.hpp>

typedef r_handle RWin32FileHandle;

enum RWin32FilePermission_ {
    RWin32FilePermission_ReadOnly  = 0,
    RWin32FilePermission_ReadWrite = 1
};

typedef r_u32 RWin32FilePermission;

namespace r_win32 {

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
};

#endif //R_WIN32_FILE_HPP