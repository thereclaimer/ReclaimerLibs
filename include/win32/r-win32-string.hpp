#ifndef R_WIN32_STRING_HPP
#define R_WIN32_STRING_HPP

#include <Windows.h>

#include "r-common.hpp"
#include "r-memory.hpp"

namespace r_win32 {

    r_external const r_wstr 
    string_cstr_to_wstr(
        const RMemoryArenaHandle arena_handle,
        const r_size             char_count_max,
        const r_cstr             c_str);

    r_external const r_cstr 
    string_wstr_to_cstr(
        const RMemoryArenaHandle arena_handle,
        const r_size             char_count_max,
        const r_wstr             w_str);
};


#endif //R_WIN32_STRING_HPP