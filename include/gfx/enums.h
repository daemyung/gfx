//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#ifndef GFX_ENUMS_GUARD
#define GFX_ENUMS_GUARD

#include <cstdint>

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

enum class Vender : uint32_t {
    apple = 0x106B, amd = 0x1022, arm = 0x13B5, nvidia = 0x10DE, qualcomm = 0x5143,
};

//----------------------------------------------------------------------------------------------------------------------

enum class Format : uint32_t {
    invalid = 0,
    rgb8_unorm, rgba8_unorm, bgra8_unorm, r32_float, rg32_float, rgb32_float, rgba32_float, d24_unorm_s8_uint
};

//----------------------------------------------------------------------------------------------------------------------

enum class Heap_type : uint8_t {
    local = 0, upload, readback
};

//----------------------------------------------------------------------------------------------------------------------

enum class Index_type : uint8_t {
    invalid = 0, uint16, uint32
};

//----------------------------------------------------------------------------------------------------------------------

enum class Image_type : uint8_t {
    two_dim = 0, cube, swap_chain
};

//----------------------------------------------------------------------------------------------------------------------

enum class Filter : uint8_t {
    nearest = 0, linear
};

//----------------------------------------------------------------------------------------------------------------------

enum class Mip_filter : uint8_t {
    nearest = 0, linear
};

//----------------------------------------------------------------------------------------------------------------------

enum class Address_mode : uint8_t {
    repeat = 0, clamp_to_edge
};

//----------------------------------------------------------------------------------------------------------------------

enum class Pipeline_stage : uint8_t {
    vertex_shader = 0x01, fragment_shader = 0x02, output_merger = 0x04, transfer = 0x08
};

//----------------------------------------------------------------------------------------------------------------------

enum class Step_rate : uint8_t {
    vertex = 0, instance
};

//----------------------------------------------------------------------------------------------------------------------

enum class Topology : uint8_t {
    triangle_list = 0, triangle_strip, point
};

//----------------------------------------------------------------------------------------------------------------------

enum class Cull_mode : uint8_t {
    none  = 0, front, back,
};

//----------------------------------------------------------------------------------------------------------------------

enum class Front_face : uint8_t {
    counter_clockwise = 0, clockwise
};

//----------------------------------------------------------------------------------------------------------------------

enum class Compare_op : uint8_t {
    never = 0, less, greater, equal, not_equal, less_or_equal, greater_or_equal, always
};

//----------------------------------------------------------------------------------------------------------------------

enum class Stencil_op : uint8_t {
    keep = 0, zero, replace, increment_and_clamp, decrement_and_clamp, invert, increment_and_wrap, decrement_and_wrap
};

//----------------------------------------------------------------------------------------------------------------------

enum class Blend_factor : uint8_t
{
    zero = 0, one, src_alpha, one_minus_src_alpha, dst_alpha, one_minus_dst_alpha
};

//----------------------------------------------------------------------------------------------------------------------

enum class Blend_op : uint8_t {
    add = 0, subtract, reverse_subtract, min, max
};

//----------------------------------------------------------------------------------------------------------------------

enum class Load_op : uint8_t {
    load = 0, clear, dont_care
};

//----------------------------------------------------------------------------------------------------------------------

enum class Store_op : uint8_t {
    store = 0, dont_care
};

//----------------------------------------------------------------------------------------------------------------------

enum class Color_space : uint8_t {
    srgb_non_linear = 0
};

//----------------------------------------------------------------------------------------------------------------------

} // of namespace Gfx_lib

#endif // GFX_ENUMS_GUARD
