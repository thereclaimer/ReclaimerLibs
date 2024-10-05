#pragma once

#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const r_u64 
r_win32::system_ticks(
    r_void) {

    LARGE_INTEGER win32_large_int = {0};
    QueryPerformanceCounter(&win32_large_int);

    const r_u64 ticks = win32_large_int.QuadPart;

    return(ticks);
}

r_external const r_u64 
r_win32::system_frequency(
    r_void) {

    //get system frequency (Hz)
    LARGE_INTEGER win32_large_int = {0};
    QueryPerformanceFrequency(&win32_large_int);

    const r_u64 frequency = win32_large_int.QuadPart;

    return(frequency);
}

r_external const r_u64 
r_win32::system_process_id(
    r_void) {

    const r_u64 process_id = GetCurrentProcessId();

    return(process_id);
}

r_external const r_u64 
r_win32::system_thread_id(
    r_void) {

    const r_u64 thread_id = GetCurrentThreadId();

    return(thread_id);
}

r_external const r_size
r_win32::system_page_size(
    r_void) {

    RWin32SystemInfo& system_info = r_win32_internal::context_get_system_info();
    
    return(system_info.win32_system_info.dwPageSize);
}

r_external const r_size
r_win32::system_page_size_large(
    r_void) {

    RWin32SystemInfo& system_info = r_win32_internal::context_get_system_info();

    return(system_info.large_page_size);
}

r_external const r_size
r_win32::system_allocation_granularity(
    r_void) {

    RWin32SystemInfo& system_info = r_win32_internal::context_get_system_info();

    return(system_info.win32_system_info.dwAllocationGranularity);
}

r_external void
r_win32::system_sleep(
    const r_timems milliseconds) {

    Sleep(milliseconds);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal r_void 
r_win32_internal::system_info_get(
    r_void) {

    //get the current system time
    FILETIME system_time_as_file_time;
    GetSystemTimeAsFileTime(&system_time_as_file_time);
    
    //convert the system time to milliseconds
    const r_timems time_initialized = 
        ((r_timems)system_time_as_file_time.dwLowDateTime + 
        ((r_timems)(system_time_as_file_time.dwHighDateTime) << 32LL)) / 10000;

    //get the win32 system info
    RWin32SystemInfo& system_info = r_win32_internal::context_get_system_info();
    GetSystemInfo(&system_info.win32_system_info);

    //get large page size
    system_info.large_page_size = GetLargePageMinimum();

    //set initialization time
    system_info.time_initialized = time_initialized;
}
