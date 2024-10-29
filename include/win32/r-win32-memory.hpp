#ifndef R_WIN32_MEMORY_HPP
#define R_WIN32_MEMORY_HPP

#include <r-common.hpp>
#include <Windows.h>

/**********************************************************************************/
/* MEMORY ALLCOATION / DEALLOCATION                                               */
/**********************************************************************************/

namespace r_win32 {

    r_external const r_size memory_align_to_page                   (const r_size size);
    r_external const r_size memory_align_to_large_page             (const r_size size);
    r_external const r_size memory_align_to_allocation_granularity (const r_size size);
    
    r_external r_memory memory_reserve                        (const r_memory start, const r_size size);
    r_external r_memory memory_commit                         (const r_memory start, const r_size size);
    r_external r_memory memory_reserve_and_commit             (const r_memory start, const r_size size);
    r_external r_memory memory_reserve_and_commit_large_pages (const r_memory start, const r_size size);
    
    r_external const r_b8 memory_decommit (const r_memory start, const r_size size);
    r_external const r_b8 memory_release  (const r_memory start, const r_size size);
};

/**********************************************************************************/
/* MEMORY PAGES                                                                   */
/**********************************************************************************/

struct RWin32MemoryPageReservation {
    r_address start;
    r_size    size_requested;
    r_size    size_actual;
    r_size    page_size;
    r_size    page_count_total;
    r_size    page_count_used;
};

namespace r_win32 {

    r_external const r_b8 memory_pages_reserve(RWin32MemoryPageReservation& page_reservation_ref);
    r_external const r_b8 memory_pages_release(RWin32MemoryPageReservation& page_reservation_ref);
};

struct RWin32MemoryPageTable {
    r_address page_start;
    r_size    page_block_size_requested;
    r_size    page_block_size_actual;
    r_size    page_block_count;
    r_size    page_size;
    r_size    page_count;
    r_size    page_table_size_total;
};

namespace r_win32 {

    r_external const r_b8 
    memory_page_table_create(
        RWin32MemoryPageReservation& in_page_reservation,
        RWin32MemoryPageTable&      out_page_table);
};

struct RWin32MemoryPageBlock {
    r_address page_table_start;
    r_address page_block_start;
    r_address page_block_size;
    r_index   page_block_index;
    r_size    page_size;
    r_size    page_count;
};

namespace r_win32 {

    r_external const r_b8
    memory_page_block_commit(
        RWin32MemoryPageTable&  in_page_table_ref,
        RWin32MemoryPageBlock& out_page_block_ref);

    r_external const r_b8     memory_page_block_decommit (RWin32MemoryPageBlock& page_block_ref);
    r_external const r_memory memory_page_block_start    (RWin32MemoryPageBlock& page_block_ref);
};

#endif //R_WIN32_MEMORY_HPP