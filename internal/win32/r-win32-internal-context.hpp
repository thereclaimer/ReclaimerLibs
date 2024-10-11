#ifndef R_WIN32_INTERNAL_HPP
#define R_WIN32_INTERNAL_HPP

#include <r-memory.hpp>

#include "r-win32.hpp"
#include "r-win32-internal-window.hpp"
#include "r-win32-internal-rendering.hpp"
#include "r-win32-internal-file.hpp"
#include "r-win32-internal-system.hpp"

struct RWin32Context {
    RWin32Window*      current_window_ptr;
    RMemoryRegionHandle   region;
    RWin32SystemInfo   system_info;
    RWin32MainArgs     args;
    RWin32FileManager  file_manager;
};

r_global RWin32Context _r_win32_context;

namespace r_win32_internal {

    //---------------------------
    // context
    //---------------------------

    inline const r_b8 context_is_initialized(r_void) { return(_r_win32_context.system_info.time_initialized > 0); } 
    
    inline       RWin32SystemInfo&  context_get_system_info        (r_void) { return(_r_win32_context.system_info);        }
    inline       RWin32MainArgs&    context_get_args               (r_void) { return(_r_win32_context.args);               } 
    inline       RWin32Window*      context_get_current_window_ptr (r_void) { return(_r_win32_context.current_window_ptr); }
    inline       RWin32FileManager& context_get_file_manager       (r_void) { return(_r_win32_context.file_manager);       }
    
    inline r_void context_set_current_window_ptr (RWin32Window* current_window_ptr) { _r_win32_context.current_window_ptr = current_window_ptr; }

    inline const RMemoryArenaHandle
    context_arena_commit(
        r_void) {
        
        return(r_mem::arena_commit(_r_win32_context.region));
    }
    
    inline const r_b8
    context_arena_decommit(
        const RMemoryArenaHandle arena_handle) {

        return(r_mem::arena_decommit(arena_handle));
    }

    r_internal RWin32RenderingContext* context_rendering_context_commit   (r_void);
    r_internal RWin32Window*           context_window_commit              (r_void);
    r_internal RWin32FileTable*        context_file_table_commit          (r_void);

    r_internal const r_b8 context_window_decommit            (RWin32Window* window_ptr);
    r_internal const r_b8 context_rendering_context_decommit (RWin32RenderingContext* rendering_context_ptr);

    //---------------------------
    // args
    //---------------------------
    
    inline const HINSTANCE main_args_get_h_instance      (r_void) { return(_r_win32_context.args.h_instance);      }
    inline const HINSTANCE main_args_get_h_prev_instance (r_void) { return(_r_win32_context.args.h_prev_instance); }
    inline const PWSTR     main_args_get_p_cmd_line      (r_void) { return(_r_win32_context.args.p_cmd_line);      }
    inline const int       main_args_get_n_cmd_show      (r_void) { return(_r_win32_context.args.n_cmd_show);      }    

};

#endif //R_WIN32_INTERNAL_HPP