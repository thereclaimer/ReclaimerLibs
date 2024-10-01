#pragma once

#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const r_size
r_win32::memory_align_to_page(
    const r_size size) {

    const r_size page_size    = r_win32::system_page_size(); 
    const r_size size_aligned = r_align_a_to_b(size,page_size); 

    return(size_aligned);
}

r_external const r_size
r_win32::memory_align_to_large_page(
    const r_size size) {

    const r_size large_page_size = r_win32::system_page_size_large(); 
    const r_size size_aligned    = r_align_a_to_b(size,large_page_size); 

    return(size_aligned);
}

r_external const r_size
r_win32::memory_align_to_allocation_granularity(
    const r_size size) {

    const r_size granularity  = r_win32::system_allocation_granularity();
    const r_size size_aligned = r_align_a_to_b(size,granularity); 

    return(size_aligned);
}

r_external r_memory
r_win32::memory_reserve(
    const r_size size) {

    const r_size size_aligned = r_win32::memory_align_to_page(size);

    r_memory reservation = 
        (r_memory)VirtualAlloc(
            NULL,
            size_aligned,
            MEM_RESERVE,
            PAGE_NOACCESS);
    
    return(reservation);
}

r_external const r_b8
r_win32::memory_release(
    const r_memory start,
    const r_size   size) {

    const r_size size_aligned = r_win32::memory_align_to_page(size);

    const r_b8 result = VirtualFree(
        start,
        size_aligned,
        MEM_RELEASE);

    return(result);
}

r_external r_memory
r_win32::memory_commit(
    const r_memory start, 
    const r_size   size) {

    const r_size size_aligned = r_win32::memory_align_to_page(size);

    r_memory commit = 
        (r_memory)VirtualAlloc(
            start,
            size_aligned,
            MEM_COMMIT,
            PAGE_READWRITE);

    return(commit);
}

r_external const r_b8
r_win32::memory_decommit(
    const r_memory start,
    const r_size   size) {

    const r_size size_aligned = r_win32::memory_align_to_page(size);

    const r_b8 result = VirtualFree(
        start,
        size_aligned,
        MEM_DECOMMIT);

    return(result);
}

r_external r_memory
r_win32::memory_reserve_and_commit(
    const r_size size) {

    const r_size size_aligned = r_win32::memory_align_to_page(size);

    r_memory commit = 
        (r_memory)VirtualAlloc(
            NULL,
            size_aligned,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE);

    return(commit);
}

r_external r_memory
r_win32::memory_reserve_and_commit_large_pages(
    const r_size size) {

    const r_size size_aligned = r_win32::memory_align_to_large_page(size);

    r_memory commit = 
        (r_memory)VirtualAlloc(
            NULL,
            size_aligned,
            MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES,
            PAGE_READWRITE);

    return(commit);
}