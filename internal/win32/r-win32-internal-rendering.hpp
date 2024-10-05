#ifndef R_WIN32_INTERNAL_RENDERING_HPP
#define R_WIN32_INTERNAL_RENDERING_HPP

#include "r-win32.hpp"

struct RWin32RenderingContextOpenGL {
    HDC   device_context;
    HGLRC gl_rendering_context;
};

struct RWin32RenderingContextDX3D {
    //TODO

};

struct RWin32RenderingContext {
    RHNDMemoryArena            arena;
    RWin32RenderingContextType type;
    RWin32RenderingViewport    viewport;
    RColor32BitNormalized      clear_color;
    union {
        RWin32RenderingContextOpenGL opengl;
        RWin32RenderingContextDX3D   dx3d;
    };
};


#endif //R_WIN32_INTERNAL_RENDERING_HPP