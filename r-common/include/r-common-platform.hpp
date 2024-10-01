#ifndef R_COMMON_PLATFORM_HPP
#define R_COMMON_PLATFORM_HPP

#include "r-common-types.hpp"

/**********************************************************************************/
/* PLATFORM INFO                                                                  */
/**********************************************************************************/

struct RPlatformInfo {
    r_size physical_memory_size;
    r_size page_size;
    r_size large_page_size;
    r_size allocation_granularity;
    r_size processor_count;
};

typedef r_b8
(*r_funcptr_platform_get_info) (
    RPlatformInfo& platform_info);

/**********************************************************************************/
/* PLATFORM MEMORY                                                                */
/**********************************************************************************/

typedef r_memory
(*r_funcptr_platform_memory_reserve) (
    const r_memory start,
    const r_size   size);

typedef r_memory
(*r_funcptr_platform_memory_commit) (
    const r_memory start,
    const r_size   size);

typedef r_memory
(*r_funcptr_platform_memory_reserve_and_commit) (
    const r_memory start,
    const r_size   size);

typedef r_b8
(*r_funcptr_platform_memory_decommit) (
    const r_memory start,
    const r_size   size);

typedef r_b8
(*r_funcptr_platform_memory_release) (
    const r_memory start,
    const r_size   size);

struct RPlatformMemoryApi {
    r_funcptr_platform_memory_reserve            reserve;
    r_funcptr_platform_memory_commit             commit;
    r_funcptr_platform_memory_reserve_and_commit reserve_and_commit;
    r_funcptr_platform_memory_decommit           decommit;
    r_funcptr_platform_memory_release            release;
};

#endif //R_COMMON_PLATFORM_HPP