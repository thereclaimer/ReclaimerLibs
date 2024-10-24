#ifndef R_COMMON_TYPES_HPP
#define R_COMMON_TYPES_HPP

#include <cstdint>

//signed integers
typedef int8_t  r_s8;
typedef int16_t r_s16;
typedef int32_t r_s32;
typedef int64_t r_s64;

//unsigned integers
typedef uint8_t  r_u8;
typedef uint16_t r_u16;
typedef uint32_t r_u32;
typedef uint64_t r_u64;

//floats
typedef float  r_f32;
typedef double r_f64;

//booleans
typedef int8_t  r_b8;
typedef int16_t r_b16;
typedef int32_t r_b32;
typedef int64_t r_b64;

//strings
typedef char     r_char;
typedef wchar_t  r_wchar;
typedef char*    r_cstr;
typedef wchar_t* r_wstr;

//memory
typedef void     r_void;
typedef void*    r_handle;
typedef uint8_t  r_byte;
typedef uint8_t* r_memory;
typedef size_t   r_size;
typedef size_t   r_index;
typedef intptr_t r_address;

#define R_INVALID_INDEX SIZE_MAX

//time
typedef uint64_t r_timems;

#endif //R_COMMON_TYPES_HPP