#pragma once

#include "macros.hpp"

GFX_BEGIN_NAMESPACE

namespace ShaderType
{
    enum
    {
        VERTEX		 = 0,
        FRAGMENT	 = 1,
        GEOMETRY	 = 2,
        TESS_CONTROL = 3,
        TESS_EVAL	 = 4,
        COMPUTE		 = 5
    };
};

namespace DepthTest
{
    enum
    {
        DISABLE = 0,
        ENABLE  = 1
    };
};

namespace PrimitiveType
{
    enum
    {
        POINTS         = 0,
        TRIANGLES      = 1,
        TRIANGLE_STRIP = 2,
        LINES          = 3,
        LINE_STRIP     = 4
    };
};

namespace CullMode
{
    enum
    {
        NONE           = 0,
        FRONT          = 1,
        BACK           = 2,
        FRONT_AND_BACK = 3
    };
};

namespace FillMode
{
    enum
    {
        SOLID     = 0,
        WIREFRAME = 1
    };
};

namespace StencilOperation
{
    enum
    {
        KEEP     = 0,
        ZERO     = 1,
        REPLACE  = 2,
        INCR_SAT = 3,
        DECR_SAT = 4,
        INVERT   = 5,
        INCR     = 6,
        DECR     = 7
    };
};

namespace ComparisonFunction
{
    enum
    {
        NEVER         = 0,
        LESS          = 1,
        EQUAL         = 2,
        LESS_EQUAL    = 3,
        GREATER       = 4,
        NOT_EQUAL     = 5,
        GREATER_EQUAL = 6,
        ALWAYS        = 7
    };
};

namespace GraphicsCommandType
{
    enum
    {
        Draw                  = 0,
        DrawIndexed           = 1,
        DrawIndexedBaseVertex = 2,
        BindTexture           = 3,
        BindSamplerState      = 4,
        BindRasterizerState   = 5,
        BindDepthStencilState = 6,
        BindVertexArray       = 7,
        BindFramebuffer       = 8,
        BindUniformBuffer     = 9,
        CopyUniformData       = 10
    };
};

namespace ClearTarget
{
    enum
    {
        COLOR   = 1,
        DEPTH   = 2,
        STENCIL = 4,
        ALL     = COLOR | DEPTH | STENCIL
    };
};

namespace TextureType
{
    enum
    {
        TEXTURE1D   = 0,
        TEXTURE2D   = 1,
        TEXTURE3D   = 2,
        TEXTURECUBE = 3
    };
};

namespace TextureWrapMode
{
    enum
    {
        REPEAT          = 0,
        MIRRORED_REPEAT = 1,
        CLAMP_TO_EDGE   = 2,
        CLAMP_TO_BORDER = 3
    };
};

namespace TextureFormat
{
    enum
    {
        R32G32B32_FLOAT    = 0,
        R32G32B32A32_FLOAT = 1,
        R32G32B32_UINT     = 2,
        R32G32B32A32_UINT  = 3,
        R32G32B32_INT      = 4,
        R32G32B32A32_INT   = 5,
        R16G16B16A16_FLOAT = 6,
        R16G16B16A16_UINT  = 7,
        R16G16B16A16_INT   = 8,
        R8G8B8A8_UNORM     = 9,
        R8G8B8A8_UINT      = 10,
        D32_FLOAT_S8_UINT  = 11,
        D24_FLOAT_S8_UINT  = 12,
        D16_FLOAT          = 13
    };
};

namespace TextureFilteringMode
{
    enum
    {
        LINEAR_ALL            = 0,
        NEAREST_ALL           = 1,
        ANISOTROPIC_ALL       = 2,
        LINEAR_MIPMAP_NEAREST = 3,
        NEAREST_MIPMAP_LINEAR = 4,
    };
};

namespace BufferType
{
    enum
    {
        VERTEX  = 0,
        INDEX   = 1,
        UNIFORM = 2,
        APPEND  = 3,
        QUERY   = 4
    };
};

namespace BufferUsageType
{
    enum
    {
        STATIC  = 0,
        DYNAMIC = 1,
        STREAM  = 2
    };
};

namespace BufferMapType
{
    enum
    {
        READ = 0,
        WRITE,
        READ_WRITE
    };
};

namespace DataType
{
    enum
    {
        BYTE   = 0,
        UBYTE  = 1,
        INT16  = 2,
        INT32  = 3,
        UINT16 = 4,
        UINT32 = 5,
        FLOAT  = 6
    };
};

namespace InputLayoutType
{
    enum
    {
        STANDARD_VERTEX         = 0,
        STANDARD_SKINNED_VERTEX = 1,
        OCEAN_VERTEX            = 2,
        QUAD_VERTEX             = 3,
        CUSTOM_VERTEX           = 4
    };
};


GFX_END_NAMESPACE
