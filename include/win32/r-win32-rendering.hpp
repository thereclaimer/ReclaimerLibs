#ifndef R_WIN32_RENDERING_HPP
#define R_WIN32_RENDERING_HPP

#include <r-common.hpp>

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

struct RWin32RenderingContextOpenGL {
    HDC   device_context;
    HGLRC gl_rendering_context;
};

struct RWin32RenderingContextDX3D {
    //TODO

};

struct RWin32RenderingContext {
    RWin32RenderingContextType type;
    RWin32RenderingViewport    viewport;
    RColor32BitNormalized      clear_color;
    union {
        RWin32RenderingContextOpenGL opengl;
        RWin32RenderingContextDX3D   dx3d;
    };
};

namespace r_win32 {

    r_external const r_b8   rendering_create_opengl_context(r_void);
    r_external const r_void rendering_set_clear_color(RColor32Bit color_32);
    r_external const r_void rendering_clear(r_void);

    r_external const r_void 
    rendering_update_viewport_dimensions(
        r_u32 position_x,
        r_u32 position_y,
        r_u32 width,
        r_u32 height);
};

#endif //R_WIN32_RENDERING_HPP