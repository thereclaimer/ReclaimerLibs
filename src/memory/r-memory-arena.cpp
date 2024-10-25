#pragma once

#include "r-memory-internal.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const RMemoryArenaHandle 
r_mem::arena_at_index(
    const RMemoryRegionHandle region_handle,
    const r_index          arena_index) {

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle); 

    //sanity check
    if (!region_ptr ||
        arena_index >= region_ptr->arena_count) {

        return(NULL);
    }

    //get the arena at the index
    return(&region_ptr->arenas[arena_index]);
}

r_external const RMemoryArenaHandle
r_mem::arena_commit(
    const RMemoryRegionHandle region_handle) {

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle); 

    //find the next uncommitted arena
    r_index arena_index = 0;
    const r_b8 arena_found = 
        r_mem_internal::arena_next_uncommitted(
            region_ptr,
            0,
            &arena_index);

    //if we didn't find an arena, we're done
    if (!arena_found) {
        return(NULL);
    }

    //commit the arena
    RMemoryArena* committed_arena =
        r_mem_internal::arena_commit_immediate_index(
            region_ptr,
            arena_index);

    //return the comitted arena
    return(committed_arena);
}

r_external const RMemoryArenaHandle
r_mem::arena_commit_next(
    const RMemoryArenaHandle arena_handle) {

    //get the arena and region
    RMemoryArena*  arena_ptr  = r_mem_internal::arena_from_handle(arena_handle);
    RMemoryRegion* region_ptr = r_mem_internal::region_from_arena(arena_ptr);

    //sanity check    
    if (!arena_ptr || !region_ptr) {
        return(NULL);
    }

    //make sure that there is an arena adjacent to this one
    if (arena_ptr->index == region_ptr->arena_count - 1) {
        return(NULL);
    }

    //get the next arena
    RMemoryArena* next_arena = arena_ptr + 1;

    //make sure its not already committed
    if (next_arena->commit) {
        return(NULL);
    }

    //commit the arena
    const r_b8 commit_result = 
        r_mem_internal::arena_commit_immediate(
            region_ptr,
            next_arena);

    //return the comitted arena
    return(commit_result ? next_arena : NULL);
}

r_external const RMemoryArenaHandle
r_mem::arena_commit_index(
    const RMemoryRegionHandle region_handle,
    const r_index          arena_index) {

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);
    if (!region_ptr) {
        return(NULL);
    }

    //commit the arena
    RMemoryArena* arena_ptr = 
        r_mem_internal::arena_commit_immediate_index(
            region_ptr,
            arena_index);

    return(arena_ptr);
}

r_external const r_b8
r_mem::arena_decommit(
    const RMemoryArenaHandle arena_handle) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_arena(arena_ptr);
    if (!region_ptr) {
        return(false);
    }

    //attempt to decommit the arena
    const r_b8 decommit_result = 
        r_mem_internal::arena_decommit_immediate(
            region_ptr,
            arena_ptr);

    //return the result
    return(decommit_result);
}

r_external const r_b8
r_mem::arena_is_committed(
    const RMemoryArenaHandle arena_handle) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }
    
    //return the commit status
    return(arena_ptr->commit != NULL ? true : false);
}

r_external const r_b8
r_mem::arena_at_index_is_committed(
    const RMemoryRegionHandle region_handle,
    const r_index          arena_index) {

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);
    if (!region_ptr) {
        return(NULL);
    }

    //get the arena
    RMemoryArena* arena_ptr =  (RMemoryArena*)r_mem::arena_at_index(region_ptr,arena_index);
    if (!arena_ptr) {
        return(NULL);
    }

    //return the commit status
    return(arena_ptr->commit != NULL ? true : false);
}

r_external const r_size
r_mem::arena_size_total(
    const RMemoryArenaHandle arena_handle) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);  
    if (!arena_ptr) {
        return(NULL);
    }

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_arena(arena_ptr);
    if (!region_ptr) {
        return(NULL);
    }

    //return the arena size
    return(region_ptr->arena_size);
}

r_external const r_size
r_mem::arena_size_used(
    const RMemoryArenaHandle arena_handle) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);  
    if (!arena_ptr) {
        return(NULL);
    }

    //the position is the used size
    return(arena_ptr->position);
}

r_external const r_size
r_mem::arena_size_free(
    const RMemoryArenaHandle arena_handle) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);  
    if (!arena_ptr) {
        return(NULL);
    }

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_arena(arena_ptr);
    if (!region_ptr) {
        return(NULL);
    }

    //calculate the free size
    const r_size free_size = region_ptr->arena_size - arena_ptr->position;
    return(free_size);
}

