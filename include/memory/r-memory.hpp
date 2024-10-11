#ifndef R_MEMORY_HPP
#define R_MEMORY_HPP

#include <r-common.hpp>

typedef r_handle RMemoryReservationHandle;
typedef r_handle RMemoryRegionHandle;
typedef r_handle RMemoryArenaHandle;
typedef r_handle RMemoryBlockHandle;
typedef r_handle RMemoryBlockAllocatorHandle;
typedef r_handle RMemoryStackAllocatorHandle;
typedef r_handle RMemoryDoubleStackAllocatorHandle;

/**********************************************************************************/
/* SIZES                                                                          */
/**********************************************************************************/

namespace r_mem {

    inline const r_size size_kilobytes(const r_size size) { return(size * 1024);               }
    inline const r_size size_megabytes(const r_size size) { return(size * 1024 * 1024);        }
    inline const r_size size_gigabytes(const r_size size) { return(size * 1024 * 1024 * 1024); }
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

namespace r_mem {

    r_external const r_b8
    memory_manager_create(
        const RPlatformMemoryApi& platform_memory_api,
        const r_size              maximum_reserved_size,
        const r_size              reservation_alignment,
        const r_size              arena_alignment,
        const r_size              internal_stack_size);

    r_external const r_b8
    memory_manager_create_win32(
        const r_size maximum_reserved_size,
        const r_size internal_stack_size);

    r_external const r_b8   memory_manager_destroy                  (r_void);
    r_external const r_size memory_manager_maximum_reserved_size    (r_void);
    r_external const r_size memory_manager_reservation_alignment    (r_void);
    r_external const r_size memory_manager_arena_alignment          (r_void);
    r_external const r_size memory_manager_current_reserved_size    (r_void);
    r_external const r_size memory_manager_internal_stack_size_total(r_void);
    r_external const r_size memory_manager_internal_stack_size_used (r_void);
    r_external const r_size memory_manager_internal_stack_size_free (r_void);
    r_external const r_size memory_manager_align_to_reservation     (const r_size size);
    r_external const r_size memory_manager_align_to_arena           (const r_size size);
};

/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryReservationHandle 
    reserve(
        const r_cstr reservation_tag,
        const r_size minimum_reservation_size);

