#pragma once

#include "r-memory-internal.hpp"

r_internal const r_b8
r_mem_internal::stack_create(
    const r_size stack_size) {

    if (stack_size <= 0) {
        return(false);
    }

    RMemoryStack& stack = r_mem_internal::memory_manager_get_stack();

    stack.size     = stack_size;
    stack.position = 0;
    stack.start    = r_mem_internal::platform_memory_reserve_and_commit(NULL,stack_size);

    if (!stack.start) {
        return(false);
    }

    return(true);
}

r_internal const r_b8
r_mem_internal::stack_can_push(
    const r_size size) {

    RMemoryStack& stack = r_mem_internal::memory_manager_get_stack();

    const r_size new_position = stack.position + size;
    const r_b8 can_push       = new_position > stack.size; 
    
    return(can_push);
}

r_internal r_memory
r_mem_internal::stack_push(
    const r_size size) {

    RMemoryStack& stack = r_mem_internal::memory_manager_get_stack();

    //make sure we can push
    const r_b8 can_push = r_mem_internal::stack_can_push(size);
    if (!can_push) {
        return(NULL);
    }

    //get the current stack memory
    const r_memory stack_memory = stack.start + stack.position;    

    //update the stack position
    stack.position += size;

    //return the stack memory
    return(stack_memory);
}

r_internal RMemoryReservation*
r_mem_internal::stack_push_reservation(
    r_void) {

    const r_size aligned_size = r_align_size_struct(RMemoryReservation);

    RMemoryReservation* reservation_ptr = (RMemoryReservation*)r_mem_internal::stack_push(aligned_size);    

    return(reservation_ptr);
}

r_internal RMemoryRegion*
r_mem_internal::stack_push_region(
    r_void) {

    const r_size aligned_size = r_align_size_struct(RMemoryRegion);

    RMemoryRegion* region_ptr = (RMemoryRegion*)r_mem_internal::stack_push(aligned_size);    

    return(region_ptr);
}

r_internal RMemoryArena*
r_mem_internal::stack_push_arenas(
    const r_size arena_count) {

    const r_size push_size = sizeof(RMemoryArena) * arena_count;

    RMemoryArena* arena_ptr = (RMemoryArena*)r_mem_internal::stack_push(push_size);    

    return(arena_ptr);
}

r_internal RMemoryBlockAllocator* 
r_mem_internal::stack_push_block_allocator(
    r_void) {

    const r_size aligned_size = r_align_size_struct(RMemoryBlockAllocator);

    RMemoryBlockAllocator* block_allocator_ptr = 
        (RMemoryBlockAllocator*)r_mem_internal::stack_push(aligned_size);    

    return(block_allocator_ptr);
}

r_internal RMemoryBlock*          
r_mem_internal::stack_push_blocks(
    const r_size block_count) {

    const r_size push_size = sizeof(RMemoryBlock) * block_count;

    RMemoryBlock* block_ptr = (RMemoryBlock*)r_mem_internal::stack_push(push_size);    

    return(block_ptr);
}