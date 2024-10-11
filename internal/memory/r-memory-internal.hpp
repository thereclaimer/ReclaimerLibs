#ifndef R_MEMORY_INTERNAL_HPP
#define R_MEMORY_INTERNAL_HPP

#include <stdio.h>
#include <string.h>

#include "r-memory.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct RMemoryArena;
struct RMemoryReservation;
struct RMemoryRegion;
struct RMemoryBlockAllocator;
struct RMemoryBlock;

struct RMemoryArena {
    r_index   index;
    r_memory  commit;
    r_address position;
};

namespace r_mem_internal {

    RMemoryArena* arena_from_handle (const RMemoryArenaHandle arena_handle);
};

/**********************************************************************************/
/* REGION                                                                         */
/**********************************************************************************/

struct RMemoryRegion {
    RMemoryRegion* next;
    RMemoryArena*  arenas;
    r_memory       arena_start;
    r_size         arena_count;
    r_size         arena_size;
    r_size         region_size;
    r_index        region_index;
    r_index        reservation_index;
};

struct RMemoryRegionList{
    RMemoryRegion* first;
    RMemoryRegion* last;
    r_size         count;
    r_size         total_size;
};

namespace r_mem_internal {

    inline RMemoryRegion* region_from_handle(const RMemoryRegionHandle region_handle) { return((RMemoryRegion*)region_handle); }
    
    r_internal r_void         region_list_add            (RMemoryReservation* reservation_ptr, RMemoryRegion* region_ptr);
    r_internal const r_b8     region_list_can_add        (RMemoryReservation* reservation_ptr, const r_size region_size);
    r_internal const r_size   region_struct_size_aligned (r_void);
    r_internal RMemoryRegion* region_from_arena          (RMemoryArena* arena_ptr);

    r_internal RMemoryRegion* 
    region_initialize(
              RMemoryReservation* reservation_ptr,
        const r_size              region_size_aligned,
        const r_size              arena_size_aligned,
        const r_size              arena_count);
};


/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

namespace r_mem_internal {

    inline RMemoryArena* arena_from_handle (const RMemoryArenaHandle arena_handle) { return((RMemoryArena*)arena_handle); }

    r_internal const r_b8
    arena_commit_immediate(
        RMemoryRegion* region_ptr,
        RMemoryArena*  arena_ptr);

    r_internal RMemoryArena*
    arena_commit_immediate_index(
              RMemoryRegion* region_ptr,
        const r_index        arena_index);

    r_internal const r_b8
    arena_next_uncommitted(
              RMemoryRegion* in_region_ptr,
        const r_index        in_starting_arena_index,
              r_index*       out_uncommitted_arena_index);

    r_internal const r_b8
    arena_decommit_immediate(
        RMemoryRegion* region_ptr,
        RMemoryArena*  arena_ptr);

    r_internal const r_size arena_count_committed   (RMemoryRegion* region_ptr);
    r_internal const r_size arena_count_uncommitted (RMemoryRegion* region_ptr);
};


/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

struct RMemoryStack {
    r_memory  start;
    r_size    size;
    r_address position;
};

namespace r_mem_internal {

    r_internal const r_b8             stack_create   (const r_size stack_size);
    r_internal const r_b8             stack_can_push (const r_size size);
    r_internal r_memory               stack_push     (const r_size size);
    r_internal RMemoryReservation*    stack_push_reservation(r_void);  
    r_internal RMemoryRegion*         stack_push_region(r_void);
    r_internal RMemoryArena*          stack_push_arenas(const r_size arena_count);
    r_internal RMemoryBlockAllocator* stack_push_block_allocator(r_void);
    r_internal RMemoryBlock*          stack_push_blocks(const r_size block_count);
};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

struct RMemoryBlock {
    r_memory start;
    r_index  index;
    r_index  arena_index;
};

struct RMemoryBlockAllocator {
    RMemoryBlockAllocator* next;
    r_index                reservation_index;
    r_index                region_index;
    r_index                block_allocator_index;
    r_size                 block_count_total;
    r_size                 block_count_per_arena;
    r_size                 block_size;
    RMemoryBlock*          blocks;
};

struct RMemoryBlockAllocatorList {
    RMemoryBlockAllocator* first;
    RMemoryBlockAllocator* last;
    r_size                 count;
};

