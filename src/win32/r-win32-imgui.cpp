#pragma once

#include "r-win32-imgui.hpp"
#include "r-win32-internal-window.hpp"

r_external ImGuiContext* 
r_win32::imgui_create_context(
    const RWin32WindowHandle window_handle) {

    if (!window_handle) {
        return(false);
    }

    RWin32Window* window_ptr = (RWin32Window*)window_handle;    

    const HWND win32_window = window_ptr->win32_handle_window;


    //initialize imgui
    IMGUI_CHECKVERSION();
    ImGuiContext* imgui_context = ImGui::CreateContext();

    bool result = imgui_context != NULL;

    //initialize win32/opengl implementations for imgui
    result &= ImGui_ImplWin32_Init(win32_window);
    result &= ImGui_ImplOpenGL3_Init();

    if (!result) {
        ImGui::DestroyContext(imgui_context);
        imgui_context = NULL;
    }

    //set the imgui context in the window
    window_ptr->imgui_context = imgui_context;

    return(imgui_context);
}
