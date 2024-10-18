#ifndef R_WIN32_MONITOR_HPP
#define R_WIN32_MONITOR_HPP

#include <Windows.h>
#include <r-common.hpp>

struct RWin32MonitorInfo {
    r_u32 pixels_width;
    r_u32 pixels_height;
};

namespace r_win32 {

    r_external r_void monitor_info(RWin32MonitorInfo& monitor_info_ref);
};

#endif //R_WIN32_MONITOR_HPP