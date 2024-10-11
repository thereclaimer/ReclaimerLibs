#ifndef R_DATA_STRUCTURES_HASH_INTERNAL_HPP
#define R_DATA_STRUCTURES_HASH_INTERNAL_HPP

#include "r-data-structures-hash.hpp"

struct RHashTable {
    RMemoryArenaHandle arena_handle;
    r_cstr             string_buffer;
    r_size             string_buffer_length;
    r_size             row_count;
    struct {
        RHash128* value_128;
        r_size*   string_start;
    } columns;
};

#endif //R_DATA_STRUCTURES_HASH_INTERNAL_HPP