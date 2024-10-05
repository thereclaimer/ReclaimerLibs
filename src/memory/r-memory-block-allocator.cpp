#pragma once

#include "r-memory-internal.hpp"

r_external const RHNDMemoryBlockAllocator 
r_mem::block_allocator_create(
    const RHNDMemoryReservation reservation_handle,
    const r_cstr                block_allocator_tag,
    const r_size                block_size,
    const r_size                block_alignment,
    const r_size                block_count,
    const r_size                minimum_arena_size) {

    //get the reservation
    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    if (!reservation_ptr) {
        return(NULL);
    }

    //create a struct region for the allocator
    RMemoryRegion* region_ptr = 
        (RMemoryRegion*)r_mem::region_create_struct_pool(
            reservation_handle,
            block_allocator_tag,
            minimum_arena_size,
            block_size,
            block_alignment,
            block_count);

    if (!region_ptr) {
        return(NULL);
    }

    //now, we need to adjust our block count
    const r_size region_size        = region_ptr->arena_size;
    const r_size block_size_aligned = r_align_a_to_b(block_size,block_alignment);
    const r_size block_count_actual = region_size / block_size_aligned;  

    //push the blocks onto the stack
    RMemoryBlockAllocator* block_allocator_ptr = r_mem_internal::stack_push_block_allocator();
    RMemoryBlock*          block_ptr           = r_mem_internal::stack_push_blocks(block_count_actual);

    //sanity check
    if (!block_allocator_ptr || block_ptr) {
        return(NULL);
    }

    //calculate the blocks per arena
    const r_size arena_size = region_ptr->arena_size; 
    const r_size arena_count = region_ptr->arena_count;
    const r_size block_count_per_arena = arena_size / block_size_aligned;

    //for each arena, we are going to add the calculated number of blocks
    r_index current_block_index = 0;

    for (
        r_index arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        for (
            r_index arena_block_index = 0;
            arena_block_index < block_count_per_arena;
            ++arena_block_index) {
                        
            block_ptr[current_block_index].index       = arena_block_index;
            block_ptr[current_block_index].arena_index = arena_index;
            block_ptr[current_block_index].start       = NULL;

            ++current_block_index;
        }
    }

    //initialize the allocator
    block_allocator_ptr->next                  = NULL;
    block_allocator_ptr->reservation_index     = reservation_ptr->index;
    block_allocator_ptr->region_index          = region_ptr->region_index;
    block_allocator_ptr->block_allocator_index = 0;
    block_allocator_ptr->block_count_total     = block_count_actual;
    block_allocator_ptr->block_count_per_arena = block_count_per_arena;
    block_allocator_ptr->block_size            = block_size_aligned;
    block_allocator_ptr->blocks                = block_ptr;

    //TODO: add to list

    //return the allocator
    return(block_allocator_ptr);
}

r_external const RHNDMemoryBlock
r_mem::block_allocator_commit(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    //get the block allocator
    RMemoryBlockAllocator* block_allocator_ptr = r_mem_internal::block_allocator_from_handle(block_allocator_handle);

    //get the block reservation
    RMemoryReservation* reservation_ptr = 
        (RMemoryReservation*)r_mem::reservation_at_index(block_allocator_ptr->reservation_index);

    //get the block region
    RMemoryRegion* region_ptr = 
        (RMemoryRegion*)r_mem::region_at_index(
            reservation_ptr,
            block_allocator_ptr->region_index);


    //find the first uncommitted block
    RMemoryBlock* selected_block = NULL;
    const r_size block_array_size = sizeof(RMemoryBlock) * block_allocator_ptr->block_count_total;

    for (
        RMemoryBlock* current_block = block_allocator_ptr->blocks;
        current_block < block_allocator_ptr->blocks + block_array_size;
        ++current_block) {

        if (!current_block->start) {
            selected_block = current_block;
            break;
        }
    }

    //if we didn't find a block, we're done
    if (!selected_block) {
        return(NULL);
    }

    //if this is the first block in the arena, we need to commit the arena
    //otherwise, just grab it
    RMemoryArena* arena_ptr = 
        selected_block->index == 0
            ? (RMemoryArena*)r_mem::arena_commit_index(region_ptr, selected_block->arena_index)
            : (RMemoryArena*)r_mem::arena_at_index(region_ptr, selected_block->arena_index);

    //sanity check
    if (!arena_ptr) {
        return(NULL);
    }

    //grab the start of this arena
    const r_memory arena_start = arena_ptr->commit; 

    //calculate the offset of this block's memory
    const r_size offset = block_allocator_ptr->block_size * selected_block->index;
    
    //get the block memory
    r_memory block_memory_start = arena_start + offset;

    //update the block
    selected_block->start = block_memory_start;

    //return the block
    return(selected_block);
}

