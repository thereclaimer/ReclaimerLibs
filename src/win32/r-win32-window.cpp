#pragma once

#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const r_b8
r_win32::window_create(
    const r_cstr                     title,
    const r_u32                      position_x,
    const r_u32                      position_y,
    const r_u32                      width,
    const r_u32                      height,
    const RColorFormat               color_format,
    const RWin32RenderingContextType rendering_context,
    const RWin32WindowUseImGui       use_imgui) {

    //check we are initialized
    if (!r_win32_internal::context_is_initialized()) {
        return(false);
    }

    //get the instance from the args
    const HINSTANCE h_instance = r_win32_internal::main_args_get_h_instance();

    //define window class
    WNDCLASSA window_class = {0};
    window_class.lpfnWndProc   = (WNDPROC)r_win32_internal::window_callback;
    window_class.hInstance     = h_instance; 
    window_class.lpszClassName = "RWin32Window";
    window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    //register window class
    if (!RegisterClass(&window_class)) {
        return(false);
    }

    //create the window
    const HWND window_handle = 
        CreateWindowA(
            window_class.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            position_x,
            position_y,
            width,
            height,
            NULL,
            NULL,
            h_instance,
            NULL
        );

    if (!window_handle) {
        return(false);
    }

    //get the device context
    const HDC device_context = GetDC(window_handle);
    if (!device_context) {
        return(false);
    }

    //intiialize the window structure
    RWin32Window& window = r_win32_internal::context_get_window();
    window.win32_handle_window          = window_handle;
    window.win32_handle_device_context  = device_context;
    window.position_x                   = position_x;
    window.position_y                   = position_y;
    window.width                        = width;
    window.height                       = height;
    window.color_format                 = color_format; 
    window.rendering_context            = {0};

    return(true);
}

r_external const r_b8
r_win32::window_create_centered(
    const r_cstr                     title,
    const r_u32                      width,
    const r_u32                      height,
    const RWin32RenderingContextType rendering_context,
    const RColorFormat               color_format,
    const RWin32WindowUseImGui       use_imgui);

r_external const r_b8
r_win32::window_show(r_void) {

    if (!r_win32_internal::window_initialized()) {
        return(false);
    }

    const HWND window_handle = r_win32_internal::window_get_window_handle();

    return((r_b8)ShowWindow(window_handle,1));
}

r_external const r_b8
r_win32::window_hide(r_void) {

    if (!r_win32_internal::window_initialized()) {
        return(false);
    }

    const HWND window_handle = r_win32_internal::window_get_window_handle();

    return((r_b8)ShowWindow(window_handle,0));
}

r_external r_b8 
r_win32::window_dimensions(RWin32WindowDimensions& dimensions) {

    if (!r_win32_internal::window_initialized()) {
        return(false);
    }

    RWin32Window& window = r_win32_internal::context_get_window();
    dimensions.position_x = window.position_x;
    dimensions.position_y = window.position_y;
    dimensions.width      = window.width;
    dimensions.height     = window.height;    

    return(true);
}

r_external const r_b8
r_win32::window_frame_start(
    r_void) {

    //set the frame start ticks
    const r_u64 ticks_frame_start = r_win32::system_ticks();
    r_win32_internal::window_set_frame_system_ticks_start(ticks_frame_start);

    //get the window message
    MSG& window_message = r_win32_internal::window_get_message();

    //process events
    while(PeekMessage(&window_message,0,0,0,PM_REMOVE)) {
        TranslateMessage(&window_message);
        DispatchMessage(&window_message);

        if (window_message.message == WM_QUIT) {
            r_win32_internal::window_set_quit_received(true);
        }
    }
    
    //clear the screen
    r_win32::rendering_clear();

    //check if we received a quit event
    const r_b32 quit_received = r_win32_internal::window_get_quit_received();

    return(quit_received ? false : true);
}

r_external const r_b8
r_win32::window_frame_render(
    r_void) {

    //swap the buffers
    const HDC device_context = r_win32_internal::window_get_device_context_handle();
    SwapBuffers(device_context);

    //set the frame render ticks
    const r_u64 ticks_frame_render = r_win32::system_ticks();
    r_win32_internal::window_set_frame_system_ticks_render(ticks_frame_render);

    //check to see if we have a quit event
    const r_b8 quit_received = r_win32_internal::window_get_quit_received();

    return(quit_received ? false : true);
}

r_external const r_timems
r_win32::window_frame_delta_time_ms(
    r_void) {

    //get ticks and frequency 
    const r_u64 ticks_frame_start  = r_win32_internal::window_get_frame_system_ticks_start(); 
    const r_u64 ticks_frame_render = r_win32_internal::window_get_frame_system_ticks_render(); 
    const r_u64 system_frequency   = r_win32::system_frequency();

    //convert the difference to milliseconds
    const r_u64    ticks_elapsed      = ticks_frame_render - ticks_frame_start;
    const r_u64    delta_time_seconds = ticks_elapsed / system_frequency;
    const r_timems delta_time_ms      = delta_time_seconds * 1000;

    //return the delta time
    return(delta_time_ms);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal LRESULT 
r_win32_internal::window_on_wm_size(
    WPARAM w_param, 
    LPARAM l_param) {

    //get the window dimensions from the arguments
    const r_u32 width  = LOWORD(l_param);
    const r_u32 height = HIWORD(l_param);

    //update the window dimensions
    r_win32_internal::window_set_width(width);
    r_win32_internal::window_set_height(height);

    //update the rendering viewport
    r_win32::rendering_update_viewport_dimensions(
        0,
        0,
        width,
        height);

    return(S_OK);
}

r_internal LRESULT 
r_win32_internal::window_on_wm_quit(
    WPARAM w_param, 
    LPARAM l_param) {

    r_win32_internal::window_set_quit_received(true);

    return(S_OK);
}

r_internal LRESULT 
r_win32_internal::window_on_wm_destroy(
    WPARAM w_param, 
    LPARAM l_param) {

    PostQuitMessage(0);
    
    return(S_OK);
}

r_internal LRESULT 
r_win32_internal::window_on_wm_move(
    WPARAM w_param, 
    LPARAM l_param) {

    const r_u32 position_x = LOWORD(l_param);
    const r_u32 position_y = HIWORD(l_param);

    r_win32_internal::window_set_position_x(position_x);
    r_win32_internal::window_set_position_y(position_y);

    return(S_OK);
}

r_internal LRESULT CALLBACK
r_win32_internal::window_callback(
    HWND   window_handle,
    UINT   message,
    WPARAM w_param,
    LPARAM l_param) {

    r_win32_funcptr_window_on_wm_message wm_message_handler = NULL;

    switch (message) {

        case WM_SIZE:    wm_message_handler = window_on_wm_size;    break; 
        case WM_MOVE:    wm_message_handler = window_on_wm_move;    break; 
        case WM_QUIT:    wm_message_handler = window_on_wm_quit;    break; 
        case WM_DESTROY: wm_message_handler = window_on_wm_destroy; break; 
    }

    LRESULT wm_message_result =
        wm_message_handler
            ? wm_message_handler(w_param,l_param)
            : DefWindowProc(
                window_handle,
                message,
                w_param,
                l_param);

    return(wm_message_result);
}