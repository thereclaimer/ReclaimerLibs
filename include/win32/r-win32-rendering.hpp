#ifndef R_WIN32_RENDERING_HPP
#define R_WIN32_RENDERING_HPP

#include <r-common.hpp>
#include "r-win32-window.hpp"

enum RWin32RenderingContextType_ {
     RWin32RenderingContextType_None     = 0,
     RWin32RenderingContextType_OpenGL   = 1,
     RWin32RenderingContextType_Direct3D = 2
};

typedef r_u32 RWin32RenderingContextType;

struct RWin32RenderingViewport {
    r_u32 position_x;
    r_u32 position_y;
    r_u32 width;
    r_u32 height;
};

typedef r_handle RWin32RenderingContextHandle;

namespace r_win32 {

    r_external const RWin32RenderingContextHandle 
    rendering_create_opengl_context(
        const RHNDWin32Window window_handle);
    
    r_external const r_b8 
    rendering_set_clear_color(
        const RWin32RenderingContextHandle rendering_context_handle, 
        const RColor32Bit&                 color_32_ref);
    
    r_external const r_b8 
    rendering_clear(
        const RWin32RenderingContextHandle rendering_context_handle);

    r_external const r_b8 
    rendering_update_viewport_dimensions(
        const RWin32RenderingContextHandle rendering_context_handle,
        const r_u32                        position_x,
        const r_u32                        position_y,
        const r_u32                        width,
        const r_u32                        height);
};

#endif //R_WIN32_RENDERING_HPP