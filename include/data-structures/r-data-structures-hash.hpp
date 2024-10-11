#ifndef R_DATA_STRUCTURES_HASH_HPP
#define R_DATA_STRUCTURES_HASH_HPP

#include "meow_hash_x64_aesni.h"

#include "r-common.hpp"
#include "r-memory.hpp"

struct RHash128 {
    union {
        r_u32 hash_val[4];
        struct {
            r_u32 hash_val_0;
            r_u32 hash_val_1;
            r_u32 hash_val_2;
            r_u32 hash_val_3;
        };
    };
};


namespace r_hash {

    r_external const r_b8 
    hash_string(
        const r_cstr     in_string,
              RHash128& out_hash);
    
    r_external const r_u128_xmm hash_xmm_load (RHash128& r_hash_128_ref);
};

typedef r_handle RHashTableHandle;
typedef r_index  RHashTableIndex;

namespace r_hash {

    r_external const RHashTableHandle 
    hash_table_commit(
        const RMemoryRegionHandle region_handle,
        const r_size              max_string_size);

    
    r_external const RHashTableIndex hash_table_add(const r_cstr string);
};

#endif //R_DATA_STRUCTURES_HASH_HPP