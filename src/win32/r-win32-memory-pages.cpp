#pragma once

#include "r-win32-memory.hpp"
#include "r-win32-system.hpp"

r_external const r_b8
r_win32::memory_pages_reserve(
    RWin32MemoryPageReservation& page_reservation_ref) {

    //sanity check
    if (page_reservation_ref.size_requested == 0) {
        return(false);
    }

    //calculate the actual size of the reservation
    const r_size reservation_size_actual = r_win32::memory_align_to_allocation_granularity(page_reservation_ref.size_requested);

    //make the reservation, if it fails we're done
    r_memory reservation = r_win32::memory_reserve(NULL,page_reservation_ref.size_actual);
    if (!reservation) {
        return(false);
    }

    //calculate the other sizes
    const r_size page_size  = r_win32::system_page_size();
    const r_size page_count = reservation_size_actual / page_size;

    //update the reservation reference
    page_reservation_ref.start               = (r_address)reservation;
    page_reservation_ref.size_actual         = reservation_size_actual;
    page_reservation_ref.page_size           = page_size;
    page_reservation_ref.page_count_total    = page_count;
    page_reservation_ref.page_count_used     = 0;

    //we're done
    return(true);
}

r_external const r_b8 
r_win32::memory_pages_release(
    RWin32MemoryPageReservation& page_reservation_ref) {

    //release the memory
    const r_b8 result = r_win32::memory_release(
        (r_memory)page_reservation_ref.start,
        page_reservation_ref.size_actual);

    //we're done
    return(result);
}

r_external const r_b8
r_win32::memory_page_table_create(
    RWin32MemoryPageReservation& in_page_reservation,
    RWin32MemoryPageTable&      out_page_table) {

    //calculate the commit size
    const r_size page_size               = in_page_reservation.page_size;
    const r_size page_block_size_actual = r_win32::memory_align_to_page(out_page_table.page_block_size_requested);
    const r_size page_table_commit_size = page_block_size_actual * out_page_table.page_block_count;
    const r_size page_table_page_count  = page_table_commit_size / page_size;  

    //make sure we can fit the page table in the reservation
    const r_size page_count_new = page_table_page_count + in_page_reservation.page_count_used;
    if (page_count_new > in_page_reservation.page_count_total) {
        return(false);
    }

    //calculate the starting address
    const r_address page_reservation_start  = in_page_reservation.start; 
    const r_size    page_count_used         = in_page_reservation.page_count_used;
    const r_size    page_table_offset       = page_size * page_count_used;
    const r_address page_table_commit_start = page_reservation_start + page_table_offset;  

    //initialize the page table
    out_page_table.page_start                = page_table_commit_start;
    out_page_table.page_block_size_actual    = page_block_size_actual;
    out_page_table.page_size                 = page_size;
    out_page_table.page_count                = page_table_page_count;
    out_page_table.page_table_size_total     = page_table_commit_size;

    //update the reservation
    in_page_reservation.page_count_used = out_page_table.page_count;

    //we're done
    return(true);
}
    
r_external const r_b8
r_win32::memory_page_block_commit(
    RWin32MemoryPageTable&  in_page_table_ref,
    RWin32MemoryPageBlock& out_page_block_ref) {

    //calculate sizes and offsets
    const r_size    page_size                = out_page_block_ref.page_size; 
    const r_index   page_block_index         = out_page_block_ref.page_block_index;
    const r_size    page_block_memory_size   = in_page_table_ref.page_block_size_actual;
    const r_size    page_block_offset        = page_block_memory_size * page_block_index; 
    const r_address page_table_start         = in_page_table_ref.page_start;
    const r_memory  page_block_memory_commit = (r_memory)(page_table_start + page_block_offset);

    //commit the page table memory
    const r_memory page_block_memory_commit_start = 
        r_win32::memory_commit(
            page_block_memory_commit,
            page_block_memory_size);

    //if that failed, we're done
    if (!page_block_memory_commit_start) {
        return(false);
    }

    //initialize the page block
    out_page_block_ref.page_table_start = page_table_start;
    out_page_block_ref.page_block_start = (r_address)page_block_memory_commit_start;
    out_page_block_ref.page_block_size  = page_block_memory_size; 
    out_page_block_ref.page_size        = page_size;
    out_page_block_ref.page_count       = page_block_memory_size / page_size;

    //we're done
    return(true);
}

r_external const r_b8     
r_win32::memory_page_block_decommit(
    RWin32MemoryPageBlock& page_block_ref) {

    const r_memory page_block_commit_memory = (r_memory)page_block_ref.page_block_start;
    const r_size   page_block_commit_size   = page_block_ref.page_size * page_block_ref.page_count;

    r_b8 result = r_win32::memory_decommit(
        page_block_commit_memory,
        page_block_commit_size);

    page_block_ref.page_table_start = 0;
    page_block_ref.page_block_start = 0;
    page_block_ref.page_block_size  = 0;
    page_block_ref.page_size        = 0;
    page_block_ref.page_count       = 0;

    return(result);
}

r_external const r_memory 
r_win32::memory_page_block_start(
    RWin32MemoryPageBlock& page_block_ref) {

    return((r_memory)page_block_ref.page_block_start);
}