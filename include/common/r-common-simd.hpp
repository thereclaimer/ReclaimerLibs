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
typedef __m128i r_u128_xmm;

#define r_u128_xmm_load_aligned(u32_ptr)            _mm_load_si128((__m128i*)u32_ptr)
#define r_u128_xmm_load_unaligned(u32_ptr)          _mm_loadu_si128((__m128i*)u32_ptr)
#define r_u128_xmm_add(u32_simd_a, u32_simd_b)      _mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_u128_xmm_subtract(u32_simd_a, u32_simd_b) _mm_sub_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_u128_xmm_multiply(u32_simd_a, u32_simd_b) _mm_mul_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_u128_xmm_divide(u32_simd_a, u32_simd_b)   _mm_div_epi_mm_add_epi32(u32_simd_a, u32_simd_b)
#define r_u128_xmm_store(u32_ptr, u32_simd_a)       _mm_storeu_epi32(u32_ptr, u32_simd_a)

//floats f32[4]
typedef __m128 r_f128_xmm;

#define r_f128_xmm_load_aligned(f32_4_ptr)          _mm_load_ps(f32_4_ptr)
#define r_f128_xmm_load_unaligned(f32_4_ptr)        _mm_loadu_ps(f32_4_ptr)
#define r_f128_xmm_add(f32_simd_a, f32_simd_b)      _mm_add_ps(f32_4_a, f32_4_b)
#define r_f128_xmm_subtract(f32_simd_a, f32_simd_b) _mm_sub_ps(f32_4_a, f32_4_b)
#define r_f128_xmm_multiply(f32_simd_a, f32_simd_b) _mm_mul_ps(f32_4_a, f32_4_b)
#define r_f128_xmm_divide(f32_simd_a, f32_simd_b)   _mm_div_ps(f32_4_a, f32_4_b)
#define r_f128_xmm_store(f32_4_ptr, f32_simd)       _mm_store_ps(f32_4_ptr, f32_simd)

#endif //R_COMMON_SIMD_HPP