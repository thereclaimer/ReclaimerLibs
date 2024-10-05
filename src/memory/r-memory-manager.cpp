#pragma once

#include "r-memory-internal.hpp"
#include "r-win32.hpp"

r_external const r_b8
r_mem::memory_manager_create(
    const RPlatformMemoryApi& platform_memory_api,
    const r_size              maximum_reserved_size,
    const r_size              reservation_alignment,
    const r_size              arena_alignment,
    const r_size              internal_stack_size) {

    //make sure the platform api is valid
    const r_b8 api_valid = 
        platform_memory_api.reserve            &&
        platform_memory_api.commit             &&
        platform_memory_api.reserve_and_commit &&
        platform_memory_api.decommit           &&
        platform_memory_api.release;
    
    if (!api_valid) {
        return(false);
    }

    //sanity check on our sizes
    const r_b8 sizes_valid =
        maximum_reserved_size    > 0                        &&
        reservation_alignment    > 0                        &&
        arena_alignment          > 0                        &&
        internal_stack_size      > 0                        &&
        arena_alignment          < reservation_alignment    &&
        reservation_alignment    < maximum_reserved_size;

    if (!sizes_valid) {
        return(false);
    }

    _r_memory_manager.reservation_list      = {0};
    _r_memory_manager.platform_memory_api   = platform_memory_api;
    _r_memory_manager.maximum_reserved_size = maximum_reserved_size;
    _r_memory_manager.reservation_alignment = reservation_alignment;
    _r_memory_manager.arena_alignment       = arena_alignment;

    //create the stack
    const r_b8 stack_created = r_mem_internal::stack_create(internal_stack_size);
    if (!stack_created) {
        return(false);
    }

    //if we got through all of that, we're good to go
    return(true);
}

r_external const r_b8
r_mem::memory_manager_create_win32(
    const r_size maximum_reserved_size,
    const r_size internal_stack_size) {

    //use the win32 memory functions
    RPlatformMemoryApi& win32_memory_api_ref = _r_memory_manager.platform_memory_api;
    win32_memory_api_ref.reserve            = (r_funcptr_platform_memory_reserve)r_win32::memory_reserve;
    win32_memory_api_ref.commit             = (r_funcptr_platform_memory_commit)r_win32::memory_commit;
    win32_memory_api_ref.reserve_and_commit = (r_funcptr_platform_memory_reserve_and_commit)r_win32::memory_reserve_and_commit;
    win32_memory_api_ref.decommit           = (r_funcptr_platform_memory_decommit)r_win32::memory_decommit;
    win32_memory_api_ref.release            = (r_funcptr_platform_memory_release)r_win32::memory_release;

    //use the win32 system info for alignment
    const r_size win32_reservation_alignment = r_win32::system_allocation_granularity();
    const r_size win32_arena_alignment       = r_win32::system_page_size();

    //create the memory manager
    const r_b8 result = r_mem::memory_manager_create(
        win32_memory_api_ref,
        maximum_reserved_size,
        win32_reservation_alignment,
        win32_arena_alignment,
        internal_stack_size);

    return(result);
}

r_external const r_b8
r_mem::memory_manager_destroy(r_void) {

    r_b8 success = true;

    success &= r_mem_internal::reservation_list_release_all();

    return(success);
}

r_external const r_size
r_mem::memory_manager_maximum_reserved_size(r_void) {

    return(_r_memory_manager.maximum_reserved_size);
}

r_external const r_size
r_mem::memory_manager_reservation_alignment(r_void) {

    return(_r_memory_manager.reservation_alignment);
}

r_external const r_size
r_mem::memory_manager_arena_alignment(r_void) {

    return(_r_memory_manager.arena_alignment);
}

r_external const r_size
r_mem::memory_manager_current_reserved_size(r_void) {
    return(0);
}

r_external const r_size
r_mem::memory_manager_internal_stack_size_total(r_void) {
    return(0);
}

r_external const r_size
r_mem::memory_manager_internal_stack_size_used(r_void) {
    return(0);

}

r_external const r_size
r_mem::memory_manager_internal_stack_size_free(r_void) {
    return(0);
}

r_external const r_size 
r_mem::memory_manager_align_to_reservation(
    const r_size size) {
    
    const r_size size_aligned = 
     r_align_a_to_b(
        _r_memory_manager.reservation_alignment,
        size);

    return(size_aligned);
}

r_external const r_size 
r_mem::memory_manager_align_to_arena(
    const r_size size) {

    const r_size size_aligned = 
     r_align_a_to_b(
        _r_memory_manager.arena_alignment,
        size);

    return(size_aligned);
}
