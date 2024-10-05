#ifndef R_WIN32_WINDOW_HPP
#define R_WIN32_WINDOW_HPP

#include <Windows.h>
#include <r-common.hpp>
#include <r-memory.hpp>

typedef r_handle RHNDWin32Window;
typedef r_handle RHNDWin32RenderingContext;
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

    r_external const RHNDWin32Window
    window_create(
        const r_cstr                     title,
        const r_u32                      position_x,
        const r_u32                      position_y,
        const r_u32                      width,
        const r_u32                      height,
        const RColorFormat               color_format,
        const RWin32WindowUseImGui       use_imgui);

    r_external const RHNDWin32Window
    window_create_centered(
        const r_cstr                     title,
        const r_u32                      width,
        const r_u32                      height,
        const RColorFormat               color_format,
        const RWin32WindowUseImGui       use_imgui);

    r_external const r_b8 window_show(const RHNDWin32Window window_handle);
    r_external const r_b8 window_hide(const RHNDWin32Window window_handle);

    r_external r_b8 window_dimensions(const RHNDWin32Window window_handle, RWin32WindowDimensions& dimensions);
    
    r_external const r_b8     window_frame_start         (const RHNDWin32Window window_handle);
    r_external const r_b8     window_frame_render        (const RHNDWin32Window window_handle);
    r_external const r_timems window_frame_delta_time_ms (const RHNDWin32Window window_handle);
};

#endif //R_WIN32_WINDOW_HPP