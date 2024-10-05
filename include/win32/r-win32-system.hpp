#ifndef R_WIN32_SYSTEM_HPP
#define R_WIN32_SYSTEM_HPP

#include <Windows.h>
#include <r-common.hpp>

namespace r_win32 {

    r_external const r_u64  system_ticks                  (r_void);
    r_external const r_u64  system_frequency              (r_void);
    r_external const r_u64  system_process_id             (r_void);
    r_external const r_u64  system_thread_id              (r_void);
    r_external const r_size system_page_size              (r_void);
    r_external const r_size system_page_size_large        (r_void);
    r_external const r_size system_allocation_granularity (r_void);

    r_external r_void system_sleep (const r_timems milliseconds);
};

#endif //R_WIN32_SYSTEM_HPP