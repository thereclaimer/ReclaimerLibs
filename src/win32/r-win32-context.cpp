#pragma once

#include "r-win32-internal-context.hpp"

r_external const r_b8
r_win32::context_create(
          RWin32MainArgs& r_win32_args,
    const r_size          file_count_max,
    const r_size          stack_size) {

    r_b8 result = true;

    r_win32_internal::system_info_get();

    return(result);
}

r_external const r_b8 
r_win32::context_destroy(
    r_void) {

    r_b8 result = true;

    return(result);
}

r_external const r_b8 
r_win32::context_set_memory_region(
    const RHNDMemoryRegion region_handle) {

    _r_win32_context.region = region_handle;

    return(region_handle ? true : false);
}