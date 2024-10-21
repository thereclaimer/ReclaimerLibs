#ifndef R_WIN32_IMGUI_HPP
#define R_WIN32_IMGUI_HPP

#include <r-common.hpp>
#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

typedef r_handle RWin32WindowHandle;

namespace r_win32 {

    r_external ImGuiContext* imgui_create_context(const RWin32WindowHandle window_handle);
};

#endif //R_WIN32_IMGUI_HPP