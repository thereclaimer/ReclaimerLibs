#include "r-win32-monitor.hpp"

r_external r_void 
r_win32::monitor_info(
    RWin32MonitorInfo& monitor_info_ref) {

    monitor_info_ref.pixels_width  = GetSystemMetrics(SM_CXSCREEN); 
    monitor_info_ref.pixels_height = GetSystemMetrics(SM_CYSCREEN); 
}