namespace r_mem_internal {

    inline RMemoryBlockAllocator* 
    block_allocator_from_handle(const RMemoryBlockAllocatorHandle block_allocator_handle) {
        return((RMemoryBlockAllocator*)block_allocator_handle);
    }

    inline RMemoryBlock*
    block_from_handle(const RMemoryBlockHandle block_handle) { return((RMemoryBlock*)block_handle); }

    r_internal RMemoryBlockAllocator*
    block_allocator_from_block(RMemoryBlock* block_ptr);

    const r_size
    block_allocator_struct_size_aligned(
        r_void) {

        const r_size size_aligned = r_align_size_struct(RMemoryBlockAllocator);
        return(size_aligned);
    }
};

/**********************************************************************************/
/* STACK ALLOCATOR                                                                */
/**********************************************************************************/

struct RMemoryStackAllocator {
    RMemoryStackAllocator* next;
    r_memory               stack_start;
    r_size                 stack_size;
    r_address              stack_position;
    r_index                reservation_index;
    r_index                region_index;
    r_index                stack_allocator_index;
    r_index                current_arena_index;
};

struct RMemoryStackAllocatorList {
    RMemoryStackAllocator* first;
    RMemoryStackAllocator* last;
    r_size                 count;
};

/**********************************************************************************/
/* DOUBLE STACK ALLOCATOR                                                         */
/**********************************************************************************/

struct RMemoryDoubleStackAllocatorSubStack {
    r_memory  start;
    r_size    size;
    r_address position;
    r_index   current_arena_index;
    r_b32     top_down;
};

struct RMemoryDoubleStackAllocator {
    RMemoryDoubleStackAllocator*        next;
    r_index                             reservation_index;
    r_index                             region_index;
    r_index                             double_stack_allocator_index;
    RMemoryDoubleStackAllocatorSubStack stack_a;
    RMemoryDoubleStackAllocatorSubStack stack_b;
};

struct RMemoryDoubleStackAllocatorList {
    RMemoryDoubleStackAllocator* first;
    RMemoryDoubleStackAllocator* last;
    r_size                       count;
};


/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

struct RMemoryReservation {
    RMemoryReservation*             next;
    RMemoryRegionList               region_list;
    RMemoryBlockAllocatorList       block_allocator_list;
    RMemoryStackAllocatorList       stack_allocator_list;
    RMemoryDoubleStackAllocatorList double_stack_allocator_list;
    r_memory                        start;
    r_size                          size;
    r_address                       position;
    r_index                         index;
};

struct RMemoryReservationList {
    RMemoryReservation* first;        
    RMemoryReservation* last;
    r_size              count;
    r_size              total_reserved_size;
};

namespace r_mem_internal {

    inline RMemoryReservation* reservation_from_handle(const RMemoryReservationHandle reservation_handle) { return((RMemoryReservation*)reservation_handle); }    

    r_internal r_void       reservation_list_add         (RMemoryReservation* reservation_ptr);
    r_internal const r_b8   reservation_list_release_all (r_void);
    r_internal const r_b8   reservation_list_can_add     (const r_size reservation_size);
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

struct RMemoryManager {
    RMemoryReservationList reservation_list;
    RPlatformMemoryApi     platform_memory_api;
    RMemoryStack           stack;
    r_size                 maximum_reserved_size;
    r_size                 reservation_alignment;
    r_size                 arena_alignment;
};

r_global RMemoryManager _r_memory_manager;

namespace r_mem_internal {

    inline r_memory platform_memory_reserve              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve(start,size));            }
    inline r_memory platform_memory_commit               (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.commit(start,size));             }
    inline r_memory platform_memory_reserve_and_commit   (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve_and_commit(start,size)); }
    inline r_b8     platform_memory_decommit             (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.decommit(start,size));           }
    inline r_b8     platform_memory_release              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.release(start,size));            }

    inline RMemoryStack&             memory_manager_get_stack            (r_void) { return(_r_memory_manager.stack); };
    inline RMemoryReservationList&   memory_manager_get_reservation_list (r_void) { return(_r_memory_manager.reservation_list); };
};

#endif //R_MEMORY_INTERNAL_HPP