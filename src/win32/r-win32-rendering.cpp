#pragma once

#include "r-win32-rendering.hpp"
#include "r-win32-internal-context.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/glu.h>

r_external const r_b8 
r_win32::rendering_create_opengl_context(
    r_void) {

    //make sure we have an active context
    const HDC device_context = r_win32_internal::window_get_device_context_handle();
    if (!device_context) {
        return(false);
    }

    //set our preferred format descriptor
    PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
    preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
    preferred_format_descriptor.nVersion   = 1;
    preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
    preferred_format_descriptor.cColorBits = 32;

    //query for the closest format descriptor
    const r_s32 chosen_format_descriptor = 
        ChoosePixelFormat(
            device_context,
            &preferred_format_descriptor);

    if (!chosen_format_descriptor) {
        return(false);
    }

    //set the chosen pixel format
    const r_b8 pixel_format_is_set = 
        SetPixelFormat(
            device_context,
            chosen_format_descriptor,
            &preferred_format_descriptor);

    if (!pixel_format_is_set) {
        return(false);
    }

    //create the opengl context
    const HGLRC opengl_context = wglCreateContext(device_context);
    if (!opengl_context) {
        return(false);
    }

    //make the context current
    const r_b8 context_active = wglMakeCurrent(device_context, opengl_context);
    if (!context_active) {
        return(false);
    }

    //initialize the rendering context
    RWin32RenderingContext& rendering_context = r_win32_internal::window_get_rendering_context();
    rendering_context.opengl.device_context       = device_context;
    rendering_context.opengl.gl_rendering_context = opengl_context; 
    rendering_context.type                        = RWin32RenderingContextType_OpenGL;
    rendering_context.viewport.position_x = 0;
    rendering_context.viewport.position_y = 0;
    rendering_context.viewport.width      = r_win32_internal::window_get_width();
    rendering_context.viewport.height     = r_win32_internal::window_get_height();

    //set the viewport
    glViewport(
        rendering_context.viewport.position_x,
        rendering_context.viewport.position_y,
        rendering_context.viewport.width,
        rendering_context.viewport.height);

    //we're done
    return(true);
}

r_external const r_void 
r_win32::rendering_set_clear_color(
    RColor32Bit color_32) {

    //get the rendering context
    RWin32RenderingContext& rendering_context = r_win32_internal::window_get_rendering_context();
    
    //normalize the color
    r_common::color_32_bit_normalize(
        color_32,
        rendering_context.clear_color);
  
    switch (rendering_context.type) {

        case RWin32RenderingContextType_OpenGL: {
            
            //set the gl clear color
            glClearColor(
                rendering_context.clear_color.r,
                rendering_context.clear_color.g,
                rendering_context.clear_color.b,
                rendering_context.clear_color.a);

        } break;

        default: {
            //nothing to do
            return;
        } break;
    }


}

r_external const r_void 
r_win32::rendering_clear(
    r_void) {

    //get the rendering context
    RWin32RenderingContext& rendering_context = r_win32_internal::window_get_rendering_context();
    
    switch (rendering_context.type) {

        case RWin32RenderingContextType_OpenGL: {
            
            glClear(GL_COLOR_BUFFER_BIT);
        } break;

        default: {
            //nothing to do
            return;
        } break;
    }
}

r_external const r_void 
r_win32::rendering_update_viewport_dimensions(
    r_u32 position_x,
    r_u32 position_y,
    r_u32 width,
    r_u32 height) {

    //get the rendering context
    RWin32RenderingContext& rendering_context = r_win32_internal::window_get_rendering_context();
    
    //update the dimensions
    rendering_context.viewport.position_x = position_x;
    rendering_context.viewport.position_y = position_y;
    rendering_context.viewport.width      = width;
    rendering_context.viewport.height     = height;

    switch (rendering_context.type) {
        
        case RWin32RenderingContextType_OpenGL: {
                    
            glViewport(
                rendering_context.viewport.position_x,
                rendering_context.viewport.position_y,
                rendering_context.viewport.width,
                rendering_context.viewport.height);
        } break;

        default: {
            //nothing to do
            return;
        } break;
    }
}