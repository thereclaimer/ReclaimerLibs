#pragma once

#include "r-win32-internal-context.hpp"

r_external const r_b8
r_win32::context_create(
          RWin32MainArgs& r_win32_args,
    const r_size          file_count_max,
    const r_size          stack_size) {

    r_b8 result = true;

    r_win32_internal::system_info_get();

    //create the stack
    if (stack_size > 0) {

        RWin32ContextStack& stack_ref = _r_win32_context.stack;
        stack_ref.size     = r_win32::memory_align_to_allocation_granularity(stack_size);
        stack_ref.start    = r_win32::memory_reserve_and_commit(stack_size);
        stack_ref.position = 0;
    }

    return(result);
}

r_external const r_b8 
r_win32::context_destroy(
    r_void) {

    r_b8 result = true;

    return(result);
}

r_internal const r_b8
r_win32_internal::context_stack_can_push(
    const r_size size) {

    RWin32ContextStack& stack_ref = _r_win32_context.stack;

    const r_size new_position = stack_ref.position + size;

    if (!stack_ref.start || new_position >= stack_ref.size) {
        return(false);
    }
}

r_internal r_memory 
r_win32_internal::context_stack_push(
    const r_size size) {

    const r_b8 can_push = r_win32_internal::context_stack_can_push(size);
    if (!can_push) {
        return(NULL);
    }

    RWin32ContextStack& stack_ref = _r_win32_context.stack;
    r_memory stack_memory = stack_ref.start + stack_ref.position;
    stack_ref.position += size;

    return(stack_memory);
}

r_internal r_memory
r_win32_internal::context_stack_push_aligned(
    const r_size size,
    const r_size alignment) {

    const r_size size_aligned = r_align_a_to_b(size,alignment);

    r_memory stack_memory = r_win32_internal::context_stack_push(size_aligned);

    return(stack_memory);
}
