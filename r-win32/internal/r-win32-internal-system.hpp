#ifndef R_WIN32_INTERNAL_SYSTEM_HPP
#define R_WIN32_INTERNAL_SYSTEM_HPP

#include "r-win32.hpp"

struct RWin32SystemInfo {
    r_timems    time_initialized;
    SYSTEM_INFO win32_system_info;
    r_size      large_page_size;
};

namespace r_win32_internal {

    r_internal r_void system_info_get(r_void);
};

#endif //R_WIN32_INTERNAL_SYSTEM_HPP