r_external const r_memory
r_mem::arena_push(
    const RMemoryArenaHandle arena_handle,
    const r_size          size) {

    //first make sure we can push
    const r_b8 can_push = r_mem::arena_can_push(arena_handle, size);
    if (!can_push) {
        return(NULL);
    }

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }

    //get the memory and clear it
    r_memory arena_memory = arena_ptr->commit + arena_ptr->position;
    memset(arena_memory,0,size);    

    //update the position
    arena_ptr->position += size;

    //return the arena memory
    return(arena_memory);
}

r_external const r_memory
r_mem::arena_pull(
    const RMemoryArenaHandle arena_handle,
    const r_size          size) {

    //first make sure we can pull
    const r_b8 can_pull = r_mem::arena_can_pull(arena_handle, size);
    if (!can_pull) {
        return(NULL);
    }
    
    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }

    //clear the memory
    r_memory arena_memory = arena_ptr->commit + arena_ptr->position;
    memset(arena_memory,0,size);

    //update the position
    arena_ptr->position -= size;

    //get the memory at the updated position
    arena_memory -= size;

    //return the arena memory
    return(arena_memory);
}

r_external const r_memory
r_mem::arena_push_aligned(
    const RMemoryArenaHandle arena_handle,
    const r_size          size,
    const r_size          alignment) {

    //calculate the aligned size
    const r_size size_aligned = r_align_a_to_b(size,alignment);

    //do the same base push
    r_memory arena_memory = r_mem::arena_push(arena_handle,size_aligned);

    return(arena_memory);
}

r_external const r_memory
r_mem::arena_pull_aligned(
    const RMemoryArenaHandle arena_handle,
    const r_size          size,
    const r_size          alignment) {

    //calculate the aligned size
    const r_size size_aligned = r_align_a_to_b(size,alignment);

    //do the same base pull
    r_memory arena_memory = r_mem::arena_pull(arena_handle,size_aligned);

    return(arena_memory);
}

r_external const r_cstr
r_mem::arena_push_cstr(
    const RMemoryArenaHandle arena_handle,
    const r_size             c_str_size_max,
    const r_cstr             c_str) {

    //sanity check
    if (
        arena_handle   == NULL || // valid arena handle
        c_str_size_max == 0    || // max size is set
        c_str          == NULL) { // the string is not null

        return(false);
    }

    //get the string length, plus 1 for the null terminator
    const r_size c_str_len = strnlen_s(c_str,c_str_size_max) + 1;

    //push the memory on the arena
    r_memory arena_memory = r_mem::arena_push(arena_handle,c_str_len);

    //clear the memory
    memset(arena_memory,0,c_str_len);

    //copy the string over
    memmove(arena_memory,c_str,c_str_len);

    //cast the memory to a wstr
    const r_cstr result = (r_cstr)arena_memory;

    //we're done
    return(result);
}

r_external const r_wstr
r_mem::arena_push_wstr(
    const RMemoryArenaHandle arena_handle,
    const r_size             w_str_size_max,
    const r_wstr             w_str) {

    //sanity check
    if (
        arena_handle   == NULL || // valid arena handle
        w_str_size_max == 0    || // max size is set
        w_str          == NULL) { // the string is not null

        return(NULL);
    }

    //get the string length, plus 1 for the null terminator
    const r_size w_str_len = wcsnlen_s(w_str,w_str_size_max) + 1;

    //now, we need to get the size in bytes, since this is a wide string
    const r_size w_str_len_bytes = w_str_len * sizeof(r_wstr); 

    //push the memory on the arena
    r_memory arena_memory = r_mem::arena_push(arena_handle,w_str_len_bytes);

    //clear the memory 
    memset(arena_memory,0,w_str_len_bytes);

    //copy the string over
    memmove(arena_memory,(r_void*)w_str,w_str_len_bytes);

    //cast the memory to a wstr
    const r_wstr result = (r_wstr)arena_memory;

    //we're done
    return(result);
}

r_external const r_b8
r_mem::arena_can_push(
    const RMemoryArenaHandle arena_handle,
    const r_size          size) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }

    //get the region
    RMemoryRegion* region_ptr = r_mem_internal::region_from_arena(arena_ptr);
    if (!region_ptr) {
        return(false);
    }

    //calculate the new position from this push
    const r_size new_arena_position = arena_ptr->position + size;

    //if the new position is under the arena size, we can push
    return(new_arena_position <= region_ptr->arena_size);
}

r_external const r_b8
r_mem::arena_can_pull(
    const RMemoryArenaHandle arena_handle,
    const r_size          size) {

    //get the arena
    RMemoryArena* arena_ptr = r_mem_internal::arena_from_handle(arena_handle);
    if (!arena_ptr) {
        return(false);
    }

    //if the size is less than the current position, we can pull
    return(size <= arena_ptr->position);
}

r_external const r_b8
r_mem::arena_can_push_aligned(
    const RMemoryArenaHandle arena_handle,
    const r_size          size,
    const r_size          alignment) {

    //align the size and do the same thing as above
    const r_size size_aligned = r_align_a_to_b(size,alignment);
    const r_b8 can_push = r_mem::arena_can_push(arena_handle,size_aligned);
    return(can_push);
}

