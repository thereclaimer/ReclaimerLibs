#ifndef R_WIN32_MAIN_HPP
#define R_WIN32_MAIN_HPP

#include <Windows.h>

struct RWin32MainArgs {
    HINSTANCE h_instance;
    HINSTANCE h_prev_instance;
    PWSTR     p_cmd_line;
    int       n_cmd_show;
};

r_s32 r_win32_main(RWin32MainArgs& r_win32_args);

#ifdef  R_WIN32_MAIN_REDIRECT

int WINAPI 
wWinMain(
    HINSTANCE h_instance,
    HINSTANCE h_prev_instance,
    PWSTR     p_cmd_line,
    int       n_cmd_show) {

    RWin32MainArgs r_win32_args;
    r_win32_args.h_instance      = h_instance; 
    r_win32_args.h_prev_instance = h_prev_instance; 
    r_win32_args.p_cmd_line      = p_cmd_line; 
    r_win32_args.n_cmd_show      = n_cmd_show; 

    r_s32 result = r_win32_main(r_win32_args);

    return(result);
}

#endif //R_WIN32_MAIN_REDIRECT

#endif //R_WIN32_MAIN_HPP