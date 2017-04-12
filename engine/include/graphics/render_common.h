#ifndef RenderCommon_h
#define RenderCommon_h

#include <string>
#include <vector>
#include <core/types.h>

#define MAX_RENDER_TARGETS 8

namespace terminus
{
    struct RenderDeviceInitData
    {
        void* memory;
        size_t size;
    };
    
    enum class GeometryType
    {
        SCENE = 0,
        SKY = 1,
        QUAD = 2
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
    
    enum class GraphicsCommandType
    {
        Draw = 0,
        DrawIndexed,
        DrawIndexedBaseVertex,
        BindTexture,
        BindSamplerState,
        BindRasterizerState,
        BindDepthStencilState,
        BindVertexArray,
        BindFramebuffer,
        BindUniformBuffer,
        CopyUniformData
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
    
    enum TextureFormat
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
    
    // Resource Creation Description Structures
    
    struct Texture1DCreateDesc
    {
        uint16_t width;
        void* data;
        TextureFormat format;
        bool generate_mipmaps;
        uint16_t mipmap_levels;
        
        Texture1DCreateDesc()
        {
            generate_mipmaps = true;
            mipmap_levels = 10;
        }
    };
    
    struct Texture2DCreateDesc
    {
        uint16_t width;
        uint16_t height;
        void* data;
        TextureFormat format;
        bool create_render_target_view;
        bool generate_mipmaps;
        uint16_t mipmap_levels;
        
        Texture2DCreateDesc()
        {
            generate_mipmaps = true;
            mipmap_levels = 10;
        }
    };
    
    struct Texture3DCreateDesc
    {
        uint16_t width;
        uint16_t height;
        uint16_t depth;
        void* data;
        TextureFormat format;
        bool create_render_target_view;
        bool generate_mipmaps;
        uint16_t mipmap_levels;
        
        Texture3DCreateDesc()
        {
            generate_mipmaps = true;
            mipmap_levels = 10;
        }
    };
    
    // Array order [+X, -X, +Y, -Y, +Z, -Z]
    struct TextureCubeCreateDesc
    {
        uint16_t width;
        uint16_t height;
        void* data[6];
        TextureFormat format;
        bool generate_mipmaps;
        bool create_render_target_view;
        uint16_t mipmap_levels;
        
        TextureCubeCreateDesc()
        {
            generate_mipmaps = true;
            mipmap_levels = 10;
            create_render_target_view = false;
        }
    };
    
    struct BufferCreateDesc
    {
        void* data;
        uint32_t size;
        BufferUsageType usage_type;
    };
    
    struct VertexBuffer;
    struct IndexBuffer;
    
    struct VertexArrayCreateDesc
    {
        VertexBuffer* vertex_buffer;
        IndexBuffer* index_buffer;
        InputLayoutType layout_type;
        InputLayout* layout;
        
        VertexArrayCreateDesc()
        {
            layout = nullptr;
        }
    };
    
    struct Texture;
    
    struct FramebufferCreateDesc
    {
        uint16_t num_render_targets;
        Texture* render_targets[MAX_RENDER_TARGETS];
        Texture* depth_target;
        
        FramebufferCreateDesc()
        {
            num_render_targets = 0;
            depth_target = nullptr;
        }
    };
    
    struct DepthStencilStateCreateDesc
    {
        bool enable_depth_test;
        bool enable_stencil_test;
        bool depth_mask;
        ComparisonFunction depth_cmp_func;
        StencilOperation front_stencil_fail;
        StencilOperation front_stencil_pass_depth_fail;
        StencilOperation front_stencil_pass_depth_pass;
        ComparisonFunction front_stencil_cmp_func;
        StencilOperation back_stencil_fail;
        StencilOperation back_stencil_pass_depth_fail;
        StencilOperation back_stencil_pass_depth_pass;
        ComparisonFunction back_stencil_cmp_func;
        uint32_t stencil_mask;
        
        DepthStencilStateCreateDesc()
        {
            enable_depth_test = true;
            enable_stencil_test = false;
            depth_mask = true;
            depth_cmp_func = ComparisonFunction::LESS;
            front_stencil_fail = StencilOperation::REPLACE;
            front_stencil_pass_depth_fail = StencilOperation::REPLACE;
            front_stencil_pass_depth_pass = StencilOperation::REPLACE;
            front_stencil_cmp_func = ComparisonFunction::LESS;
            back_stencil_fail = StencilOperation::REPLACE;
            back_stencil_pass_depth_fail = StencilOperation::REPLACE;
            back_stencil_pass_depth_pass = StencilOperation::REPLACE;
            back_stencil_cmp_func = ComparisonFunction::LESS;
            stencil_mask = 0xFF;
        }
    };
    
    struct RasterizerStateCreateDesc
    {
        CullMode cull_mode;
        FillMode fill_mode;
        bool front_winding_ccw;
        bool multisample;
        bool scissor;
        
        RasterizerStateCreateDesc()
        {
            cull_mode = CullMode::BACK;
            fill_mode = FillMode::SOLID;
            front_winding_ccw = true;
            multisample = false;
            scissor = false;
        }
    };
    
    struct SamplerStateCreateDesc
    {
        TextureFilteringMode min_filter;
        TextureFilteringMode mag_filter;
        TextureWrapMode wrap_mode_u;
        TextureWrapMode wrap_mode_v;
        TextureWrapMode wrap_mode_w;
        float max_anisotropy;
        Vector4 border_color;
        
        SamplerStateCreateDesc()
        {
            max_anisotropy = 1.0f;
        }
    };
    
    struct ShaderCreateDesc
    {
        ShaderType type;
        const char* shader_source;
    };
    
    struct Shader;
    
    struct ShaderProgramCreateDesc
    {
        Shader* vertex;
        Shader* pixel;
        Shader* geometry;
        Shader* tessellation_control;
        Shader* tessellation_evaluation;
        
        ShaderProgramCreateDesc()
        {
            geometry = nullptr;
            tessellation_control = nullptr;
            tessellation_evaluation = nullptr;
        }
    };
    
    struct BlendStateCreateDesc
    {
        
    };
    
    struct PipelineStateObjectCreateDesc
    {
        DepthStencilStateCreateDesc depth_stencil_state;
        RasterizerStateCreateDesc   rasterizer_state;
        BlendStateCreateDesc        blend_state;
        DrawPrimitive               primitive;
    };
}

#endif
