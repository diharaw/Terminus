#pragma once

#include <stdint.h>
#include "macros.hpp"

GFX_BEGIN_NAMESPACE

struct Shader;
struct InputElement;
struct InputLayout;
struct VertexBuffer;
struct IndexBuffer;
struct Texture;

struct RenderDeviceInitData
{
    void*  memory;
    size_t size;
};

struct Texture1DCreateDesc
{
    uint16_t width;
    void*    data;
    uint32_t format;
    bool     generate_mipmaps;
    uint16_t mipmap_levels;
};

struct Texture2DCreateDesc
{
    uint16_t width;
    uint16_t height;
    void*    data;
    uint32_t format;
    bool     create_render_target_view;
    bool     generate_mipmaps;
    uint16_t mipmap_levels;
};

struct Texture3DCreateDesc
{
    uint16_t width;
    uint16_t height;
    uint16_t depth;
    void*    data;
    uint32_t format;
    bool     create_render_target_view;
    bool     generate_mipmaps;
    uint16_t mipmap_levels;
};

struct TextureCubeCreateDesc
{
    uint16_t width;
    uint16_t height;
    void*    data[6];
    uint32_t format;
    bool     generate_mipmaps;
    bool     create_render_target_view;
    uint16_t mipmap_levels;
};

struct BufferCreateDesc
{
    void*	 data;
    uint32_t size;
    uint32_t usage_type;
    uint32_t data_type;
};

struct InputLayoutCreateDesc
{
    InputElement* elements;
    uint32_t	  vertex_size;
    uint32_t	  num_elements;
};

struct VertexArrayCreateDesc
{
    VertexBuffer* vertex_buffer;
    IndexBuffer*  index_buffer;
    InputLayout*  layout;
};

struct FramebufferCreateDesc
{
    uint16_t num_render_targets;
    Texture* render_targets[MAX_RENDER_TARGETS];
    Texture* depth_target;
};

struct DepthStencilStateCreateDesc
{
    bool     enable_depth_test;
    bool     enable_stencil_test;
    bool     depth_mask;
    uint32_t depth_cmp_func;
    uint32_t front_stencil_fail;
    uint32_t front_stencil_pass_depth_fail;
    uint32_t front_stencil_pass_depth_pass;
    uint32_t front_stencil_cmp_func;
    uint32_t back_stencil_fail;
    uint32_t back_stencil_pass_depth_fail;
    uint32_t back_stencil_pass_depth_pass;
    uint32_t back_stencil_cmp_func;
    uint32_t stencil_mask;
};

struct RasterizerStateCreateDesc
{
    uint32_t cull_mode;
    uint32_t fill_mode;
    bool     front_winding_ccw;
    bool     multisample;
    bool     scissor;
};

struct SamplerStateCreateDesc
{
    uint32_t min_filter;
    uint32_t mag_filter;
    uint32_t wrap_mode_u;
    uint32_t wrap_mode_v;
    uint32_t wrap_mode_w;
    float    max_anisotropy;
    float    border_color[4];
};

struct ShaderCreateDesc
{
    uint32_t    type;
    const char* shader_source;
};

struct ShaderProgramCreateDesc
{
    Shader* vertex;
    Shader* pixel;
    Shader* geometry;
    Shader* tessellation_control;
    Shader* tessellation_evaluation;
};

struct BlendStateCreateDesc
{
    
};

struct PipelineStateObjectCreateDesc
{
    DepthStencilStateCreateDesc depth_stencil_state;
    RasterizerStateCreateDesc   rasterizer_state;
    BlendStateCreateDesc        blend_state;
    uint32_t                    primitive;
};

GFX_END_NAMESPACE
