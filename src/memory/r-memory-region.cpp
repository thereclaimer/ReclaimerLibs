#pragma once

#include "r-memory-internal.hpp"

r_external const RHNDMemoryRegion
r_mem::region_create(
    const RHNDMemoryReservation reservation_handle,
    const r_cstr                region_tag,
    const r_size                region_size_minimum,
    const r_size                arena_size_minimum) {

    //get the reservation
    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    if (!reservation_ptr) {
        return(NULL);
    }

    //get our aligned sizes
    const r_size arena_size_aligned  = r_mem::memory_manager_align_to_arena(arena_size_minimum);
    
    //make sure the region is aligned by the larger size
    const r_size region_size_aligned = 
        (region_size_minimum >= arena_size_minimum)
            ? r_mem::memory_manager_align_to_arena(region_size_minimum)
            : r_mem::memory_manager_align_to_arena(arena_size_minimum);

    //now, calculate the arenas, should always be at least 1
    const r_size arena_count = region_size_aligned / arena_size_aligned;
    if (arena_count == 0) {
        return(NULL);
    }

    //get a new region ptr
    RMemoryRegion* region_ptr = 
        r_mem_internal::region_initialize(
            reservation_ptr,
            region_size_aligned,
            arena_size_aligned,
            arena_count);

    //return the new region pointer
    return(region_ptr);
}

r_external const RHNDMemoryRegion
r_mem::region_create_arena_pool(
    const RHNDMemoryReservation reservation_handle,
    const r_cstr                region_tag,
    const r_size                arena_size_minimum,
    const r_size                arena_count) {

    //get the reservation
    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    if (!reservation_ptr) {
        return(NULL);
    }

    //get the aligned sizes
    const r_size arena_size_aligned  = r_mem::memory_manager_align_to_arena(arena_size_minimum);
    const r_size region_size_aligned = arena_size_aligned * arena_count; 

    //get a new region ptr
    RMemoryRegion* region_ptr = 
        r_mem_internal::region_initialize(
            reservation_ptr,
            region_size_aligned,
            arena_size_aligned,
            arena_count);

    //return the new region pointer
    return(region_ptr);
}

r_external const RHNDMemoryRegion
r_mem::region_create_struct_pool(
    const RHNDMemoryReservation reservation_handle,
    const r_cstr                region_tag,
    const r_size                arena_size_minimum,
    const r_size                struct_size,
    const r_size                struct_alignment,
    const r_size                struct_count) {

    //get the reservation
    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    if (!reservation_ptr) {
        return(NULL);
    }

    //what we're gonna do here is calculate the minimum number of arenas that will hold
    //the struct pool

    //get the aligned sizes
    const r_size arena_size_aligned = r_mem::memory_manager_align_to_arena(arena_size_minimum);
    const r_size struct_size_aligned = r_align_a_to_b(struct_size,struct_alignment);
    const r_size struct_pool_size    = struct_size_aligned * struct_count; 

    //the struct pool size is equivalient to a region size
    //so we do the same thing as the base region creation method
    const r_size region_size_aligned = 
        (struct_pool_size >= arena_size_minimum)
            ? r_mem::memory_manager_align_to_arena(struct_pool_size)
            : r_mem::memory_manager_align_to_arena(arena_size_minimum);

    //now, calculate the arenas, should always be at least 1
    const r_size arena_count = region_size_aligned / arena_size_aligned;
    if (arena_count == 0) {
        return(NULL);
    }


    //get a new region ptr
    RMemoryRegion* region_ptr = 
        r_mem_internal::region_initialize(
            reservation_ptr,
            region_size_aligned,
            arena_size_aligned,
            arena_count);

    //return the new region pointer
    return(region_ptr);
}

r_external const RHNDMemoryRegion
r_mem::region_list(
    const RHNDMemoryReservation reservation_handle) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    return(reservation_ptr ? reservation_ptr->region_list.first : NULL);

}

r_external const RHNDMemoryRegion
r_mem::region_at_index(
    const RHNDMemoryReservation reservation_handle, 
    const r_index               region_index) {

    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);

    return(NULL);
}

r_external const RHNDMemoryRegion
r_mem::region_next(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(NULL);
}

