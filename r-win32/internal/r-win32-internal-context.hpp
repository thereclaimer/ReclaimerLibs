#ifndef R_WIN32_INTERNAL_HPP
#define R_WIN32_INTERNAL_HPP

#include "r-win32.hpp"
#include "r-win32-internal-window.hpp"
#include "r-win32-internal-rendering.hpp"
#include "r-win32-internal-file.hpp"
#include "r-win32-internal-system.hpp"

struct RWin32ContextStack {
    r_memory  start;
    r_size    size;
    r_address position;
};

struct RWin32Context {
    RWin32ContextStack stack;
    RWin32SystemInfo   system_info;
    RWin32MainArgs     args;
    RWin32Window       window;
    RWin32FileTable    file_table;     
};

r_global RWin32Context _r_win32_context;

namespace r_win32_internal {

    //---------------------------
    // context
    //---------------------------

    inline const r_b8 context_is_initialized(r_void) { return(_r_win32_context.system_info.time_initialized > 0); } 
    
    inline       RWin32SystemInfo&  context_get_system_info (r_void) { return(_r_win32_context.system_info);             }
    inline       RWin32MainArgs&    context_get_args        (r_void) { return(_r_win32_context.args);                    } 
    inline       RWin32Window&      context_get_window      (r_void) { return(_r_win32_context.window);                  }
    inline       RWin32FileTable&   context_get_file_table  (r_void) { return(_r_win32_context.file_table);            }
    
    r_internal const r_b8     context_stack_can_push     (const r_size size);
    r_internal       r_memory context_stack_push         (const r_size size);
    r_internal       r_memory context_stack_push_aligned (const r_size size, const r_size alignment);

    //---------------------------
    // args
    //---------------------------
    
    inline const HINSTANCE main_args_get_h_instance      (r_void) { return(_r_win32_context.args.h_instance);      }
    inline const HINSTANCE main_args_get_h_prev_instance (r_void) { return(_r_win32_context.args.h_prev_instance); }
    inline const PWSTR     main_args_get_p_cmd_line      (r_void) { return(_r_win32_context.args.p_cmd_line);      }
    inline const int       main_args_get_n_cmd_show      (r_void) { return(_r_win32_context.args.n_cmd_show);      }    

    //---------------------------
    // window
    //---------------------------

    inline const r_b8
    window_initialized() {

        const r_b8 result = (
            _r_win32_context.system_info.time_initialized > 0 &&
            _r_win32_context.window.win32_handle_window != NULL);
    
        return(result);
    }

    inline const HWND              window_get_window_handle             (r_void) { return(_r_win32_context.window.win32_handle_window);         }
    inline const HDC               window_get_device_context_handle     (r_void) { return(_r_win32_context.window.win32_handle_device_context); }
    inline       MSG&              window_get_message                   (r_void) { return(_r_win32_context.window.message);                     }
    inline const r_u32             window_get_position_x                (r_void) { return(_r_win32_context.window.position_x);                  }
    inline const r_u32             window_get_position_y                (r_void) { return(_r_win32_context.window.position_y);                  }
    inline const r_u32             window_get_width                     (r_void) { return(_r_win32_context.window.width);                       }
    inline const r_u32             window_get_height                    (r_void) { return(_r_win32_context.window.height);                      }
    inline const r_b32             window_get_quit_received             (r_void) { return(_r_win32_context.window.quit_received);               }
    inline const r_u64             window_get_frame_system_ticks_start  (r_void) { return(_r_win32_context.window.frame_system_ticks_start);    }
    inline const r_u64             window_get_frame_system_ticks_render (r_void) { return(_r_win32_context.window.frame_system_ticks_render);   }
    inline const RColorFormat      window_get_color_format              (r_void) { return(_r_win32_context.window.color_format);                }
    inline RWin32RenderingContext& window_get_rendering_context         (r_void) { return(_r_win32_context.window.rendering_context);           }

    inline r_void window_set_position_x                (const r_u32 position_x)          { _r_win32_context.window.position_x                = position_x;    }
    inline r_void window_set_position_y                (const r_u32 position_y)          { _r_win32_context.window.position_y                = position_y;    }
    inline r_void window_set_width                     (const r_u32 width)               { _r_win32_context.window.width                     = width;         }
    inline r_void window_set_height                    (const r_u32 height)              { _r_win32_context.window.height                    = height;        }
    inline r_void window_set_quit_received             (const r_b32 quit_received)       { _r_win32_context.window.quit_received             = quit_received; }
    inline r_void window_set_color_format              (const RColorFormat color_format) { _r_win32_context.window.color_format              = color_format;  }
    inline r_void window_set_frame_system_ticks_start  (const r_u64 ticks_start)         { _r_win32_context.window.frame_system_ticks_start  = ticks_start;   }    
    inline r_void window_set_frame_system_ticks_render (const r_u64 ticks_render)        { _r_win32_context.window.frame_system_ticks_render = ticks_render;  }

    //---------------------------
    // file
    //---------------------------

};

#endif //R_WIN32_INTERNAL_HPP