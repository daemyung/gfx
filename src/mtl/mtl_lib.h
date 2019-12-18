//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#ifndef GFX_MTL_LIB_GUARD
#define GFX_MTL_LIB_GUARD

#include <stdexcept>
#include <Metal/Metal.h>
#include "enums.h"
#include "types.h"

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

template<typename R, typename T>
inline R convert(T);

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLResourceOptions convert(Heap_type type)
{
    switch (type) {
        case Heap_type::local:
            return MTLResourceStorageModePrivate;
        case Heap_type::upload:
            return MTLResourceCPUCacheModeWriteCombined;
        case Heap_type::readback:
            return MTLResourceCPUCacheModeDefaultCache;
        default:
            throw std::runtime_error("invalid the heap type");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLTextureType convert(Image_type type)
{
    switch (type) {
        case Image_type::two_dim:
            return MTLTextureType2D;
        case Image_type::cube:
            return MTLTextureTypeCube;
        default:
            throw std::runtime_error("invalid the image type");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLPixelFormat convert(Format format)
{
    switch (format) {
        case Format::rgba8_unorm:
            return MTLPixelFormatRGBA8Unorm;
        case Format::bgra8_unorm:
            return MTLPixelFormatBGRA8Unorm;
        case Format::r32_float:
            return MTLPixelFormatR32Float;
        case Format::rg32_float:
            return MTLPixelFormatRG32Float;
        case Format::rgba32_float:
            return MTLPixelFormatRGBA32Float;
        case Format::d24_unorm_s8_uint:
            return MTLPixelFormatDepth32Float_Stencil8;
        default:
            throw std::runtime_error("invalid the format");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLSamplerMinMagFilter convert(Filter filter)
{
    switch (filter) {
        case Filter::nearest:
            return MTLSamplerMinMagFilterNearest;
        case Filter::linear:
            return MTLSamplerMinMagFilterLinear;
        default:
            throw std::runtime_error("invalid the filter");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLSamplerMipFilter convert(Mip_filter filter)
{
    switch (filter) {
        case Mip_filter::nearest:
            return MTLSamplerMipFilterNearest;
        case Mip_filter::linear:
            return MTLSamplerMipFilterLinear;
        default:
            throw std::runtime_error("invalid the filter");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLSamplerAddressMode convert(Address_mode mode)
{
    switch (mode) {
        case Address_mode::repeat:
            return MTLSamplerAddressModeRepeat;
        case Address_mode::clamp_to_edge:
            return MTLSamplerAddressModeClampToEdge;
        default:
            throw std::runtime_error("invalid the address mode");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLVertexStepFunction convert(Step_rate rate)
{
    switch (rate) {
        case Step_rate::vertex:
            return MTLVertexStepFunctionPerVertex;
        case Step_rate::instance:
            return MTLVertexStepFunctionPerInstance;
        default:
            throw std::runtime_error("invalid the step rate");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLVertexFormat convert(Format format)
{
    switch (format) {
        case Format::rgb8_unorm:
            return MTLVertexFormatUChar3;
        case Format::rgba8_unorm:
            return MTLVertexFormatUChar4;
        case Format::rg32_float:
            return MTLVertexFormatFloat2;
        case Format::rgb32_float:
            return MTLVertexFormatFloat3;
        case Format::rgba32_float:
            return MTLVertexFormatFloat4;
        default:
            throw std::runtime_error("invalid the format");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLIndexType convert(Index_type type)
{
    switch (type) {
        case Index_type::uint16:
            return MTLIndexTypeUInt16;
        case Index_type::uint32:
            return MTLIndexTypeUInt32;
        default:
            throw std::runtime_error("invalid the index type");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLBlendFactor convert(Blend_factor factor)
{
    switch (factor) {
        case Blend_factor::zero:
            return MTLBlendFactorZero;
        case Blend_factor::one:
            return MTLBlendFactorOne;
        case Blend_factor::src_alpha:
            return MTLBlendFactorSourceAlpha;
        case Blend_factor::one_minus_src_alpha:
            return MTLBlendFactorOneMinusSourceAlpha;
        case Blend_factor::dst_alpha:
            return MTLBlendFactorDestinationAlpha;
        case Blend_factor::one_minus_dst_alpha:
            return MTLBlendFactorOneMinusDestinationAlpha;
        default:
            throw std::runtime_error("invalid the blend factor");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLBlendOperation convert(Blend_op op)
{
    switch (op) {
        case Blend_op::add:
            return MTLBlendOperationAdd;
        case Blend_op::subtract:
            return MTLBlendOperationSubtract;
        case Blend_op::reverse_subtract:
            return MTLBlendOperationReverseSubtract;
        case Blend_op::min:
            return MTLBlendOperationMin;
        case Blend_op::max:
            return MTLBlendOperationMax;
        default:
            throw std::runtime_error("invalid the blend op");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLPrimitiveTopologyClass convert(Topology topology)
{
    switch (topology) {
        case Topology::triangle_list:
        case Topology::triangle_strip:
            return MTLPrimitiveTopologyClassTriangle;
        case Topology::point:
            return MTLPrimitiveTopologyClassPoint;
        default:
            throw std::runtime_error("invalid the topology");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLPrimitiveType convert(Topology topology)
{
    switch (topology) {
        case Topology::triangle_list:
            return MTLPrimitiveTypeTriangle;
        case Topology::triangle_strip:
            return MTLPrimitiveTypeTriangleStrip;
        case Topology::point:
            return MTLPrimitiveTypePoint;
       default:
            throw std::runtime_error("invalid the topology");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLCullMode convert(Cull_mode mode)
{
    switch (mode) {
        case Cull_mode::front:
            return MTLCullModeFront;
        case Cull_mode::back:
            return MTLCullModeBack;
        case Cull_mode::none:
            return MTLCullModeNone;
        default:
            throw std::runtime_error("invalid the cull mode");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLStencilOperation convert(Stencil_op op)
{
    switch (op) {
        case Stencil_op::keep:
            return MTLStencilOperationKeep;
        case Stencil_op::zero:
            return MTLStencilOperationZero;
        case Stencil_op::replace:
            return MTLStencilOperationReplace;
        case Stencil_op::increment_and_clamp:
            return MTLStencilOperationIncrementClamp;
        case Stencil_op::decrement_and_clamp:
            return MTLStencilOperationDecrementClamp;
        case Stencil_op::invert:
            return MTLStencilOperationInvert;
        case Stencil_op::increment_and_wrap:
            return MTLStencilOperationIncrementWrap;
        case Stencil_op::decrement_and_wrap:
            return MTLStencilOperationDecrementWrap;
        default:
            throw std::runtime_error("invalid the stencil op");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLCompareFunction convert(Compare_op op)
{
    switch (op) {
        case Compare_op::never:
            return MTLCompareFunctionNever;
        case Compare_op::less:
            return MTLCompareFunctionLess;
        case Compare_op::greater:
            return MTLCompareFunctionGreater;
        case Compare_op::equal:
            return MTLCompareFunctionEqual;
        case Compare_op::not_equal:
            return MTLCompareFunctionNotEqual;
        case Compare_op::less_or_equal:
            return MTLCompareFunctionLessEqual;
        case Compare_op::greater_or_equal:
            return MTLCompareFunctionGreaterEqual;
        case Compare_op::always:
            return MTLCompareFunctionAlways;
        default:
            throw std::runtime_error("invalid the stencil op");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLLoadAction convert(Load_op op)
{
    switch (op) {
        case Load_op::load:
            return MTLLoadActionLoad;
        case Load_op::clear:
            return MTLLoadActionClear;
        case Load_op::dont_care:
            return MTLLoadActionDontCare;
        default:
            throw std::runtime_error("invalid the load op");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLStoreAction convert(Store_op op)
{
    switch (op) {
        case Store_op::store:
            return MTLStoreActionStore;
        case Store_op::dont_care:
            return MTLStoreActionDontCare;
        default:
            throw std::runtime_error("invalid the store op");
    }
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLClearColor convert(Clear_value clear_value)
{
    return MTLClearColorMake(clear_value.r, clear_value.g, clear_value.b, clear_value.a);
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLViewport convert(Viewport viewport)
{
    return MTLViewport { viewport.x, viewport.y, viewport.w, viewport.h, 0.0, 1.0 };
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLScissorRect convert(Scissor scissor)
{
    return { scissor.x, scissor.y, scissor.w, scissor.h };
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLOrigin convert(Offset offset)
{
    return MTLOriginMake(offset.x, offset.y, offset.z);
}

//----------------------------------------------------------------------------------------------------------------------

template<>
inline MTLSize convert(Extent extent)
{
    return MTLSizeMake(extent.w, extent.h, extent.d);
}

//----------------------------------------------------------------------------------------------------------------------

} // of namespace Gfx_lib

#endif // GFX_MTL_LIB_GUARD