r_external const r_b8
r_mem::arena_can_pull_aligned(
    const RMemoryArenaHandle arena_handle,
    const r_size          size,
    const r_size          alignment) {

    //align the size and do the same thing as above
    const r_size size_aligned = r_align_a_to_b(size,alignment);
    const r_b8 can_pull = r_mem::arena_can_pull(arena_handle,size_aligned);
    return(can_pull);    
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal const r_b8
r_mem_internal::arena_commit_immediate(
    RMemoryRegion* region_ptr,
    RMemoryArena*  arena_ptr) {

    //sanity check
    if (!region_ptr || 
        !arena_ptr  || 
        arena_ptr->index >= region_ptr->arena_count) {

        return(false);
    }

    //calculate this arena's address
    const r_size   arena_offset = region_ptr->arena_size  * arena_ptr->index; 
    const r_memory commit_start = region_ptr->arena_start + arena_offset;    

    //commit the arena memory
    r_memory arena_commit =
        r_mem_internal::platform_memory_commit(
            commit_start,
            region_ptr->arena_size);

    //if we failed, we're done
    if (!arena_commit) {
        return(false);
    }

    //initialize the arena
    arena_ptr->commit   = arena_commit;
    arena_ptr->position = 0;

    //we're done
    return(true);
}

r_internal RMemoryArena*
r_mem_internal::arena_commit_immediate_index(
            RMemoryRegion* region_ptr,
    const r_index        arena_index) {

    //sanity check
    if (!region_ptr || arena_index >= region_ptr->arena_count) {
        return(NULL);
    }

    //get the arena at the index
    RMemoryArena* arena_ptr = &region_ptr->arenas[arena_index]; 

    //attempt to commit it        
    const r_b8 result = 
        r_mem_internal::arena_commit_immediate(
            region_ptr,
            arena_ptr);

    //return the arena
    return(arena_ptr);
}

r_internal const r_b8
r_mem_internal::arena_next_uncommitted(
          RMemoryRegion* in_region_ptr,
    const r_index        in_starting_arena_index,
          r_index*       out_uncommitted_arena_index) {

    //sanity check
    if (!in_region_ptr               || 
        !out_uncommitted_arena_index || 
        in_starting_arena_index >= in_region_ptr->arena_count) {

        return(false);
    }

    //find the next uncomitted arena starting at the index
    r_b8 arena_found = false;
    for (
        RMemoryArena* current_arena = in_region_ptr->arenas + in_starting_arena_index;
        current_arena < current_arena + in_region_ptr->arena_count;
        ++current_arena) {

        
        if (!current_arena->commit) {
            arena_found = true;
            *out_uncommitted_arena_index = current_arena->index;
            break;
        }
    }

    return(arena_found);
}

r_internal const r_b8
r_mem_internal::arena_decommit_immediate(
    RMemoryRegion* region_ptr,
    RMemoryArena*  arena_ptr) {

    //sanity check
    if (!region_ptr || 
        !arena_ptr  || 
        arena_ptr->index >= region_ptr->arena_count) {

        return(false);
    }

    //calculate this arena's address
    const r_size   arena_offset   = region_ptr->arena_size  * arena_ptr->index; 
    const r_memory decommit_start = region_ptr->arena_start + arena_offset;    

    //decommit the arena memory
    r_b8 decommit_result =
        r_mem_internal::platform_memory_decommit(
            decommit_start,
            region_ptr->arena_size);

    //if we succeeded, de-initialize the arena
    if (decommit_result) {
        arena_ptr->commit   = NULL;
        arena_ptr->position = 0;
    }

    //we're done
    return(decommit_result);
}


r_internal const r_size
r_mem_internal::arena_count_committed(
    RMemoryRegion* region_ptr) {

    if (!region_ptr || region_ptr->arena_count == 0) {
        return(0);
    }

    r_size count_committed = 0;

    for (
        RMemoryArena* arena_ptr = region_ptr->arenas;
        arena_ptr < arena_ptr + region_ptr->arena_count;
        ++arena_ptr) {

        count_committed += arena_ptr->commit ? 1 : 0;
    }

    return(count_committed);
}

r_internal const r_size
r_mem_internal::arena_count_uncommitted(
    RMemoryRegion* region_ptr) {

    if (!region_ptr || region_ptr->arena_count == 0) {
        return(0);
    }

    r_size count_uncommitted = 0;

    for (
        RMemoryArena* arena_ptr = region_ptr->arenas;
        arena_ptr < arena_ptr + region_ptr->arena_count;
        ++arena_ptr) {

        count_uncommitted += arena_ptr->commit ? 0 : 1;
    }

    return(count_uncommitted);
}