r_external const r_b8     
r_mem::block_allocator_decommit(
    const RHNDMemoryBlock block_handle) {

    //get the block
    RMemoryBlock* block_ptr = r_mem_internal::block_from_handle(block_handle);
    if (!block_ptr) {
        return(false);
    }

    //get the allocator
    RMemoryBlockAllocator* block_allocator_ptr = 
        r_mem_internal::block_allocator_from_block(block_ptr);
    if (!block_allocator_ptr) {
        return(false);
    }

    //clear the block
    memset(
        block_ptr->start,
        0,
        block_allocator_ptr->block_size);
    block_ptr->start = NULL;


    //check to see if the blocks in this arena are cleared
    //if they are, we can decommit it
    const r_index block_index_start = block_ptr->arena_index * block_allocator_ptr->block_count_per_arena;
    RMemoryBlock* block_start = &block_allocator_ptr->blocks[block_index_start];
    RMemoryBlock* block_end   = block_start + block_allocator_ptr->block_count_per_arena; 

    r_b8 can_decommit = true;

    for (
        RMemoryBlock* block = block_start;
        block < block_end;
        ++block) {

        can_decommit &= !block->start;
    }

    if (can_decommit) {

        RMemoryReservation* reservation_ptr = (RMemoryReservation*)r_mem::reservation_at_index(block_allocator_ptr->reservation_index);
        RMemoryRegion* region_ptr           = (RMemoryRegion*)r_mem::region_at_index(reservation_ptr,block_allocator_ptr->region_index);
        RMemoryArena* arena_ptr             = (RMemoryArena*)r_mem::arena_at_index(region_ptr,block_ptr->arena_index);

        r_mem::arena_decommit(arena_ptr);
    }        

    return(true);
}

r_external const r_b8     
r_mem::block_allocator_reset(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    //get the block allocator
    RMemoryBlockAllocator* block_allocator_ptr = r_mem_internal::block_allocator_from_handle(block_allocator_handle);
    if (!block_allocator_ptr) {
        return(false);
    }

    //get the reservation and region
    RMemoryReservation* reservation_ptr = (RMemoryReservation*)r_mem::reservation_at_index(block_allocator_ptr->reservation_index);
    RMemoryRegion*      region_ptr      = (RMemoryRegion*)r_mem::region_at_index(reservation_ptr,block_allocator_ptr->region_index);
    if (!reservation_ptr || !region_ptr) {
        return(false);
    }        

    //get our arena and block counts
    const r_size arena_count = region_ptr->arena_count; 
    const r_size block_count = block_allocator_ptr->block_count_per_arena; 

    r_b8 allocator_reset_success = true;

    //loop through the arenas and blocks
    for (
        r_index arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        //get the address range of blocks for this arena
        RMemoryBlock* block_start = &block_allocator_ptr->blocks[(block_count * arena_index)];
        RMemoryBlock* block_end   = block_start + block_count; 

        //for each block, clear the start
        for (
            RMemoryBlock* block = block_start;
            block < block_end;
            ++block) {

            block->start = 0;
        }

        //decommit the arena
        RMemoryArena* arena_ptr = (RMemoryArena*)r_mem::arena_at_index(region_ptr,arena_index);
        allocator_reset_success &= r_mem::arena_decommit(arena_ptr);
    }

    return(allocator_reset_success);
}

r_external const r_size   
r_mem::block_allocator_size_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_size_commited(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_size_uncommited(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_block_size(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_block_count_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_block_count_committed(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_block_count_uncommitted(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_arena_size(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_arena_count_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_arena_count_committed(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const r_size   
r_mem::block_allocator_arena_count_uncommitted(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    return(0);
}

r_external const RHNDMemoryBlock 
r_mem::block_allocator_block_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle,
    const r_index                  block_index) {

    return(0);
}

r_external const RHNDMemoryBlock
r_mem::block_allocator_commit_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle,
    const r_index                  block_index) {

    return(0);
}

r_external const r_b8 
r_mem::block_allocator_decommit_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle, 
    const r_index                  block_index) {

    return(true);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal RMemoryBlockAllocator*
r_mem_internal::block_allocator_from_block(
    RMemoryBlock* block_ptr) {

    if (!block_ptr) {
        return(NULL);
    }

    //get to the front of the arena array
    RMemoryBlock* first_block_ptr = block_ptr - block_ptr->index;

    //the arena array is always at the end of a region allocation
    //so just subtract the aligned size of a region
    const r_size   block_allocator_struct_size_aligned = r_mem_internal::block_allocator_struct_size_aligned();
    const r_memory block_memory                        = (r_memory)first_block_ptr;
    const r_memory block_allocator_memory              = block_memory - block_allocator_struct_size_aligned;        

    RMemoryBlockAllocator* block_allocator_ptr = (RMemoryBlockAllocator*)block_allocator_memory; 

    return(block_allocator_ptr);

}