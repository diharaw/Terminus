#include <graphics/render_device.h>
#include <graphics/command_queue.h>
#include <graphics/command_pool.h>
#include <core/context.h>

#if defined(TERMINUS_OPENGL) || defined(TERMINUS_DIRECT3D11)

TERMINUS_BEGIN_NAMESPACE

namespace gpu_dispatch
{
	typedef void(*DispatchFunction)(RenderDevice&, CommandBuffer&);

	void draw(RenderDevice& device, CommandBuffer& buffer)
	{
		DrawCmdData data;
		buffer.read<DrawCmdData>(data);
		device.draw(data.first_index, data.count);
	}

	void draw_indexed(RenderDevice& device, CommandBuffer& buffer)
	{
		DrawIndexedCmdData data;
		buffer.read<DrawIndexedCmdData>(data);
		device.draw_indexed(data.index_count);
	}

	void draw_indexed_base_vertex(RenderDevice& device, CommandBuffer& buffer)
	{
		DrawIndexedBaseVertexCmdData data;
		buffer.read<DrawIndexedBaseVertexCmdData>(data);
		device.draw_indexed_base_vertex(data.index_count, data.base_index, data.base_vertex);
	}

	void bind_framebuffer(RenderDevice& device, CommandBuffer& buffer)
	{
		BindFramebufferCmdData data;
		buffer.read<BindFramebufferCmdData>(data);
		device.bind_framebuffer(data.framebuffer);

		// temp
		device.set_viewport(0, 0, 0, 0);
	}

	void bind_shader_program(RenderDevice& device, CommandBuffer& buffer)
	{
		BindShaderProgramCmdData data;
		buffer.read<BindShaderProgramCmdData>(data);
		device.bind_shader_program(data.program);
	}

	void bind_sampler_state(RenderDevice& device, CommandBuffer& buffer)
	{
		BindSamplerStateCmdData data;
		buffer.read<BindSamplerStateCmdData>(data);
		device.bind_sampler_state(data.state, data.shader_type, data.slot);
	}

	void bind_vertex_array(RenderDevice& device, CommandBuffer& buffer)
	{
		BindVertexArrayCmdData data;
		buffer.read<BindVertexArrayCmdData>(data);
		device.bind_vertex_array(data.vertex_array);
	}

	void bind_uniform_buffer(RenderDevice& device, CommandBuffer& buffer)
	{
		BindUniformBufferCmdData data;
		buffer.read<BindUniformBufferCmdData>(data);
		device.bind_uniform_buffer(data.buffer, data.shader_type, data.slot);
	}

	void bind_pipeline_state_object(RenderDevice& device, CommandBuffer& buffer)
	{
		BindPipelineStateObjectData data;
		buffer.read<BindPipelineStateObjectData>(data);

		if (data.pso)
			device.bind_pipeline_state_object(data.pso);
	}

	void bind_texture(RenderDevice& device, CommandBuffer& buffer)
	{
		BindTextureCmdData data;
		buffer.read<BindTextureCmdData>(data);

		if (data.texture)
			device.bind_texture(data.texture, data.shader_type, data.slot);
	}

	void copy_uniform_data(RenderDevice& device, CommandBuffer& buffer)
	{
		CopyUniformCmdData data;
		buffer.read<CopyUniformCmdData>(data);

		void* ptr = device.map_buffer(data.buffer, data.map_type);
		memcpy(ptr, data.data, data.size);
		device.unmap_buffer(data.buffer);
	}

	void clear_framebuffer(RenderDevice& device, CommandBuffer& buffer)
	{
		ClearFramebufferCmdData data;
		buffer.read<ClearFramebufferCmdData>(data);
		device.clear_framebuffer(data.clear_target, data.clear_color);
	}

	static const DispatchFunction g_dispatch_table[] =
	{
		&draw,
		&draw_indexed,
		&draw_indexed_base_vertex,
		&bind_framebuffer,
		&bind_shader_program,
		&bind_vertex_array,
		&bind_uniform_buffer,
		&bind_sampler_state,
		&bind_pipeline_state_object,
		&bind_texture,
		&copy_uniform_data,
		&clear_framebuffer
	};
}

CommandQueue* RenderDevice::create_command_queue()
{
	return nullptr;
}

void RenderDevice::destroy_command_queue(CommandQueue* queue)
{

}

void RenderDevice::submit_command_queue(CommandQueue* queue, CommandBuffer* buffers, uint32_t count)
{
	// NOTE: Command Queue unused in Direct3D 11 and OpenGL backends.
	RenderDevice& device = context::get_render_device();

	if (count == 0)
		clear_framebuffer(FramebufferClearTarget::ALL, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	for (int i = 0; i < count; i++)
	{
		CommandBuffer& buffer = buffers[i];
		uint32_t type;

		while (true)
		{
			buffer.read(type);

			if (type >= CommandBuffer::End)
				break;

			gpu_dispatch::g_dispatch_table[type](device, buffer);
		}

		buffer.reset();
	}
}

CommandPool* RenderDevice::create_command_pool()
{
	CommandPool* pool = new CommandPool();
	pool->_allocator = new(&pool->_allocator_mem[0]) PoolAllocator(sizeof(CommandBuffer), 8, COMMAND_POOL_SIZE, &pool->_buffer[0]);
	pool->_num_allocated = 0;
	return pool;
}

void RenderDevice::destroy_command_pool(CommandPool* pool)
{
	TE_SAFE_DELETE(pool);
}

CommandBuffer* RenderDevice::create_command_buffer(CommandPool* pool)
{
	if (pool)
	{
		CommandBuffer* buffer = TE_NEW(pool->_allocator) CommandBuffer();
		ID id = pool->_allocated_list.add();

		CommandBuffer*& ref = pool->_allocated_list.lookup(id);
		ref = buffer;

		buffer->_id = id;
		return buffer;
	}
	else
		return nullptr;
}

void RenderDevice::destroy_command_buffer(CommandPool* pool, CommandBuffer* buffer)
{
	if (pool && buffer)
	{
		pool->_allocated_list.remove(buffer->_id);
		TE_DELETE(buffer, pool->_allocator);
	}
}

void RenderDevice::reset_command_pool(CommandPool* pool)
{
	if (pool)
	{
		for (int i = 0; i < pool->_allocated_list.size(); i++)
		{
			pool->_allocated_list._objects[i]->reset();
		}
	}
}

TERMINUS_END_NAMESPACE

#endif