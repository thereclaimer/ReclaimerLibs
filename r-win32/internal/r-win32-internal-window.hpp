#ifndef R_WIN32_INTERNAL_WINDOW_HPP
#define R_WIN32_INTERNAL_WINDOW_HPP

#include "r-win32.hpp"
#include "r-win32-internal-rendering.hpp"

typedef LRESULT
(*r_win32_funcptr_window_on_wm_message)(
    WPARAM w_param,
    LPARAM l_param);

struct RWin32Window {
    HWND                   win32_handle_window;
    HDC                    win32_handle_device_context;
    MSG                    message;
    r_u32                  position_x;
    r_u32                  position_y;
    r_u32                  width;
    r_u32                  height;
    r_b32                  quit_received;
    RColorFormat           color_format;
    r_u64                  frame_system_ticks_start;
    r_u64                  frame_system_ticks_render;
    RWin32RenderingContext rendering_context;
};

namespace r_win32_internal {

    r_internal LRESULT CALLBACK
    window_callback(
        HWND   window_handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param);

    r_internal LRESULT window_on_wm_size    (WPARAM w_param, LPARAM l_param);
    r_internal LRESULT window_on_wm_move    (WPARAM w_param, LPARAM l_param);
    r_internal LRESULT window_on_wm_quit    (WPARAM w_param, LPARAM l_param);
    r_internal LRESULT window_on_wm_close   (WPARAM w_param, LPARAM l_param);
    r_internal LRESULT window_on_wm_destroy (WPARAM w_param, LPARAM l_param);
};

#endif //R_WIN32_INTERNAL_WINDOW_HPP