#pragma once

#include "r-win32-internal-context.hpp"

/**********************************************************************************/
/* EXTERNAL                                                                       */
/**********************************************************************************/

r_external const RWin32WindowHandle
r_win32::window_create(
    const r_cstr                     title,
    const r_u32                      position_x,
    const r_u32                      position_y,
    const r_u32                      width,
    const r_u32                      height,
    const RColorFormat               color_format,
    const RWin32WindowUseImGui       use_imgui) {

    //check we are initialized
    if (!r_win32_internal::context_is_initialized()) {
        return(false);
    }

    //get an arena
    RWin32Window* window_ptr = r_win32_internal::context_window_commit();
    if (!window_ptr) {
        return(NULL); 
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
    window_ptr->win32_handle_window          = window_handle;
    window_ptr->win32_handle_device_context  = device_context;
    window_ptr->position_x                   = position_x;
    window_ptr->position_y                   = position_y;
    window_ptr->width                        = width;
    window_ptr->height                       = height;
    window_ptr->color_format                 = color_format; 
    window_ptr->rendering_context_ptr        = NULL;

    //we're done
    return(window_ptr);
}

r_external const RWin32WindowHandle
r_win32::window_create_centered(
    const r_cstr                     title,
    const r_u32                      width,
    const r_u32                      height,
    const RColorFormat               color_format,
    const RWin32WindowUseImGui       use_imgui) {

    //get the center of the screen
    const r_s32 screen_width  = GetSystemMetrics(SM_CXSCREEN);
    const r_s32 screen_height = GetSystemMetrics(SM_CYSCREEN);

    //calculate the offsets
    const r_s32 screen_offset_x = (screen_width  - width)  / 2;
    const r_s32 screen_offset_y = (screen_height - height) / 2;

    //pass the center coordinates to the main window creation function
    const RWin32WindowHandle window_handle =
        r_win32::window_create(
            title,
            screen_offset_x,
            screen_offset_y,
            width,
            height,
            color_format,
            use_imgui);

    return(window_handle);
}

r_external const r_b8
r_win32::window_show(const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;

    r_win32_internal::context_set_current_window_ptr(window_ptr);

    return((r_b8)ShowWindow(window_ptr->win32_handle_window,1));
}

r_external const r_b8
r_win32::window_hide(const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    const RWin32Window* window_ptr    = (RWin32Window*)window_handle;
    const HWND          window_win32 = window_ptr->win32_handle_window;

    return((r_b8)ShowWindow(window_win32,0));
}

r_external r_b8 
r_win32::window_dimensions(
    const RWin32WindowHandle          in_window_handle,
          RWin32WindowDimensions& out_window_dimensions_ref) {

    if (!in_window_handle) {
        return(false);
    }

    const RWin32Window* window_ptr = (RWin32Window*)in_window_handle;
    out_window_dimensions_ref.position_x = window_ptr->position_x;
    out_window_dimensions_ref.position_y = window_ptr->position_y;
    out_window_dimensions_ref.width      = window_ptr->width;
    out_window_dimensions_ref.height     = window_ptr->height;    

    return(true);
}

r_external const r_b8
r_win32::window_frame_start(
    const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;

    //set the current window
    r_win32_internal::context_set_current_window_ptr(window_ptr);

    //set the frame start ticks
    const r_u64 ticks_frame_start = r_win32::system_ticks();

    //if we have an imgui context, start a new frame
    if (window_ptr->imgui_context) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    //get the window message
    MSG& window_message = window_ptr->message; 

    //process events
    while(PeekMessage(&window_message,0,0,0,PM_REMOVE)) {
        TranslateMessage(&window_message);
        DispatchMessage(&window_message);

        if (window_message.message == WM_QUIT) {
            window_ptr->quit_received = true;
        }
    }
    
    //clear the screen
    r_win32::rendering_clear(window_ptr->rendering_context_ptr);

    //check if we received a quit event
    return(window_ptr->quit_received ? false : true);
}

r_external const r_b8
r_win32::window_frame_render(
    const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;

    //if we have an imgui context, render the draw data
    if (window_ptr->imgui_context) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    //swap the buffers
    SwapBuffers(window_ptr->win32_handle_device_context);

    //set the frame render ticks
    window_ptr->frame_system_ticks_render = r_win32::system_ticks();

    //return quit event status
    return(window_ptr->quit_received ? false : true);
}

r_external const r_timems
r_win32::window_frame_delta_time_ms(
    const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;

    //get ticks and frequency 
    const r_u64 ticks_frame_start  = window_ptr->frame_system_ticks_start; 
    const r_u64 ticks_frame_render = window_ptr->frame_system_ticks_render;
    const r_u64 system_frequency   = r_win32::system_frequency();

    //convert the difference to milliseconds
    const r_u64    ticks_elapsed      = ticks_frame_render - ticks_frame_start;
    const r_u64    delta_time_seconds = ticks_elapsed / system_frequency;
    const r_timems delta_time_ms      = delta_time_seconds * 1000;

    //return the delta time
    return(delta_time_ms);
}

r_external const HWND 
r_win32::window_win32_handle(
    const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;

    return(window_ptr->win32_handle_window);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

r_internal LRESULT 
r_win32_internal::window_on_wm_size(
    WPARAM w_param, 
    LPARAM l_param) {

    //get the current window
    RWin32Window* current_window_ptr = r_win32_internal::context_get_current_window_ptr();

    //get the window dimensions from the arguments
    const r_u32 width  = LOWORD(l_param);
    const r_u32 height = HIWORD(l_param);

    //update the window dimensions
    current_window_ptr->width  = width;
    current_window_ptr->height = height;

    //update the rendering viewport
    r_win32::rendering_update_viewport_dimensions(
        current_window_ptr->rendering_context_ptr,
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

    RWin32Window* current_window_ptr = r_win32_internal::context_get_current_window_ptr();

    current_window_ptr->quit_received = true;

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

    RWin32Window* current_window_ptr = r_win32_internal::context_get_current_window_ptr();

    current_window_ptr->position_x = LOWORD(l_param);
    current_window_ptr->position_y = HIWORD(l_param);

    return(S_OK);
}

r_internal LRESULT CALLBACK
r_win32_internal::window_callback(
    HWND   window_handle,
    UINT   message,
    WPARAM w_param,
    LPARAM l_param) {

    r_win32_funcptr_window_on_wm_message wm_message_handler = NULL;

    RWin32Window* current_window_ptr = r_win32_internal::context_get_current_window_ptr();
    if (current_window_ptr && current_window_ptr->imgui_context) {
        
        const LRESULT imgui_result =
            ImGui_ImplWin32_WndProcHandler(
                window_handle,
                message,
                w_param,
                l_param);

        if (imgui_result) {
            return(true);
        }
    }

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