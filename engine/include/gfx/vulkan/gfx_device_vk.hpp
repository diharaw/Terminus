#pragma once

#include <core/terminus_macros.hpp>
#include <gfx/gfx_descs.hpp>

#include <stl/vector.hpp>
#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>

#define TE_VULKAN_DEBUG

TE_BEGIN_TERMINUS_NAMESPACE

struct VertexBuffer {};
struct IndexBuffer {};
struct UniformBuffer {};
struct StorageBuffer {};
struct IndirectBuffer {};
struct VertexArray {};
struct PipelineState {};
struct CommandBuffer {};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR   capabilities;
	Vector<VkSurfaceFormatKHR> format;
	Vector<VkPresentModeKHR>   present_modes;
};

class GfxDevice
{
public:
	GfxDevice();
	~GfxDevice();

	bool initialize();
	void shutdown();

	// Buffer creation API
	VertexBuffer* create(const VertexBufferDesc& desc);

	// Buffer destruction API
	void destroy(VertexBuffer* buffer);

private:
	bool create_instance();
	bool setup_debug_callback();
	bool choose_physical_device();
	bool check_device_extension_support(VkPhysicalDevice device);
	bool is_device_suitable(VkPhysicalDevice device);
	bool create_surface();
	void query_swap_chain_support(VkPhysicalDevice device, SwapChainSupportDetails& details);
	bool create_logical_device();
	bool create_queues();
	bool check_validation_layer_support();
	void required_extensions(Vector<const char*>& extensions);

private:
	DeviceProperties m_device_properties;
	DeviceLimits	 m_device_limits;
	VkInstance		 m_instance;
	VkDevice		 m_device;
	VkQueue			 m_graphics_queue;
	VkQueue			 m_compute_queue;
	VkQueue			 m_transfer_queue;
	VkPhysicalDevice m_physical_device;
	VkSurfaceKHR	 m_surface;

#if defined(TE_VULKAN_DEBUG)
	VkDebugReportCallbackEXT m_debug_callback;
#endif
};

TE_END_TERMINUS_NAMESPACE