#pragma once

#include "r-win32-string.hpp"

r_external const r_wstr 
r_win32::string_cstr_to_wstr(
    const RMemoryArenaHandle arena_handle,
    const r_size             char_count_max,
    const r_cstr             c_str) {

    //sanity check
    if (
        arena_handle   == NULL ||
        char_count_max == 0    ||
        c_str          == NULL) {

        return(NULL);
    }

    //get the length of the input c-string, including null terminator
    const r_size str_char_count = strnlen_s(c_str,char_count_max) + 1;

    //determine the byte length of the equivalient w-string
    const r_size w_char_size      = sizeof(r_wchar);
    const r_size w_str_size_bytes = w_char_size * str_char_count;

    //get the memory
    const r_wstr w_str = r_mem_arena_push_array(arena_handle,w_str_size_bytes,r_wchar);

    //if that failed, we're done
    if (!w_str) {
        return(NULL);
    }

    //clear the memory
    memset(w_str,0,w_str_size_bytes);        

    //convert the c-string to w-string
    MultiByteToWideChar(CP_UTF8, 0, c_str, str_char_count, w_str, str_char_count);

    //we're done
    return(w_str);
}

r_external const r_cstr 
r_win32::string_wstr_to_cstr(
    const RMemoryArenaHandle arena_handle,
    const r_size             char_count_max,
    const r_wstr             w_str) {
    
    //sanity check
    if (
        arena_handle   == NULL ||
        char_count_max == 0    ||
        w_str          == NULL) {

        return(NULL);
    }

    //get the character length of the w-string
    const r_size str_char_count = wcsnlen_s(w_str,char_count_max);

    //get the memory
    const r_cstr c_str = r_mem_arena_push_array(arena_handle,str_char_count,r_char);

    //clear the memory
    memset(c_str,0,str_char_count);

    //convert the w-string to a c-string 
    WideCharToMultiByte(
        CP_UTF8,
        0, 
        w_str,
        str_char_count,
        c_str,
        str_char_count,
        NULL,
        NULL);

    //we're done
    return(c_str);
}