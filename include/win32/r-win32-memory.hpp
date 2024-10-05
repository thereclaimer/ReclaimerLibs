#ifndef R_WIN32_MEMORY_HPP
#define R_WIN32_MEMORY_HPP

#include <r-common.hpp>
#include <Windows.h>

namespace r_win32 {

    r_external const r_size memory_align_to_page                   (const r_size size);
    r_external const r_size memory_align_to_large_page             (const r_size size);
    r_external const r_size memory_align_to_allocation_granularity (const r_size size);

    r_external r_memory memory_reserve  (const r_size   size);
    r_external r_memory memory_commit   (const r_memory start, const r_size size);
    
    r_external const r_b8 memory_decommit (const r_memory start, const r_size size);
    r_external const r_b8 memory_release  (const r_memory start, const r_size size);

    r_external r_memory memory_reserve_and_commit             (const r_size size);
    r_external r_memory memory_reserve_and_commit_large_pages (const r_size size);
};

#endif //R_WIN32_MEMORY_HPP