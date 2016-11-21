#ifndef RenderCommon_h
#define RenderCommon_h

#include <string>
#include <vector>

enum class GeometryType
{
	SCENE = 0,
	QUAD = 1
};

enum class DepthTest
{
	DISABLE = 0,
	ENABLE = 1
};

enum class DrawPrimitive
{
    POINTS = 0,
    TRIANGLES,
    TRIANGLE_STRIP,
    LINES,
    LINE_STRIP
};

enum class CullMode
{
    FRONT = 0,
    BACK = 1,
    NONE = 2,
	FRONT_BACK = 3
};

enum class FillMode
{
    SOLID = 0,
    WIREFRAME
};

enum class StencilOperation
{
    KEEP = 0,
    ZERO,
    REPLACE,
    INCR_SAT,
    DECR_SAT,
    INVERT,
    INCR,
    DECR
};

enum class ComparisonFunction
{
    NEVER = 0,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS
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

enum class BufferType
{
    VERTEX = 0,
    INDEX,
    UNIFORM,
    APPEND,
    QUERY
};

enum class BufferUsageType
{
    STATIC = 0,
    DYNAMIC
};

enum class BufferMapType
{
    READ = 0,
    WRITE,
    READ_WRITE
};

enum class BufferDataType
{
    FLOAT = 0,
    INT,
    UINT
};

// TODO: add instancing capability

enum class InputLayoutType
{
    STANDARD_VERTEX = 0,
    STANDARD_SKINNED_VERTEX,
    OCEAN_VERTEX,
    QUAD_VERTEX,
    CUSTOM_VERTEX
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
    COLOR = 1,
    DEPTH = 2,
    STENCIL = 4,
	ALL = 8
};

enum class TextureTarget
{
    TEXTURE1D = 0,
    TEXTURE2D,
    TEXTURE3D,
    TEXTURECUBE
};

enum class TextureWrapMode
{
    REPEAT = 0,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum class TextureFormat
{
    R32G32B32_FLOAT = 0,
    R32G32B32A32_FLOAT,
    R32G32B32_UINT,
    R32G32B32A32_UINT,
    R32G32B32_INT,
    R32G32B32A32_INT,
    R16G16B16A16_FLOAT,
    R16G16B16A16_UINT,
    R16G16B16A16_INT,
    R8G8B8A8_UNORM,
    R8G8B8A8_UINT,
    D32_FLOAT_S8_UINT,
    D24_FLOAT_S8_UINT,
    D16_FLOAT
};

// GL_LINEAR_MIP_MAP_LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR

enum class TextureFilteringMode
{
    LINEAR_ALL = 0,
    NEAREST_ALL,
    ANISOTROPIC_ALL,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
};

// Shader Common

enum class ShaderType
{
    VERTEX = 0,
    GEOMETRY = 1,
    TESSELLATION_CONTROL = 2,
    TESSELLATION_EVALUATION = 3,
    PIXEL = 4
};

#endif 
