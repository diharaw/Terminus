#pragma once

#include <Core/config.h>

#if defined(TERMINUS_OPENGL)

#include <vector>
#include <graphics/render_common.h>
#include <resource/asset_common.h>
#include <core/types.h>
#include <core/global.h>
#include <container/packed_array.h>
#include <platform/platform.h>
#include <memory/pool_allocator.h>
#include <graphics/graphics_types_gl.h>

#define MAX_TEXTURE_1D 1
#define MAX_TEXTURE_2D 1024
#define MAX_TEXTURE_3D 1
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
	struct CommandQueue;
	struct CommandPool;
	struct CommandBuffer;

	class RenderDevice
	{

	private:
		// Stateful Object References for Window mode switching. GL only.
		PackedArray<VertexArray*, MAX_VERTEX_ARRAY>	m_VertexArrayPool;
		PackedArray<Framebuffer*, MAX_FRAMEBUFFER> 	m_FramebufferPool;
        
        // Resource IDs
        
        uint16 _texture_res_id;
        uint16 _buffer_res_id;
        uint16 _framebuffer_res_id;
        uint16 _vertex_array_res_id;
        uint16 _shader_program_res_id;
        uint16 _sampler_res_id;

	public:

		RenderDevice();
		~RenderDevice();
		void initialize();
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
		CommandQueue* create_command_queue();
		CommandPool* create_command_pool();
		CommandBuffer* create_command_buffer(CommandPool* pool);

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
		void destroy_command_queue(CommandQueue* queue);
		void destroy_command_pool(CommandPool* pool);
		void destroy_command_buffer(CommandPool* pool, CommandBuffer* buffer);

		// Object Use
		void reset_command_pool(CommandPool* pool);
		void submit_command_queue(CommandQueue* queue, CommandBuffer* buffers, uint32_t count);
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

		inline Framebuffer* find_framebuffer_from_pool(String name)
		{
			return _framebuffer_map[name];
		}

		inline Texture2D* find_render_target_from_pool(String name)
		{
			return _render_target_map[name];
		}

		inline void add_to_pool(String name, Texture2D* texture)
		{
			_render_target_map[name] = texture;
		}

		inline void add_to_pool(String name, Framebuffer* framebuffer)
		{
			_framebuffer_map[name] = framebuffer;
		}

	private:
		Framebuffer*							 _current_framebuffer;
		Platform*								 _platform;
		GLenum									 _primitive_type;
		ShaderProgram*							 _current_program;
		GLuint									 _last_sampler_location;
		int										 _drawable_width;
		int										 _drawable_height;
		int										 _width;
		int										 _height;
		std::unordered_map<String, Texture2D*>	 _render_target_map;
		std::unordered_map<String, Framebuffer*> _framebuffer_map;
	};
} // namespace terminus

#endif
