#ifndef RenderCommon_h
#define RenderCommon_h

#include <string>
#include <vector>

enum GeometryType
{
	GEOMETRY_SCENE = 0,
	GEOMETRY_QUAD = 1
};

enum DepthTest
{
	DEPTH_DISABLE = 0,
	DEPTH_ENABLE = 1
};

enum DrawPrimitive
{
    PRIMITIVE_POINTS = 0,
    PRIMITIVE_TRIANGLES,
    PRIMITIVE_TRIANGLE_STRIP,
    PRIMITIVE_LINES,
    PRIMITIVE_LINE_STRIP
};

enum CullMode
{
    CULL_FRONT = 0,
    CULL_BACK = 1,
    CULL_NONE = 2,
	CULL_FRONT_BACK = 3
};

enum FillMode
{
    FILL_SOLID = 0,
    FILL_WIREFRAME
};

enum StencilOperation
{
    STENCIL_OP_KEEP = 0,
    STENCIL_OP_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INCR_SAT,
    STENCIL_OP_DECR_SAT,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCR,
    STENCIL_OP_DECR
};

enum ComparisonFunction
{
    FUNC_NEVER = 0,
    FUNC_LESS,
    FUNC_EQUAL,
    FUNC_LESS_EQUAL,
    FUNC_GREATER,
    FUNC_NOT_EQUAL,
    FUNC_GREATER_EQUAL,
    FUNC_ALWAYS
};

struct RasterizerStateDesc
{
    CullMode m_cullMode;
    FillMode m_fillMode;
    bool m_frontWindingCCW;
    bool m_multisample;
    bool m_scissor;
};

struct DepthStencilStateDesc
{
    bool m_enableDepthTest;
    bool m_enableStencilTest;
    
    // Depth
    
    bool m_depthMask;
    ComparisonFunction m_depthComparisonFunction;
    
    // Stencil
    
    StencilOperation m_frontStencilFail;
    StencilOperation m_frontStencilPassDepthFail;
    StencilOperation m_frontStencilPassDepthPass;
    ComparisonFunction m_frontStencilComparisonFunction;
    
    StencilOperation m_backStencilFail;
    StencilOperation m_backStencilPassDepthFail;
    StencilOperation m_backStencilPassDepthPass;
    ComparisonFunction m_backStencilComparisonFunction;
    
    unsigned int m_stencilMask;
};

// BUFFER COMMON

enum BufferType
{
    BUFFER_VERTEX = 0,
    BUFFER_INDEX,
    BUFFER_UNIFORM,
    BUFFER_APPEND,
    BUFFER_QUERY
};

enum BufferUsageType
{
    USAGE_STATIC = 0,
    USAGE_DYNAMIC
};

enum class BufferMapType
{
    MAP_READ = 0,
    MAP_WRITE,
    MAP_READ_WRITE
};

enum BufferDataType
{
    BUFFER_FLOAT = 0,
    BUFFER_INT,
    BUFFER_UINT
};

// TODO: add instancing capability

enum InputLayoutType
{
    LAYOUT_STANDARD_VERTEX = 0,
    LAYOUT_STANDARD_SKINNED_VERTEX,
    LAYOUT_CUSTOM
};

struct InputElement
{
    int m_numSubElements;
    BufferDataType m_type;
    bool m_isNormalized;
    unsigned int m_offset;
    std::string m_semanticName;
};

struct InputLayout
{
    std::vector<InputElement> m_Elements;
    unsigned int m_vertexSize;
    
    void addElement(int _index, InputElement _element)
    {
        m_Elements[_index] = _element;
    }
};

// Texture Common

enum class FramebufferClearTarget
{
    FB_TARGET_COLOR = 1,
    FB_TARGET_DEPTH = 2,
    FB_TARGET_STENCIL = 4,
	FB_TARGET_ALL = 8
};

enum TextureTarget
{
    TEXTURE1D = 0,
    TEXTURE2D,
    TEXTURE3D,
    TEXTURECUBE
};

enum TextureWrapMode
{
    REPEAT = 0,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum TextureFormat
{
    FORMAT_R32G32B32_FLOAT = 0,
    FORMAT_R32G32B32A32_FLOAT,
    FORMAT_R32G32B32_UINT,
    FORMAT_R32G32B32A32_UINT,
    FORMAT_R32G32B32_INT,
    FORMAT_R32G32B32A32_INT,
    FORMAT_R16G16B16A16_FLOAT,
    FORMAT_R16G16B16A16_UINT,
    FORMAT_R16G16B16A16_INT,
    FORMAT_R8G8B8A8_UNORM,
    FORMAT_R8G8B8A8_UINT,
    FORMAT_D32_FLOAT_S8_UINT,
    FORMAT_D24_FLOAT_S8_UINT,
    FORMAT_D16_FLOAT
};

// GL_LINEAR_MIP_MAP_LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR

enum TextureFilteringMode
{
    LINEAR_ALL = 0,
    NEAREST_ALL,
    ANISOTROPIC_ALL,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
};

// Shader Common

enum ShaderType
{
    SHADER_VERTEX = 0,
    SHADER_GEOMETRY = 1,
    SHADER_TESSELLATION_CONTROL = 2,
    SHADER_TESSELLATION_EVALUATION = 3,
    SHADER_PIXEL = 4
};

#endif 
