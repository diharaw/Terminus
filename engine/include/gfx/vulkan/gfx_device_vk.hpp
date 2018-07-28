#pragma once

#if defined(TE_GFX_BACKEND_VK)

#include <gfx/gfx_descs.hpp>
#include <gfx/gfx_types.hpp>
#include <stl/string_buffer.hpp>
#include <stl/vector.hpp>
#include <SDL_vulkan.h>

#define TE_VULKAN_DEBUG

TE_BEGIN_TERMINUS_NAMESPACE

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR   capabilities;
	Vector<VkSurfaceFormatKHR> format;
	Vector<VkPresentModeKHR>   present_modes;
};

struct QueueInfos
{
	// Most ideal queue = 3
	// Second most ideal queue = 2
	// Queue for minimum functionality = 1
	// Not found = 0

	int32_t graphics_queue_index = -1;
	int32_t graphics_queue_quality = 0;
	int32_t compute_queue_index = -1;
	int32_t compute_queue_quality = 0;
	int32_t transfer_queue_index = -1;
	int32_t transfer_queue_quality = 0;
	int32_t presentation_queue_index = -1;
	int32_t queue_count = 0;
	VkDeviceQueueCreateInfo infos[32];
};

class GfxDevice
{
public:
	GfxDevice();
	~GfxDevice();

	bool initialize();
	void recreate_swap_chain();
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
	void  update_texture(Texture* texture, uint32_t mip_slice , uint32_t array_slice, void* data);
	void  update_buffer(Buffer* buffer, size_t size, void* data);
	void* map_buffer(Buffer* buffer, size_t offset, size_t size);
	void  unmap_buffer(Buffer* buffer);

	// Synchronization
	void wait_for_fences(uint32_t count, Fence** fences, uint64_t timeout);
	void check_fences(uint32_t count, Fence** fences, bool* status);
	void wait_for_idle();

	// Command encoding
	void cmd_begin_recording(CommandBuffer* cmd);
	void cmd_end_recording(CommandBuffer* cmd);
	void cmd_set_viewport(CommandBuffer* cmd, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	void cmd_bind_vertex_array(CommandBuffer* cmd, VertexArray* vertex_array);
	void cmd_bind_framebuffer(CommandBuffer* cmd, Framebuffer* framebuffer, ClearValue* color_clear_values, ClearValue ds_clear_value);
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
	// Instance
	bool create_instance();
	
	// Device
	bool choose_physical_device();
	bool is_device_suitable(VkPhysicalDevice device);
	
	// Surface
	bool create_surface();

	// Queues
	bool create_logical_device();
	// Checks if the bit flags of the given queue has the required combination. 1 = must have, 0 = must NOT have, -1 = don't care. 
	bool is_queue_compatible(VkQueueFlags current_queue_flags, int32_t graphics, int32_t compute, int32_t transfer);
	bool find_queues();

	// Swap chain
	void			   query_swap_chain_support(VkPhysicalDevice device, SwapChainSupportDetails& details);
	VkSurfaceFormatKHR choose_swap_surface_format(const Vector<VkSurfaceFormatKHR>& available_formats);
	VkPresentModeKHR   choose_swap_present_mode(const Vector<VkPresentModeKHR>& available_modes);
	VkExtent2D		   choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool			   create_swap_chain();
	void			   shutdown_swap_chain();

	// Extensions
	bool	 check_device_extension_support(VkPhysicalDevice device);
	void	 required_extensions(Vector<const char*>& extensions);

	// Validation layers
	bool	 setup_debug_callback();
	bool	 check_validation_layer_support();
	VkResult create_debug_report_callback_ext(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void	 destroy_debug_report_callback_ext(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	// Helpers
	Texture* create_swap_chain_texture(uint32_t w, uint32_t h, VkImage image, VkFormat format, VkSampleCountFlagBits sample_count);
	void	 calc_image_size_and_extents(Texture* texture, uint32_t mip_level, uint32_t& w, uint32_t& h, uint32_t& d, size_t& size);

private:
	struct DeviceProperties
	{
		uint32_t	   vendor_id;
		uint32_t	   driver;
		StringBuffer32 name;
		StringBuffer32 type;
	};

	struct DeviceLimits
	{

	};

	DeviceProperties	   m_device_properties;
	DeviceLimits		   m_device_limits;
	QueueInfos			   m_queue_infos;
	VkInstance			   m_instance;
	VkDevice			   m_device;
	VkQueue				   m_graphics_queue;
	VkQueue				   m_compute_queue;
	VkQueue				   m_transfer_queue;
	VkQueue				   m_presentation_queue;
	VkPhysicalDevice	   m_physical_device;
	VkSurfaceKHR		   m_surface;
	VkSwapchainKHR		   m_swap_chain;
	VkFormat			   m_swap_chain_image_format;
	VkExtent2D			   m_swap_chain_extent;
	Vector<Texture*>	   m_swap_chain_textures;
	Vector<Framebuffer*>   m_swap_chain_framebuffers;
	struct VmaAllocator_T* m_allocator;
	uint32_t			   m_framebuffer_index;

#if defined(TE_VULKAN_DEBUG)
	VkDebugReportCallbackEXT m_debug_callback;
#endif
};

TE_END_TERMINUS_NAMESPACE

#endif