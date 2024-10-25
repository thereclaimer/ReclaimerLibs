#ifndef R_WIN32_WINDOW_HPP
#define R_WIN32_WINDOW_HPP

#include <Windows.h>
#include <r-common.hpp>
#include <r-memory.hpp>

typedef r_handle RWin32WindowHandle;
typedef r_handle RWin32RenderingContextHandle;
typedef r_u32    RWin32RenderingContextType;

struct RWin32WindowDimensions {
    r_u32 position_x;
    r_u32 position_y;
    r_u32 width;
    r_u32 height;    
};

enum RWin32WindowUseImGui_ {
     RWin32WindowUseImGui_Yes,
     RWin32WindowUseImGui_No
};

typedef r_u32 RWin32WindowUseImGui;

namespace r_win32 {

    r_external const RWin32WindowHandle
    window_create(
        const r_cstr                     title,
        const r_u32                      position_x,
        const r_u32                      position_y,
        const r_u32                      width,
        const r_u32                      height,
        const RColorFormat               color_format,
        const RWin32WindowUseImGui       use_imgui);

    r_external const RWin32WindowHandle
    window_create_centered(
        const r_cstr                     title,
        const r_u32                      width,
        const r_u32                      height,
        const RColorFormat               color_format,
        const RWin32WindowUseImGui       use_imgui);

    r_external const r_b8 window_show(const RWin32WindowHandle window_handle);
    r_external const r_b8 window_hide(const RWin32WindowHandle window_handle);

    r_external r_b8 window_dimensions(const RWin32WindowHandle window_handle, RWin32WindowDimensions& dimensions);
    
    r_external const r_b8     window_frame_start         (const RWin32WindowHandle window_handle);
    r_external const r_b8     window_frame_render        (const RWin32WindowHandle window_handle);
    r_external const r_timems window_frame_delta_time_ms (const RWin32WindowHandle window_handle);

    r_external const HWND window_win32_handle(const RWin32WindowHandle window_handle);
};

#endif //R_WIN32_WINDOW_HPP