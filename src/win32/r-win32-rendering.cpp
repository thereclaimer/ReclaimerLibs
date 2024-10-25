#pragma once

#include "r-win32-rendering.hpp"
#include "r-win32-internal-context.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/glu.h>
#include "r-win32-internal-window.hpp"
#include "r-win32-internal-rendering.hpp"

r_external const RWin32RenderingContextHandle 
r_win32::rendering_create_opengl_context(
    const RWin32WindowHandle window_handle) {


    if(!window_handle) {
        return(false);
    }
    RWin32Window* window_ptr = (RWin32Window*)window_handle; 

    //make sure we have an active context
    const HDC device_context = window_ptr->win32_handle_device_context; 
    if (!device_context) {
        return(false);
    }

    //allocate a rendering context
    RWin32RenderingContext* rendering_context_ptr = r_win32_internal::context_rendering_context_commit();
    if (!rendering_context_ptr) {
        return(NULL);
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

    rendering_context_ptr->opengl.device_context       = device_context;
    rendering_context_ptr->opengl.gl_rendering_context = opengl_context; 
    rendering_context_ptr->type                        = RWin32RenderingContextType_OpenGL;
    rendering_context_ptr->viewport.position_x         = 0;
    rendering_context_ptr->viewport.position_y         = 0;
    rendering_context_ptr->viewport.width              = window_ptr->width;
    rendering_context_ptr->viewport.height             = window_ptr->height;

    //set the viewport
    glViewport(
        rendering_context_ptr->viewport.position_x,
        rendering_context_ptr->viewport.position_y,
        rendering_context_ptr->viewport.width,
        rendering_context_ptr->viewport.height);

    //set the rendering context in the window
    window_ptr->rendering_context_ptr = rendering_context_ptr;

    //we're done
    return(rendering_context_ptr);
}

r_external const r_b8 
r_win32::rendering_set_clear_color(
    const RWin32RenderingContextHandle rendering_context_handle, 
    const RColor32Bit&                 color_32_ref) {

    //get the rendering context
    if (!rendering_context_handle) {
        return(false);
    }

    RWin32RenderingContext* rendering_context_ptr = (RWin32RenderingContext*)rendering_context_handle;

    //normalize the color
    r_common::color_32_bit_normalize(
        color_32_ref,
        rendering_context_ptr->clear_color);
  
    switch (rendering_context_ptr->type) {

        case RWin32RenderingContextType_OpenGL: {
            
            //set the gl clear color
            glClearColor(
                rendering_context_ptr->clear_color.r,
                rendering_context_ptr->clear_color.g,
                rendering_context_ptr->clear_color.b,
                rendering_context_ptr->clear_color.a);

        } break;

        default: {
            //nothing to do
            return(true);
        } break;
    }

    return(true);
}

r_external const r_b8 
r_win32::rendering_clear(
    const RWin32RenderingContextHandle rendering_context_handle) {

    //get the rendering context
    if (!rendering_context_handle) {
        return(false);
    }

    RWin32RenderingContext* rendering_context_ptr = (RWin32RenderingContext*)rendering_context_handle;
    
    switch (rendering_context_ptr->type) {

        case RWin32RenderingContextType_OpenGL: {
            
            glClear(GL_COLOR_BUFFER_BIT);
        } break;

        default: {
            //nothing to do
            return(true);
        } break;
    }

    return(true);
}

r_external const r_b8 
r_win32::rendering_update_viewport_dimensions(
    const RWin32RenderingContextHandle rendering_context_handle,
    const r_u32                        position_x,
    const r_u32                        position_y,
    const r_u32                        width,
    const r_u32                        height) {

    //get the rendering context
    if (!rendering_context_handle) {
        return(false);
    }

    RWin32RenderingContext* rendering_context_ptr = (RWin32RenderingContext*)rendering_context_handle;
    
    //update the dimensions
    rendering_context_ptr->viewport.position_x = position_x;
    rendering_context_ptr->viewport.position_y = position_y;
    rendering_context_ptr->viewport.width      = width;
    rendering_context_ptr->viewport.height     = height;

    switch (rendering_context_ptr->type) {
        
        case RWin32RenderingContextType_OpenGL: {
                    
            glViewport(
                rendering_context_ptr->viewport.position_x,
                rendering_context_ptr->viewport.position_y,
                rendering_context_ptr->viewport.width,
                rendering_context_ptr->viewport.height);
        } break;

        default: {
            //nothing to do
            return(true);
        } break;
    }

    return(true);
}