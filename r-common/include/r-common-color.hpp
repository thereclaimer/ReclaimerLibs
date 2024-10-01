#ifndef R_COMMON_COLOR_HPP
#define R_COMMON_COLOR_HPP

#include "r-common-types.hpp"

enum RColorFormat_ {
    RColorFormat_RGBA = 0,
    RColorFormat_ARGB = 1,
    RColorFormat_ABGR = 2,
    RColorFormat_BGRA = 3
};

typedef r_u32 RColorFormat; 

struct RColorRGBA {
    r_u8 a;
    r_u8 b;
    r_u8 g;
    r_u8 r;
};

struct RColorARGB {
    r_u8 b;
    r_u8 g;
    r_u8 r;
    r_u8 a;
};

struct RColorABGR {
    r_u8 r;
    r_u8 g;
    r_u8 b;
    r_u8 a;
};

struct RColorBGRA {
    r_u8 a;
    r_u8 r;
    r_u8 g;
    r_u8 b;
};

struct RColor32Bit {
    RColorFormat format;
    union {
        r_u32      hex;
        RColorRGBA rgba;
        RColorARGB argb;
        RColorABGR abgr;
        RColorBGRA bgra;
    };
};

struct RColor32BitNormalized {
    r_f32 r;
    r_f32 g;
    r_f32 b;
    r_f32 a;
};

namespace r_common {

    inline r_void
    color_32_bit_normalize(
        RColor32Bit&           in_color_32,
        RColor32BitNormalized& out_color_32_normalized) {

        switch (in_color_32.format) {
        
            case RColorFormat_RGBA: {
                out_color_32_normalized.r = (r_f32)in_color_32.rgba.r / 255.0f; 
                out_color_32_normalized.g = (r_f32)in_color_32.rgba.g / 255.0f;
                out_color_32_normalized.b = (r_f32)in_color_32.rgba.b / 255.0f;
                out_color_32_normalized.a = (r_f32)in_color_32.rgba.a / 255.0f;
            } break;

            case RColorFormat_ARGB: {
                out_color_32_normalized.r = (r_f32)in_color_32.argb.r / 255.0f; 
                out_color_32_normalized.g = (r_f32)in_color_32.argb.g / 255.0f;
                out_color_32_normalized.b = (r_f32)in_color_32.argb.b / 255.0f;
                out_color_32_normalized.a = (r_f32)in_color_32.argb.a / 255.0f;
            } break;

            case RColorFormat_ABGR: {
                out_color_32_normalized.r = (r_f32)in_color_32.abgr.r / 255.0f; 
                out_color_32_normalized.g = (r_f32)in_color_32.abgr.g / 255.0f;
                out_color_32_normalized.b = (r_f32)in_color_32.abgr.b / 255.0f;
                out_color_32_normalized.a = (r_f32)in_color_32.abgr.a / 255.0f;
            } break;

            case RColorFormat_BGRA: {
                out_color_32_normalized.r = (r_f32)in_color_32.bgra.r / 255.0f; 
                out_color_32_normalized.g = (r_f32)in_color_32.bgra.g / 255.0f;
                out_color_32_normalized.b = (r_f32)in_color_32.bgra.b / 255.0f;
                out_color_32_normalized.a = (r_f32)in_color_32.bgra.a / 255.0f;
            } break;
        }
    }

    inline RColor32Bit 
    color_32_bit_create(
        const RColorFormat color_format,
        const r_byte       red,
        const r_byte       green,
        const r_byte       blue,
        const r_byte       alpha) {

        RColor32Bit color_32;
        color_32.format = color_format;

        switch (color_32.format) {

            case RColorFormat_RGBA: {

                color_32.rgba.r = red;
                color_32.rgba.g = green;
                color_32.rgba.b = blue;
                color_32.rgba.a = alpha;

            } break;

            case RColorFormat_ARGB: {

                color_32.argb.r = red;
                color_32.argb.g = green;
                color_32.argb.b = blue;
                color_32.argb.a = alpha;

            } break;

            case RColorFormat_ABGR: {

                color_32.abgr.r = red;
                color_32.abgr.g = green;
                color_32.abgr.b = blue;
                color_32.abgr.a = alpha;

            } break;

            case RColorFormat_BGRA: {

                color_32.bgra.r = red;
                color_32.bgra.g = green;
                color_32.bgra.b = blue;
                color_32.bgra.a = alpha;

            }  break;
        }

        return(color_32);
    }
};

#endif //R_COMMON_COLOR_HPP