    r_external const r_size reservation_size_total         (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_size_used          (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_size_free          (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_arena_pool_count   (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_arena_region_count (const RMemoryReservationHandle reservation_handle);

    
    r_external const r_size                reservation_list_count (r_void);
    r_external const RMemoryReservationHandle reservation_list       (r_void);
    r_external const RMemoryReservationHandle reservation_next       (const RMemoryReservationHandle reservation_handle);
    r_external const RMemoryReservationHandle reservation_at_index   (const r_index reservation_index);
};

/**********************************************************************************/
/* REGION                                                                         */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryRegionHandle
    region_create(
        const RMemoryReservationHandle reservation_handle,
        const r_cstr                region_tag,
        const r_size                region_size_minimum,
        const r_size                arena_size_minimum);

    r_external const RMemoryRegionHandle
    region_create_arena_pool(
        const RMemoryReservationHandle reservation_handle,
        const r_cstr                region_tag,
        const r_size                arena_size_minimum,
        const r_size                arena_count);

    r_external const RMemoryRegionHandle
    region_create_struct_pool(
        const RMemoryReservationHandle reservation_handle,
        const r_cstr                region_tag,
        const r_size                arena_size_minimum,
        const r_size                struct_size,
        const r_size                struct_alignment,
        const r_size                struct_count);

    r_external const RMemoryRegionHandle region_list     (const RMemoryReservationHandle reservation_handle);
    r_external const RMemoryRegionHandle region_at_index (const RMemoryReservationHandle reservation_handle, const r_index);
    r_external const RMemoryRegionHandle region_next     (const RMemoryRegionHandle region_handle);

    r_external const r_size region_size_total              (const RMemoryRegionHandle region_handle);
    r_external const r_size region_size_committed          (const RMemoryRegionHandle region_handle);
    r_external const r_size region_size_decommitted        (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_size              (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_total       (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_committed   (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_decommitted (const RMemoryRegionHandle region_handle);
};

/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryArenaHandle arena_at_index     (const RMemoryRegionHandle region_handle, const r_index arena_index);
    r_external const RMemoryArenaHandle arena_commit_index (const RMemoryRegionHandle region_handle, const r_index arena_index);
    r_external const RMemoryArenaHandle arena_commit       (const RMemoryRegionHandle arena_handle);
    r_external const RMemoryArenaHandle arena_commit_next  (const RMemoryArenaHandle  arena_handle);
    r_external const r_b8            arena_decommit     (const RMemoryArenaHandle  arena_handle);

    r_external const r_b8 arena_is_committed          (const RMemoryArenaHandle  arena_handle);
    r_external const r_b8 arena_at_index_is_committed (const RMemoryRegionHandle region_handle, const r_index arena_index);

    r_external const r_size arena_size_total (const RMemoryArenaHandle arena_handle);
    r_external const r_size arena_size_used  (const RMemoryArenaHandle arena_handle);
    r_external const r_size arena_size_free  (const RMemoryArenaHandle arena_handle);
    
    r_external const r_memory arena_push         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external const r_memory arena_pull         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external const r_memory arena_push_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);
    r_external const r_memory arena_pull_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);

    r_external const r_b8 arena_can_push         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external const r_b8 arena_can_pull         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external const r_b8 arena_can_push_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);
    r_external const r_b8 arena_can_pull_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);
};

#define r_mem_arena_push_struct(arena,struct)    (struct*)r_mem::arena_push_aligned(arena,sizeof(struct),alignof(struct))
#define r_mem_arena_push_array(arena,count,type)   (type*)r_mem::arena_push(arena,sizeof(type) * count)

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryBlockAllocatorHandle 
    block_allocator_create(
        const RMemoryReservationHandle reservation_handle,
        const r_cstr                block_allocator_tag,
        const r_size                block_size,
        const r_size                block_alignment,
        const r_size                block_count,
        const r_size                minimum_arena_size);

    r_external const RMemoryBlockHandle block_allocator_commit                  (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const RMemoryBlockHandle block_allocator_block_at_index          (const RMemoryBlockAllocatorHandle block_allocator_handle, const r_index block_index);
    r_external const RMemoryBlockHandle block_allocator_commit_at_index         (const RMemoryBlockAllocatorHandle block_allocator_handle, const r_index block_index);
    
    r_external const r_b8 block_allocator_decommit          (const RMemoryBlockHandle block_handle);
    r_external const r_b8 block_allocator_reset             (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_b8 block_allocator_decommit_at_index (const RMemoryBlockAllocatorHandle block_allocator_handle, const r_index block_index);
    
    r_external const r_size block_allocator_size_total              (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_size_commited           (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_size_uncommited         (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_block_size              (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_block_count_total       (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_block_count_committed   (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_block_count_uncommitted (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_arena_size              (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_arena_count_total       (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_arena_count_committed   (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocator_arena_count_uncommitted (const RMemoryBlockAllocatorHandle block_allocator_handle);
}

/**********************************************************************************/
/* STACK ALLOCATOR                                                                */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryStackAllocatorHandle
    stack_allocator_create(
        const r_cstr stack_allocator_tag,
        const r_size stack_allocator_size,
        const r_size minimum_arena_size);

    r_external r_memory stack_allocator_memory (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external r_memory stack_allocator_push   (const RMemoryStackAllocatorHandle stack_allocator_handle, const r_size push_size);
    r_external r_memory stack_allocator_pull   (const RMemoryStackAllocatorHandle stack_allocator_handle, const r_size pull_size);
    
    r_external const r_b8   stack_allocator_set_save_point          (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_b8   stack_allocator_reset                   (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_b8   stack_allocator_reset_to_save_point     (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_size_total              (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_size_commited           (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_size_uncommited         (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_arena_size              (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_arena_count_total       (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_arena_count_committed   (const RMemoryStackAllocatorHandle stack_allocator_handle);
    r_external const r_size stack_allocator_arena_count_uncommitted (const RMemoryStackAllocatorHandle stack_allocator_handle);
};

/**********************************************************************************/
/* DOUBLE STACK ALLOCATOR                                                         */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryDoubleStackAllocatorHandle
    double_stack_allocator_create(
        const r_cstr stack_allocator_tag,
        const r_size stack_allocator_size,
        const r_size minimum_arena_size);

    r_external r_memory double_stack_allocator_a_memory (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external r_memory double_stack_allocator_a_push   (const RMemoryDoubleStackAllocatorHandle double_, const r_size push_size);
    r_external r_memory double_stack_allocator_a_pull   (const RMemoryDoubleStackAllocatorHandle double_, const r_size pull_size);

    r_external r_memory double_stack_allocator_a_memory (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external r_memory double_stack_allocator_a_push   (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle, const r_size push_size);
    r_external r_memory double_stack_allocator_a_pull   (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle, const r_size pull_size);
    
    r_external const r_b8     double_stack_allocator_a_set_save_point        (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_b8     double_stack_allocator_a_reset                 (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_b8     double_stack_allocator_a_reset_to_save_point   (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_a_arena_count_committed (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_a_size_used             (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);

    r_external const r_b8     double_stack_allocator_b_set_save_point        (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_b8     double_stack_allocator_b_reset                 (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_b8     double_stack_allocator_b_reset_to_save_point   (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_b_arena_count_committed (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_b_size_used             (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);

    r_external const r_size   double_stack_allocator_size_total              (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_arena_size              (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_arena_count_total       (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_arena_count_uncommitted (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_arena_count_committed   (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_size_commited           (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
    r_external const r_size   double_stack_allocator_size_uncommited         (const RMemoryDoubleStackAllocatorHandle double_stack_allocator_handle);
};

#endif //R_MEMORY_HPP