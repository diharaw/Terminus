#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <gfx/gfx_enums.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct Shader;
struct InputElement;
struct InputLayout;
struct Buffer;
struct Texture;

struct RenderDeviceInitData
{
    void*  memory;
    size_t size;
};

struct TextureCreateDesc
{
	TextureFormat format;
	TextureUsage  usage;
	TextureType	  type;
    uint16_t width;
    uint16_t height;
    uint16_t depth;
	uint16_t samples;
    void*    data;
	uint16_t array_slices;
    uint16_t mipmap_levels;
    uint32_t flags;
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
    Buffer*		 vertex_buffer;
    Buffer*		 index_buffer;
    InputLayout* layout;
};

struct RenderTargetDesc
{
	Texture* texture = nullptr;
	uint32_t array_slice = 0;
	uint32_t mip_slice = 0;
};

struct DepthStencilTargetDesc
{
	Texture* texture = nullptr;
	uint32_t array_slice = 0;
	uint32_t mip_slice = 0;
};

struct FramebufferCreateDesc
{
	uint32_t			   render_target_count;
	RenderTargetDesc*      render_targets;
	DepthStencilTargetDesc depth_stencil_target;
};

struct DepthStencilStateCreateDesc
{
    bool     enable_depth_test;
	bool     enable_depth_write;
    bool     enable_stencil_test;
	CompFunc depth_cmp_func;
	StencilOp front_stencil_fail;
	StencilOp front_stencil_pass_depth_fail;
	StencilOp front_stencil_pass_depth_pass;
	CompFunc front_stencil_cmp_func;
	uint32_t front_stencil_mask;
	StencilOp back_stencil_fail;
	StencilOp back_stencil_pass_depth_fail;
	StencilOp back_stencil_pass_depth_pass;
	CompFunc back_stencil_cmp_func;
	uint32_t back_stencil_mask;
};

struct RasterizerStateCreateDesc
{
    CullMode cull_mode = GFX_CULL_MODE_BACK;
    FillMode fill_mode = GFX_FILL_MODE_SOLID;
    bool     front_winding_ccw = true;
    bool     multisample = false;
    bool     scissor = false;
};

struct SamplerStateCreateDesc
{
    TextureFilter min_filter = GFX_FILTER_LINEAR_ALL;
	TextureFilter mag_filter = GFX_FILTER_LINEAR;
    TextureAddressMode wrap_mode_u = GFX_ADDRESS_REPEAT;
	TextureAddressMode wrap_mode_v = GFX_ADDRESS_REPEAT;
    TextureAddressMode wrap_mode_w = GFX_ADDRESS_REPEAT;
	uint32_t comparison_mode;
	CompFunc comparison_func = GFX_COMP_FUNC_LESS;
    float    max_anisotropy = 0;
    float    border_color[4];
};

struct SourceShaderCreateDesc
{
    ShaderStage type;
    const char* shader_source;
	const char*	entry_point;
};

struct BinaryShaderCreateDesc
{
	ShaderStage type;
	size_t		size;
	void*		data;
	const char*	entry_point;
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
	bool enable = false;
	BlendFunc src_func;
	BlendFunc dst_func;
	BlendOp blend_op;
	BlendFunc src_func_alpha;
	BlendFunc dst_func_alpha;
	BlendOp blend_op_alpha;
};

struct PipelineStateCreateDesc
{
    DepthStencilStateCreateDesc depth_stencil_state;
    RasterizerStateCreateDesc   rasterizer_state;
    BlendStateCreateDesc        blend_state;
    uint32_t                    primitive;
};

TE_END_TERMINUS_NAMESPACE