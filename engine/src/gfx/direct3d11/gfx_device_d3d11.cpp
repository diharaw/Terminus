#if defined(TE_GFX_BACKEND_D3D11)

#include <gfx/direct3d11/gfx_device_d3d11.hpp>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "utility.h"
#include "logger.h"

GfxDevice::GfxDevice()
{
    
}

GfxDevice::~GfxDevice()
{
    
}

bool GfxDevice::init()
{

}

Shader* GfxDevice::create_shader(const char* source, uint32_t type)
{

}

ShaderProgram* GfxDevice::create_shader_program(Shader** shaders, uint32_t count)
{

}

void GfxDevice::attach_render_target(Framebuffer* framebuffer, const RenderTargetDesc& desc)
{

}

void GfxDevice::attach_depth_stencil_target(Framebuffer* framebuffer, const DepthStencilTargetDesc& desc)
{

}

Framebuffer* GfxDevice::create_framebuffer(const FramebufferCreateDesc& desc)
{

}

VertexBuffer* GfxDevice::create_vertex_buffer(const BufferCreateDesc& desc)
{

}

UniformBuffer* GfxDevice::create_uniform_buffer(const BufferCreateDesc& desc)
{

}

IndexBuffer* GfxDevice::create_index_buffer(const BufferCreateDesc& desc)
{

}

InputLayout* GfxDevice::create_input_layout(const InputLayoutCreateDesc& desc)
{

}

VertexArray* GfxDevice::create_vertex_array(const VertexArrayCreateDesc& desc)
{

}

Texture2D* GfxDevice::create_texture_2d(const Texture2DCreateDesc& desc)
{

}

Texture2D* GfxDevice::create_texture_2d_array(const Texture2DArrayCreateDesc& desc)
{

}

TextureCube* GfxDevice::create_texture_cube(const TextureCubeCreateDesc& desc)
{

}

void GfxDevice::set_texture_data(Texture* texture, const int& mipSlice, const int& arraySlice, uint16_t& width, uint16_t& height, void* data)
{

}

void GfxDevice::generate_mipmaps(Texture* texture)
{

}

void GfxDevice::wait_for_idle()
{
	
}

RasterizerState* GfxDevice::create_rasterizer_state(const RasterizerStateCreateDesc& desc)
{

}

DepthStencilState* GfxDevice::create_depth_stencil_state(const DepthStencilStateCreateDesc& desc)
{

}

BlendState* GfxDevice::create_blend_state(const BlendStateCreateDesc& desc)
{

}

SamplerState* GfxDevice::create_sampler_state(const SamplerStateCreateDesc& desc)
{

}

void GfxDevice::destroy(Shader* shader)
{

}

void GfxDevice::destroy(ShaderProgram* program)
{

}

void GfxDevice::destroy(VertexBuffer* vertex_buffer)
{

}

void GfxDevice::destroy(IndexBuffer* index_buffer)
{

}

void GfxDevice::destroy(UniformBuffer* buffer)
{

}

void GfxDevice::destroy(VertexArray* vertex_array)
{

}

void GfxDevice::destroy(Texture* texture)
{

}

void GfxDevice::destroy(RasterizerState* state)
{
	
}

void GfxDevice::destroy(BlendState* state)
{
	
}

void GfxDevice::destroy(SamplerState* state)
{
	
}

void GfxDevice::destroy(DepthStencilState* state)
{

}

void GfxDevice::destroy(PipelineStateObject* pso)
{
	
}

void GfxDevice::destroy(Framebuffer* framebuffer)
{

}

void GfxDevice::texture_extents(Texture* texture, const int& mipSlice, int& width, int& height)
{

}

void GfxDevice::texture_data(Texture* texture, const int& mipSlice, const int& arraySlice, void* data)
{

}

void GfxDevice::bind_pipeline_state_object(PipelineStateObject* pso)
{

}

int GfxDevice::uniform_buffer_alignment()
{

}

void GfxDevice::bind_texture(Texture* texture, uint32_t shader_stage, uint32_t buffer_slot)
{

}

void GfxDevice::bind_uniform_buffer(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot)
{

}

void GfxDevice::bind_uniform_buffer_range(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot, size_t offset, size_t size)
{
	
}

void GfxDevice::bind_vertex_array(VertexArray* vertex_array)
{

}

void GfxDevice::bind_rasterizer_state(RasterizerState* state)
{

}

void GfxDevice::bind_sampler_state(SamplerState* state, uint32_t shader_stage, uint32_t slot)
{

}

void GfxDevice::bind_blend_state(BlendState* state)
{

}

void GfxDevice::bind_framebuffer(Framebuffer* framebuffer)
{

}

void GfxDevice::bind_depth_stencil_state(DepthStencilState* state)
{

}

void GfxDevice::bind_shader_program(ShaderProgram* program)
{

}

void* GfxDevice::map_buffer(Buffer* buffer, uint32_t type)
{

}

void GfxDevice::unmap_buffer(Buffer* buffer)
{

}

void GfxDevice::set_primitive_type(uint32_t primitive)
{
	
}

void GfxDevice::clear_framebuffer(uint32_t clear_target, float* clear_color)
{

}

void GfxDevice::set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
{
	
}

void GfxDevice::dispatch_compute(uint32_t x, uint32_t y, uint32_t z)
{

}

void GfxDevice::draw(uint32_t first_index, uint32_t count)
{
	
}

void GfxDevice::draw_indexed(uint32_t index_count)
{

}

void GfxDevice::draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
{

}

#endif