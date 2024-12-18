#ifndef R_WIN32_CONTEXT_HPP
#define R_WIN32_CONTEXT_HPP

#include <r-common.hpp>
#include <r-memory.hpp>

#include "r-win32-main.hpp"

namespace r_win32 {

    r_external const r_b8 
    context_create(RWin32MainArgs& r_win32_args);

    r_external const r_b8 context_destroy (r_void);

    r_external const r_b8 context_set_memory_region(const RMemoryRegionHandle region_handle);
};


#endif //R_WIN32_CONTEXT_HPP