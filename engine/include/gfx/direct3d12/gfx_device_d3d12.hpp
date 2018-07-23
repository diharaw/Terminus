#pragma once

#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/gfx_descs.hpp>
#include <gfx/gfx_types.hpp>
#include <stl/string_buffer.hpp>
#include <stl/vector.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

#define FRAME_COUNT 3

class GfxDevice
{
public:
	GfxDevice();
	~GfxDevice();

	bool initialize();
	void shutdown();

	Framebuffer* accquire_next_framebuffer(SemaphoreGPU* signal_semaphore);

	// Resource creation
	Texture*	    create_texture(const TextureCreateDesc& desc);
	Buffer*		    create_buffer(const BufferCreateDesc& desc);
	VertexArray*    create_vertex_array(const VertexArrayCreateDesc& desc);
	InputLayout*    create_input_layout(const InputLayoutCreateDesc& desc);
	Framebuffer*    create_framebuffer(const FramebufferCreateDesc& desc);
	Shader*		    create_shader_from_binary(const BinaryShaderCreateDesc& desc);
	Shader*		    create_shader_from_source(const SourceShaderCreateDesc& desc);
	PipelineLayout* create_pipeline_layout(const PipelineLayoutCreateDesc& desc);
	PipelineState*  create_pipeline_state(const PipelineStateCreateDesc& desc);
	CommandPool*	create_command_pool(CommandPoolType type);
	CommandBuffer*  create_command_buffer(CommandPool* cmd_pool);
	Fence*		    create_fence();
	SemaphoreGPU*   create_semaphore();

	// Resource destruction
	void destroy_texture(Texture* texture);
	void destroy_buffer(Buffer* buffer);
	void destroy_vertex_array(VertexArray* vertex_array);
	void destroy_input_layout(InputLayout* input_layout);
	void destroy_framebuffer(Framebuffer* framebuffer);
	void destroy_shader(Shader* shader);
	void destroy_pipeline_layout(PipelineLayout* pipeline_layout);
	void destory_pipeline_state(PipelineState* pipeline_state);
	void destroy_command_pool(CommandPool* cmd_pool);
	void destroy_fence(Fence* fence);
	void destroy_semaphore(SemaphoreGPU* semaphore);
	void reset_command_pool(CommandPool* cmd_pool);

	// Resource updates
	void  update_texture(Texture* texture, uint32_t mip_slice, uint32_t array_slice, void* data);
	void  update_buffer(Buffer* buffer, size_t size, void* data);
	void* map_buffer(Buffer* buffer, size_t offset, size_t size);
	void  unmap_buffer(Buffer* buffer);

	// Synchronization
	void wait_for_fences(uint32_t count, Fence** fences, uint64_t timeout);
	void check_fences(uint32_t count, Fence** fences, bool* status);
	void wait_for_idle();

	// Command encoding
	void cmd_bind_vertex_array(CommandBuffer* cmd, VertexArray* vertex_array);
	void cmd_bind_framebuffer(CommandBuffer* cmd, Framebuffer* framebuffer);
	void cmd_bind_pipeline_state(CommandBuffer* cmd, PipelineState* pipeline_state);
	void cmd_resource_barrier(CommandBuffer* cmd);
	void cmd_draw(CommandBuffer* cmd, uint32_t  vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance);
	void cmd_draw_indexed(CommandBuffer* cmd, uint32_t index_count, uint32_t instance_Count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance);
	void cmd_draw_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset, uint32_t draw_count, uint32_t stride);
	void cmd_draw_indexed_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset, uint32_t draw_count, uint32_t stride);
	void cmd_dispatch(CommandBuffer* cmd, uint32_t x, uint32_t y, uint32_t z);
	void cmd_dispatch_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset);

	// Command submission
	void submit_graphics(uint32_t cmd_buf_count,
						 CommandBuffer** command_buffers,
						 uint32_t wait_sema_count,
						 SemaphoreGPU** wait_semaphores,
						 uint32_t signal_sema_count,
						 SemaphoreGPU** signal_semaphores,
						 Fence* fence);

	void submit_compute(uint32_t cmd_buf_count,
						CommandBuffer** command_buffers,
						uint32_t wait_sema_count,
						SemaphoreGPU** wait_semaphores,
						uint32_t signal_sema_count,
						SemaphoreGPU** signal_semaphores,
						Fence* fence);

	// Presentation
	void present(uint32_t wait_sema_count, SemaphoreGPU** wait_semaphores);

private:
	void enable_debug_layer();
	void get_hardware_adapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
	void create_device();
	void create_command_queues();
	void create_swap_chain();
	void create_descriptor_heaps();
	void query_descriptor_sizes();
	void create_framebuffers();
	
private:
	ComPtr<IDXGIFactory4>        m_dxgi_factory;
	ComPtr<IDXGISwapChain3>      m_swap_chain;
	ComPtr<ID3D12Device>         m_device;
	Texture*                     m_back_buffer_rt[FRAME_COUNT];
	Framebuffer*                 m_back_buffer_fbo[FRAME_COUNT];
	ComPtr<ID3D12CommandQueue>   m_graphics_cmd_queue;
	ComPtr<ID3D12CommandQueue>   m_compute_cmd_queue;
	ComPtr<ID3D12CommandQueue>   m_copy_cmd_queue;
	ComPtr<ID3D12DescriptorHeap> m_rtv_heap;
	ComPtr<ID3D12DescriptorHeap> m_dsv_heap;
	ComPtr<ID3D12DescriptorHeap> m_cbv_srv_uav_heap_cpu;
	ComPtr<ID3D12DescriptorHeap> m_cbv_srv_uav_heap_gpu;
	ComPtr<ID3D12DescriptorHeap> m_sampler_heap_cpu;
	ComPtr<ID3D12DescriptorHeap> m_sampler_heap_gpu;
	UINT                         m_dxgi_factory_flags;
	UINT                         m_rtv_descriptor_size;
	UINT                         m_dsv_descriptor_size;
	UINT                         m_cbv_srv_uav_descriptor_size;
	UINT                         m_sampler_descriptor_size;
	UINT                         m_frame_index;
};

TE_END_TERMINUS_NAMESPACE

#endif