#pragma once

#include <core/terminus_macros.hpp>
#include <gfx/gfx_descs.hpp>
#include <gfx/gfx_types.hpp>
#include <stl/vector.hpp>
#include <vulkan/vulkan.h>
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
	void shutdown();

	Texture* create_texture(const TextureDesc& desc);
	void destroy_texture(Texture* texture);
	Buffer* create_buffer(const BufferDesc& desc);
	void destroy_buffer(Buffer* buffer);
	Framebuffer* create_framebuffer(const FramebufferDesc& desc);
	void destroy_framebuffer(Framebuffer* framebuffer);

private:
	// Instance
	bool	 create_instance();
	
	// Device
	bool	 choose_physical_device();
	bool	 is_device_suitable(VkPhysicalDevice device);
	
	// Surface
	bool	 create_surface();

	// Queues
	bool	 create_logical_device();
	// Checks if the bit flags of the given queue has the required combination. 1 = must have, 0 = must NOT have, -1 = don't care. 
	bool	 is_queue_compatible(VkQueueFlags current_queue_flags, int32_t graphics, int32_t compute, int32_t transfer);
	bool	 find_queues();

	// Swap chain
	void	 query_swap_chain_support(VkPhysicalDevice device, SwapChainSupportDetails& details);
	VkSurfaceFormatKHR choose_swap_surface_format(const Vector<VkSurfaceFormatKHR>& available_formats);
	VkPresentModeKHR choose_swap_present_mode(const Vector<VkPresentModeKHR>& available_modes);
	VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool	 create_swap_chain();
	void	 shutdown_swap_chain();

	// Extensions
	bool	 check_device_extension_support(VkPhysicalDevice device);
	void	 required_extensions(Vector<const char*>& extensions);

	// Validation layers
	bool	 setup_debug_callback();
	bool	 check_validation_layer_support();
	VkResult create_debug_report_callback_ext(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void	 destroy_debug_report_callback_ext(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

private:
	DeviceProperties m_device_properties;
	DeviceLimits	 m_device_limits;
	QueueInfos		 m_queue_infos;
	VkInstance		 m_instance;
	VkDevice		 m_device;
	VkQueue			 m_graphics_queue;
	VkQueue			 m_compute_queue;
	VkQueue			 m_transfer_queue;
	VkQueue			 m_presentation_queue;
	VkPhysicalDevice m_physical_device;
	VkSurfaceKHR	 m_surface;
	VkSwapchainKHR	 m_swap_chain;
	VkFormat		 m_swap_chain_image_format;
	VkExtent2D		 m_swap_chain_extent;
	Vector<Texture*> m_swap_chain_textures;
	Vector<Framebuffer*> m_swap_chain_framebuffers;

#if defined(TE_VULKAN_DEBUG)
	VkDebugReportCallbackEXT m_debug_callback;
#endif
};

TE_END_TERMINUS_NAMESPACE