r_external const r_size
r_mem::region_size_total(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

r_external const r_size
r_mem::region_size_committed(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

r_external const r_size
r_mem::region_size_decommitted(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);
    
    return(0);
}

r_external const r_size
r_mem::region_arena_size(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

r_external const r_size
r_mem::region_arena_count_total(
    const RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

r_external const r_size
r_mem::region_arena_count_committed(
    RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

r_external const r_size
r_mem::region_arena_count_decommitted(
    RHNDMemoryRegion region_handle) {

    RMemoryRegion* region_ptr = r_mem_internal::region_from_handle(region_handle);

    return(0);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal r_void     
r_mem_internal::region_list_add(
    RMemoryReservation* reservation_ptr,
    RMemoryRegion*      region_ptr) {

    RMemoryRegionList& region_list = reservation_ptr->region_list;

    //calculate the starting arena address
    const r_memory reservation_start         = reservation_ptr->start;
    const r_size   reservation_size_occupied = reservation_ptr->region_list.total_size; 
    const r_memory region_arena_start        = reservation_start + reservation_size_occupied;  

    region_ptr->arena_start  = region_arena_start;
    region_ptr->region_index = region_list.count;


    if (!region_list.first) {
        region_list.first = region_ptr;
        region_list.last  = region_ptr;
    }

    region_list.last->next        = region_ptr;
    region_list.last              = region_ptr;
    region_list.total_size       += region_ptr->region_size;
}

r_internal const r_b8 
r_mem_internal::region_list_can_add(
    RMemoryReservation* reservation_ptr,
    const r_size        region_size) {

    const r_size reservation_size       = reservation_ptr->size;
    const r_size total_regions_size     = reservation_ptr->region_list.total_size; 
    const r_size new_total_regions_size = total_regions_size + region_size; 

    const r_b8 can_add = new_total_regions_size <= reservation_size;

    return(can_add);
}

r_internal RMemoryRegion* 
r_mem_internal::region_initialize(
          RMemoryReservation* reservation_ptr,
    const r_size              region_size_aligned,
    const r_size              arena_size_aligned,
    const r_size              arena_count) {

    //make sure we can add the region to the reservation
    const r_b8 can_add_region = r_mem_internal::region_list_can_add(reservation_ptr,region_size_aligned);
    if (!can_add_region) {
        return(NULL);
    }

    //push a region struct and an array of arenas onto the stack
    RMemoryRegion* region_ptr = r_mem_internal::stack_push_region();
    RMemoryArena*  arena_ptr  = r_mem_internal::stack_push_arenas(arena_count);

    //make sure we have our pointers
    if (!region_ptr || !arena_ptr) {
        return(NULL);
    }

    //initialize the region
    region_ptr->next              = NULL;
    region_ptr->arenas            = arena_ptr;
    region_ptr->arena_start       = NULL;
    region_ptr->arena_count       = arena_count;
    region_ptr->arena_size        = arena_size_aligned;
    region_ptr->region_size       = region_size_aligned;
    region_ptr->region_index      = 0;
    region_ptr->reservation_index = reservation_ptr->index;

    //initialize the arenas
    for (
        r_index arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        arena_ptr[arena_index].index    = arena_index;
        arena_ptr[arena_index].commit   = NULL;
        arena_ptr[arena_index].position = 0;
    }

    //add the region to the list
    r_mem_internal::region_list_add(reservation_ptr,region_ptr);

    //return the region ptr
    return(region_ptr);
}

r_internal RMemoryRegion* 
r_mem_internal::region_from_arena(RMemoryArena* arena_ptr) {

    if (!arena_ptr) {
        return(NULL);
    }

    //get to the front of the arena array
    RMemoryArena* first_arena_ptr = arena_ptr - arena_ptr->index;

    //the arena array is always at the end of a region allocation
    //so just subtract the aligned size of a region
    const r_size   region_struct_size_aligned = r_mem_internal::region_struct_size_aligned();
    const r_memory arena_memory               = (r_memory)first_arena_ptr;
    const r_memory region_memory              = arena_memory - region_struct_size_aligned;        

    RMemoryRegion* region_ptr = (RMemoryRegion*)region_memory; 

    return(region_ptr);
}

r_internal const r_size
r_mem_internal::region_struct_size_aligned() {

    const r_size region_struct_size_aligned = r_align_size_struct(RMemoryRegion);
    
    return(region_struct_size_aligned);
}