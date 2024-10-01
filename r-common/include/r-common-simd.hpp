#ifndef R_COMMON_SIMD_HPP
#define R_COMMON_SIMD_HPP

#include "r-common-types.hpp"

#if _MSC_VER
    #if !defined(__clang__)
        #define INSTRUCTION_REORDER_BARRIER _ReadWriteBarrier()
    #endif //!defined(__clang__)
    #include <intrin.h>
#else //_MSC_VER
    #include <x86intrin.h>
#endif //_MSC_VER

//integers u32[4]
typedef __m128i r_simd_4_u32;

#define r_simd_4_u32_load_aligned(u32_ptr)            _mm_load_si128((__m128i*)u32_ptr)
#define r_simd_4_u32_load_unaligned(u32_ptr)          _mm_loadu_si128((__m128i*)u32_ptr)
#define r_simd_4_u32_add(u32_simd_a, u32_simd_b)      _mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_simd_4_u32_subtract(u32_simd_a, u32_simd_b) _mm_sub_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_simd_4_u32_multiply(u32_simd_a, u32_simd_b) _mm_mul_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_simd_4_u32_divide(u32_simd_a, u32_simd_b)   _mm_div_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_simd_4_u32_store(u32_ptr, u32_simd_a)       _mm_storeu_epi32(u32_ptr, u32_simd_a)

//floats f32[4]
typedef __m128 r_simd_4_f32;

#define r_simd_4_f32_load_aligned(f32_4_ptr)          _mm_load_ps(f32_4_ptr)
#define r_simd_4_f32_load_unaligned(f32_4_ptr)        _mm_loadu_ps(f32_4_ptr)
#define r_simd_4_f32_add(f32_simd_a, f32_simd_b)      _mm_add_ps(f32_4_a, f32_4_b)
#define r_simd_4_f32_subtract(f32_simd_a, f32_simd_b) _mm_sub_ps(f32_4_a, f32_4_b)
#define r_simd_4_f32_multiply(f32_simd_a, f32_simd_b) _mm_mul_ps(f32_4_a, f32_4_b)
#define r_simd_4_f32_divide(f32_simd_a, f32_simd_b)   _mm_div_ps(f32_4_a, f32_4_b)
#define r_simd_4_f32_store(f32_4_ptr, f32_simd)       _mm_store_ps(f32_4_ptr, f32_simd)

struct RSimdXMMRegisters_32_u32 {
    r_simd_4_u32 xmm_0;
    r_simd_4_u32 xmm_1;
    r_simd_4_u32 xmm_2;
    r_simd_4_u32 xmm_3;
    r_simd_4_u32 xmm_4;
    r_simd_4_u32 xmm_5;
    r_simd_4_u32 xmm_6;
    r_simd_4_u32 xmm_7;
};

struct RSimdXMMRegisters_32_f32 {
    r_simd_4_f32 xmm_0;
    r_simd_4_f32 xmm_1;
    r_simd_4_f32 xmm_2;
    r_simd_4_f32 xmm_3;
    r_simd_4_f32 xmm_4;
    r_simd_4_f32 xmm_5;
    r_simd_4_f32 xmm_6;
    r_simd_4_f32 xmm_7;
};

namespace r_simd {

    inline r_void 
    xmm_pack_32_u32(
        r_u32*                    in_f32_ptr,
        RSimdXMMRegisters_32_u32* out_registers) {

        out_registers->xmm_0 = r_simd_4_u32_load_aligned(in_u32_ptr +  0);
        out_registers->xmm_1 = r_simd_4_u32_load_aligned(in_u32_ptr +  4);
        out_registers->xmm_2 = r_simd_4_u32_load_aligned(in_u32_ptr +  8);
        out_registers->xmm_3 = r_simd_4_u32_load_aligned(in_u32_ptr + 12);
        out_registers->xmm_4 = r_simd_4_u32_load_aligned(in_u32_ptr + 16);
        out_registers->xmm_5 = r_simd_4_u32_load_aligned(in_u32_ptr + 20);
        out_registers->xmm_6 = r_simd_4_u32_load_aligned(in_u32_ptr + 24);
        out_registers->xmm_7 = r_simd_4_u32_load_aligned(in_u32_ptr + 28);
    }

    inline r_void 
    xmm_pack_32_f32(
        r_f32*                    in_f32_ptr,
        RSimdXMMRegisters_32_f32* out_registers) {

        out_registers->xmm_0 = r_simd_4_u32_load_aligned(in_f32_ptr +  0);
        out_registers->xmm_1 = r_simd_4_u32_load_aligned(in_f32_ptr +  4);
        out_registers->xmm_2 = r_simd_4_u32_load_aligned(in_f32_ptr +  8);
        out_registers->xmm_3 = r_simd_4_u32_load_aligned(in_f32_ptr + 12);
        out_registers->xmm_4 = r_simd_4_u32_load_aligned(in_f32_ptr + 16);
        out_registers->xmm_5 = r_simd_4_u32_load_aligned(in_f32_ptr + 20);
        out_registers->xmm_6 = r_simd_4_u32_load_aligned(in_f32_ptr + 24);
        out_registers->xmm_7 = r_simd_4_u32_load_aligned(in_f32_ptr + 28);
    }

};


#endif //R_COMMON_SIMD_HPP