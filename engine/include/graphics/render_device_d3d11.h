#pragma once

#include <Core/config.h>

#if defined(TERMINUS_DIRECT3D11)

#include <vector>
#include <graphics/render_common.h>
#include <resource/asset_common.h>
#include <core/types.h>
#include <container/packed_array.h>
#include <platform/platform.h>
#include <memory/pool_allocator.h>
#include <graphics/graphics_types_d3d11.h>

#define MAX_TEXTURE_1D 1
#define MAX_TEXTURE_2D 1
#define MAX_TEXTURE_3D 1024
#define MAX_TEXTURE_CUBE 64
#define MAX_FRAMEBUFFER 64
#define MAX_VERTEX_BUFFER 64
#define MAX_INDEX_BUFFER 64
#define MAX_UNIFORM_BUFFER 64
#define MAX_VERTEX_ARRAY 64
#define MAX_SHADER_PROGRAM 256
#define MAX_SHADER 256
#define MAX_SAMPLER_STATE 16
#define MAX_RASTERIZER_STATE 16
#define MAX_DEPTH_STENCIL_STATE 16

namespace terminus
{
	class GraphicsQueue;

	class RenderDevice
	{
	public:

		RenderDevice();
		~RenderDevice();
		void initialize();
		bool initialize_api();
		void shutdown();

		// Object Creation
		PipelineStateObject* create_pipeline_state_object(PipelineStateObjectCreateDesc desc);
		Texture1D* create_texture_1d(Texture1DCreateDesc desc);
		Texture2D* create_texture_2d(Texture2DCreateDesc desc);
		Texture3D* create_texture_3d(Texture3DCreateDesc desc);
		TextureCube* create_texture_cube(TextureCubeCreateDesc desc);
		VertexBuffer* create_vertex_buffer(BufferCreateDesc desc);
		IndexBuffer* create_index_buffer(BufferCreateDesc desc);
		UniformBuffer* create_uniform_buffer(BufferCreateDesc desc);
		VertexArray* create_vertex_array(VertexArrayCreateDesc desc);
		RasterizerState* create_rasterizer_state(RasterizerStateCreateDesc desc);
		SamplerState*	 create_sampler_state(SamplerStateCreateDesc desc);
		Framebuffer*	 create_framebuffer(FramebufferCreateDesc desc);
		Shader*			 create_shader(ShaderCreateDesc desc);
		ShaderProgram*   create_shader_program(ShaderProgramCreateDesc desc);
		DepthStencilState* create_depth_stencil_state(DepthStencilStateCreateDesc desc);
		void attach_render_target(Framebuffer* framebuffer, Texture* render_target);
		void attach_depth_stencil_target(Framebuffer* framebuffer, Texture* render_target);
		GraphicsQueue* create_graphics_queue();

		// Object Destruction
		void destroy_pipeline_state_object(PipelineStateObject* pso);
		void destroy_texture_1d(Texture1D* texture);
		void destroy_texture_2d(Texture2D* texture);
		void destroy_texture_3d(Texture3D* texture);
		void destroy_texture_cube(TextureCube* texture);
		void destroy_vertex_buffer(VertexBuffer* buffer);
		void destroy_index_buffer(IndexBuffer* buffer);
		void destroy_uniform_buffer(UniformBuffer* buffer);
		void destroy_vertex_array(VertexArray* vertex_array);
		void destroy_rasterizer_state(RasterizerState* state);
		void destroy_sampler_state(SamplerState* state);
		void destroy_depth_stencil_state(DepthStencilState* state);
		void destroy_framebuffer(Framebuffer* framebuffer);
		void destroy_shader(Shader* shader);
		void destory_shader_program(ShaderProgram* program);
		void destroy_graphics_queue(GraphicsQueue* queue);

		// Object Use
		void submit_graphics_queue(GraphicsQueue* queue);
		void submit_compute_queue();
		void submit_graphics_queue(GraphicsQueue* queue);
		void submit_compute_queue();
		void  bind_pipeline_state_object(PipelineStateObject* pso);
		void  bind_texture(Texture* texture, ShaderType shader_stage, uint32_t buffer_slot);
		void  bind_uniform_buffer(UniformBuffer* uniform_buffer, ShaderType shader_stage, uint32_t buffer_slot);
		void  bind_rasterizer_state(RasterizerState* state);
		void  bind_sampler_state(SamplerState* state, ShaderType shader_stage, uint32_t slot);
		void  bind_vertex_array(VertexArray* vertex_array);
		void  bind_framebuffer(Framebuffer* framebuffer);
		void  bind_depth_stencil_state(DepthStencilState* state);
		void  bind_shader_program(ShaderProgram* program);
		void* map_buffer(Buffer* buffer, BufferMapType type);
		void  unmap_buffer(Buffer* buffer);
		void  set_primitive_type(DrawPrimitive primitive);
		void  clear_framebuffer(FramebufferClearTarget clear_target, Vector4 clear_color);
		void  set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y);
		void  swap_buffers();

		// Stateless Methods
		void draw(uint32_t first_index, uint32_t count);
		void draw_indexed(uint32_t index_count);
		void draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex);
		void draw_instanced();
		void draw_indexed_instanced();

		// Getters
		inline ID3D11Device* GetD3D11Device()
		{
			return m_device;
		}

		inline ID3D11DeviceContext* GetD3D11DeviceContext()
		{
			return m_device_context;
		}

		inline Framebuffer* GetFramebufferFromPool(String name)
		{
			return m_framebuffer_map[name];
		}

		inline Texture2D* GetRenderTargetFromPool(String name)
		{
			return m_render_target_map[name];
		}

		inline void AddToRenderTargetPool(String name, Texture2D* texture)
		{
			m_render_target_map[name] = texture;
		}

		inline void AddToFramebufferPool(String name, Framebuffer* framebuffer)
		{
			m_framebuffer_map[name] = framebuffer;
		}

	private:
		DXGI_FORMAT get_dxgi_format(TextureFormat format);

	private:
		ID3D10Blob* create_stub_shader(InputLayout _layout);

		bool				     m_vsync;
		int						 m_video_card_memory;
		Framebuffer*			 m_current_framebuffer;
		Framebuffer*			 m_default_framebuffer;
		Texture2D*				 m_default_render_target;
		Texture2D*				 m_default_depth_target;
		char					 m_video_card_desc[128];
		std::unordered_map<String, Texture2D*> m_render_target_map;
		std::unordered_map<String, Framebuffer*> m_framebuffer_map;
		float					 _window_width;
		float					 _window_height;

		D3D11_PRIMITIVE_TOPOLOGY m_primitive_type;
		ID3D11Device*			 m_device;
		ID3D11DeviceContext*	 m_device_context;
		IDXGISwapChain*			 m_swap_chain;

		// Resource IDs

		uint16 m_texture_res_id;
		uint16 m_buffer_res_id;
		uint16 m_framebuffer_res_id;
		uint16 m_vertex_array_res_id;
		uint16 m_shader_program_res_id;
		uint16 m_sampler_res_id;
	};
} // namespace terminus

#endif
