#pragma once

#include "r-win32-internal-context.hpp"

r_external const r_b8
r_win32::context_create(
    RWin32MainArgs& r_win32_args) {

    r_b8 result = true;

    r_win32_internal::system_info_get();

    return(result);
}

r_external const r_b8 
r_win32::context_destroy(
    r_void) {

    r_b8 result = true;

    return(result);
}

r_external const r_b8 
r_win32::context_set_memory_region(
    const RHNDMemoryRegion region_handle) {

    _r_win32_context.region = region_handle;

    return(region_handle ? true : false);
}


r_internal RWin32Window* 
r_win32_internal::context_window_commit(
    r_void) {

    const RHNDMemoryArena window_arena_handle = r_win32_internal::context_arena_commit();
    if (!window_arena_handle) {
        return(NULL);
    }

    const r_size window_size      = sizeof(RWin32Window);
    const r_size window_alignment = alignof(RWin32Window);

    RWin32Window* window_ptr = 
        (RWin32Window*)r_mem::arena_push_aligned(
            window_arena_handle,
            window_size,
            window_alignment);
    
    if (!window_ptr) {
        return(NULL);
    }

    window_ptr->arena = window_arena_handle;

    return(window_ptr);
}

r_internal const r_b8
r_win32_internal::context_window_decommit(
    RWin32Window* window_ptr) {

    if (!window_ptr) {
        return(false);
    }

    return(r_mem::arena_decommit(window_ptr->arena));
}

r_internal RWin32RenderingContext* 
r_win32_internal::context_rendering_context_commit(
    r_void) {

    const RHNDMemoryArena rendering_context_arena_handle = r_win32_internal::context_arena_commit();
    if (!rendering_context_arena_handle) {
        return(NULL);
    }

    const r_size rendering_context_size      = sizeof(RWin32RenderingContext);
    const r_size rendering_context_alignment = alignof(RWin32RenderingContext);

    RWin32RenderingContext* rendering_context_ptr =
        (RWin32RenderingContext*)r_mem::arena_push_aligned(
            rendering_context_arena_handle,        
            rendering_context_size,
            rendering_context_alignment);

    if (!rendering_context_ptr) {
        return(NULL);
    }

    rendering_context_ptr->arena = rendering_context_arena_handle;

    return(rendering_context_ptr);
}

r_internal RWin32FileTable*
r_win32_internal::context_file_table_commit(
    r_void) {

    //get an arena
    const RHNDMemoryArena file_table_arena_handle = r_win32_internal::context_arena_commit();
    if (!file_table_arena_handle) {
        return(NULL);
    }

    //calculate table size and alignment
    const r_size file_table_size      = sizeof(RWin32FileTable);
    const r_size file_table_alignment = alignof(RWin32FileTable);

    //push the table onto the arena
    RWin32FileTable* file_table_ptr =
        (RWin32FileTable*)r_mem::arena_push_aligned(
            file_table_arena_handle,
            file_table_size,
            file_table_alignment);

    //if that failed, we're done
    if (!file_table_ptr) {
        return(NULL);
    }

    //set the file table arena, and we're done
    file_table_ptr->arena_handle = file_table_arena_handle;
    return(file_table_ptr);
}


r_internal const r_b8                    
r_win32_internal::context_rendering_context_decommit(
    RWin32RenderingContext* rendering_context_ptr) {

    if (!rendering_context_ptr) {
        return(false);
    }

    return(r_mem::arena_decommit(rendering_context_ptr->arena));

}

            