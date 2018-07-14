#if defined(TE_GFX_BACKEND_VK)

#define VMA_IMPLEMENTATION
#include <gfx/vulkan/vk_mem_alloc.h>
#include <gfx/vulkan/gfx_device_vk.hpp>
#include <gfx/vulkan/gfx_types_vk.hpp>
#include <gfx/vulkan/gfx_initializers_vk.hpp>
#include <core/engine_core.hpp>
#include <io/logger.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// -----------------------------------------------------------------------------------------------------------------------------------

const char* kDeviceTypes[] = 
{
	"VK_PHYSICAL_DEVICE_TYPE_OTHER",
	"VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_CPU"
};

// -----------------------------------------------------------------------------------------------------------------------------------

const char* kVendorNames[] = 
{
	"Unknown",
	"AMD",
	"IMAGINATION",
	"NVIDIA",
	"ARM",
	"QUALCOMM",
	"INTEL"
};

// -----------------------------------------------------------------------------------------------------------------------------------

const char* kValidationLayers[] =
{
	"VK_LAYER_LUNARG_standard_validation"
};

// -----------------------------------------------------------------------------------------------------------------------------------

const char* kDeviceExtensions[] = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// -----------------------------------------------------------------------------------------------------------------------------------

enum VkVendor
{
	VK_VENDOR_AMD = 0x1002,
	VK_VENDOR_IMAGINATION = 0x1010,
	VK_VENDOR_NVIDIA = 0x10DE,
	VK_VENDOR_ARM = 0x13B5,
	VK_VENDOR_QUALCOMM = 0x5143,
	VK_VENDOR_INTEL = 0x8086,
};

// -----------------------------------------------------------------------------------------------------------------------------------

const char* get_vendor_name(uint32_t id)
{
	switch (id)
	{
	case 0x1002:
		return kVendorNames[1];
	case 0x1010:
		return kVendorNames[2];
	case 0x10DE:
		return kVendorNames[3];
	case 0x13B5:
		return kVendorNames[4];
	case 0x5143:
		return kVendorNames[5];
	case 0x8086:
		return kVendorNames[6];
	default:
		return kVendorNames[0];
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

const VkImageType kImageTypeTable[] =
{
	VK_IMAGE_TYPE_1D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_3D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
	VK_IMAGE_TYPE_2D,
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkFormat kFormatTable[] = 
{
	VK_FORMAT_R32G32B32_SFLOAT,
	VK_FORMAT_R32G32B32A32_SFLOAT,
	VK_FORMAT_R32G32B32_UINT,
	VK_FORMAT_R32G32B32A32_UINT,
	VK_FORMAT_R32G32B32_SINT,
	VK_FORMAT_R32G32B32A32_SINT,
	VK_FORMAT_R16G16_SFLOAT,
	VK_FORMAT_R16G16B16_SFLOAT,
	VK_FORMAT_R16G16B16A16_SFLOAT,
	VK_FORMAT_R16G16B16_UINT,
	VK_FORMAT_R16G16B16A16_UINT,
	VK_FORMAT_R16G16B16_SINT,
	VK_FORMAT_R16G16B16A16_SINT,
	VK_FORMAT_R8G8B8_UNORM,
	VK_FORMAT_R8G8B8A8_UNORM,
	VK_FORMAT_R8G8B8_SRGB,
	VK_FORMAT_R8G8B8A8_SRGB,
	VK_FORMAT_R8G8B8_SNORM,
	VK_FORMAT_R8G8B8A8_SNORM,
	VK_FORMAT_R8G8B8_SINT,
	VK_FORMAT_R8G8B8A8_SINT,
	VK_FORMAT_R8G8B8_UINT,
	VK_FORMAT_R8G8B8A8_UINT,
	VK_FORMAT_R8_UNORM,
	VK_FORMAT_R8_SNORM,
	VK_FORMAT_D32_SFLOAT_S8_UINT,
	VK_FORMAT_D24_UNORM_S8_UINT,
	VK_FORMAT_D16_UNORM,
	VK_FORMAT_R32G32_SFLOAT,
	VK_FORMAT_R16_SFLOAT
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkIndexType kIndexTypeTable[] =
{
	VK_INDEX_TYPE_MAX_ENUM,
	VK_INDEX_TYPE_MAX_ENUM,
	VK_INDEX_TYPE_MAX_ENUM,
	VK_INDEX_TYPE_MAX_ENUM,
	VK_INDEX_TYPE_UINT16,
	VK_INDEX_TYPE_UINT32,
	VK_INDEX_TYPE_MAX_ENUM
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkPipelineBindPoint kPipelineBindPointTable[] =
{
	VK_PIPELINE_BIND_POINT_GRAPHICS,
	VK_PIPELINE_BIND_POINT_COMPUTE
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkCullModeFlags kCullModeTable[] =
{
	VK_CULL_MODE_FRONT_BIT,
	VK_CULL_MODE_BACK_BIT,
	VK_CULL_MODE_FRONT_AND_BACK,
	VK_CULL_MODE_NONE
};

const VkPolygonMode kFillModeTable[] =
{
	VK_POLYGON_MODE_FILL,
	VK_POLYGON_MODE_LINE
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkCompareOp kCompareOpTable[] =
{
	VK_COMPARE_OP_NEVER,
	VK_COMPARE_OP_LESS,
	VK_COMPARE_OP_EQUAL,
	VK_COMPARE_OP_LESS_OR_EQUAL,
	VK_COMPARE_OP_GREATER,
	VK_COMPARE_OP_NOT_EQUAL,
	VK_COMPARE_OP_GREATER_OR_EQUAL,
	VK_COMPARE_OP_ALWAYS
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkStencilOp kStencilOpTable[] =
{
	VK_STENCIL_OP_KEEP,
	VK_STENCIL_OP_ZERO,
	VK_STENCIL_OP_REPLACE,
	VK_STENCIL_OP_INCREMENT_AND_CLAMP,
	VK_STENCIL_OP_DECREMENT_AND_CLAMP,
	VK_STENCIL_OP_INVERT,
	VK_STENCIL_OP_INCREMENT_AND_WRAP,
	VK_STENCIL_OP_DECREMENT_AND_WRAP,
};

// -----------------------------------------------------------------------------------------------------------------------------------

const size_t kPixelSizes[] =
{
	// @TODO: Add compressed formats
	sizeof(float) * 3,
	sizeof(float) * 4,
	sizeof(uint32_t) * 3,
	sizeof(uint32_t) * 4,
	sizeof(int32_t) * 3,
	sizeof(int32_t) * 4,
	sizeof(uint16_t) * 2,
	sizeof(uint16_t) * 3,
	sizeof(uint16_t) * 4,
	sizeof(uint16_t) * 3,
	sizeof(uint16_t) * 4,
	sizeof(uint16_t) * 3,
	sizeof(uint16_t) * 4,
	sizeof(uint8_t) * 3,
	sizeof(uint8_t) * 4,
	sizeof(uint8_t) * 3,
	sizeof(uint8_t) * 4,
	sizeof(uint8_t) * 3,
	sizeof(uint8_t) * 4,
	sizeof(uint8_t) * 3,
	sizeof(uint8_t) * 4,
	sizeof(uint8_t) * 3,
	sizeof(uint8_t) * 4,
	sizeof(uint8_t),
	sizeof(uint8_t),
	sizeof(float),
	sizeof(float),
	sizeof(uint16_t),
	sizeof(float) * 2,
	sizeof(uint16_t)
};

// -----------------------------------------------------------------------------------------------------------------------------------

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT obj_type,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layer_prefix,
	const char* msg,
	void* user_data)
{
	TE_LOG_ERROR("Validation Layer : " + std::string(msg));

	return VK_FALSE;
}

// -----------------------------------------------------------------------------------------------------------------------------------

// Helpers Declaration
VkRenderPass create_render_pass(VkDevice device, VmaAllocator allocator, const FramebufferCreateDesc& desc);
bool allocate_buffer(VkDevice device, VmaAllocator allocator, VkBufferCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkBuffer& buffer, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info);
bool allocate_image(VkDevice device, VmaAllocator allocator, VkImageCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkImage& image, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info);
bool create_image_view(VkDevice device, VmaAllocator allocator, Texture* texture, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_layer, uint32_t layer_count, VkImageView& image_view);

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::GfxDevice() : m_framebuffer_index(0) {}

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::~GfxDevice() {}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::initialize()
{
	// Create Vulkan instance
	if (!create_instance())
		return false;

#if defined(TE_VULKAN_DEBUG)
	// Setup debug callback
	if (!setup_debug_callback())
		return false;
#endif

	// Create surface
	if (!create_surface())
		return false;

	// Choose physical device
	if (!choose_physical_device())
		return false;

	// Create logical device
	if (!create_logical_device())
		return false;

	// Create swap chain
	if (!create_swap_chain())
		return false;

	VmaAllocatorCreateInfo allocator_info = {};
	allocator_info.physicalDevice = m_physical_device;
	allocator_info.device = m_device;

	vmaCreateAllocator(&allocator_info, &m_allocator);

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::shutdown()
{
	vkDeviceWaitIdle(m_device);

	vmaDestroyAllocator(m_allocator);

	destroy_debug_report_callback_ext(m_instance, m_debug_callback, nullptr);

	shutdown_swap_chain();

	vkDestroyDevice(m_device, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::create_instance()
{
#if defined(TE_VULKAN_DEBUG)
	if (!check_validation_layer_support())
		TE_LOG_ERROR("Validation layers requested, but not available!");
#endif

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Terminus Engine";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "Terminus Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_info = {};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;

	// Setup extensions
	Vector<const char*> extensions;
	required_extensions(extensions);

	instance_info.enabledExtensionCount = extensions.size();
	instance_info.ppEnabledExtensionNames = &extensions[0];

#if defined(TE_VULKAN_DEBUG)
	instance_info.enabledLayerCount = static_cast<uint32_t>(sizeof(kValidationLayers)/sizeof(const char*));
	instance_info.ppEnabledLayerNames = &kValidationLayers[0];
#else
	instance_info.enabledLayerCount = 0;
#endif

	if (vkCreateInstance(&instance_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create Vulkan instance");
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::choose_physical_device()
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

	if (device_count == 0)
	{
		TE_LOG_ERROR("No Vulkan-compatible devices found!");
		return false;
	}

	VkPhysicalDevice devices[32];
	vkEnumeratePhysicalDevices(m_instance, &device_count, &devices[0]);

	TE_LOG_INFO("Number of Physical Devices found: " + std::to_string(device_count));

	for (uint32_t i = 0; i < device_count; i++)
	{
		VkPhysicalDevice& device = devices[i];
		
		if (is_device_suitable(device))
		{
			m_physical_device = device;
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::is_device_suitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	uint32_t vendorId = properties.vendorID;

	// For now, look for a Discrete GPU.
	// TODO: Make device search look for the most powerful GPU in the system, whether it is discrete or not.
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		bool extensions_supported = check_device_extension_support(device);
		SwapChainSupportDetails details;
		query_swap_chain_support(device, details);

		if (details.format.size() > 0 && details.present_modes.size() > 0 && extensions_supported)
		{
			m_device_properties.vendor_id = properties.vendorID;
			m_device_properties.name = properties.deviceName;
			m_device_properties.type = kDeviceTypes[properties.deviceType];
			m_device_properties.driver = properties.driverVersion;

			std::cout << std::endl;
			std::cout << "Vendor: " << get_vendor_name(properties.vendorID) << std::endl;
			std::cout << "Name: " << properties.deviceName << std::endl;
			std::cout << "Type: " << kDeviceTypes[properties.deviceType] << std::endl;
			std::cout << "Driver: " << properties.driverVersion << std::endl;

			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::create_surface()
{
	return SDL_Vulkan_CreateSurface((SDL_Window*)global::application()->handle(), m_instance, &m_surface);
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::is_queue_compatible(VkQueueFlags current_queue_flags, int32_t graphics, int32_t compute, int32_t transfer)
{
	if (graphics == 1)
	{
		// If you need graphics, and queue doesn't have it...
		if (!(current_queue_flags & VK_QUEUE_GRAPHICS_BIT))
			return false;
	}
	else if (graphics == 0)
	{
		// If you don't need graphics, but queue has it...
		if (current_queue_flags & VK_QUEUE_GRAPHICS_BIT)
			return false;
	}

	if (compute == 1)
	{
		// If you need compute, and queue doesn't have it...
		if (!(current_queue_flags & VK_QUEUE_COMPUTE_BIT))
			return false;
	}
	else if (compute == 0)
	{
		// If you don't need compute, but queue has it...
		if (current_queue_flags & VK_QUEUE_COMPUTE_BIT)
			return false;
	}

	if (transfer == 1)
	{
		// If you need transfer, and queue doesn't have it...
		if (!(current_queue_flags & VK_QUEUE_TRANSFER_BIT))
			return false;
	}
	else if (transfer == 0)
	{
		// If you don't need transfer, but queue has it...
		if (current_queue_flags & VK_QUEUE_TRANSFER_BIT)
			return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::find_queues()
{
	uint32_t family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &family_count, nullptr);

	std::cout << std::endl;
	std::cout << "Number of Queue families: " << family_count << std::endl;

	VkQueueFamilyProperties families[32];
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &family_count, &families[0]);

	for (uint32_t i = 0; i < family_count; i++)
	{
		VkQueueFlags bits = families[i].queueFlags;

		std::cout << std::endl;
		std::cout << "Family " << i << std::endl;
		std::cout << "Supported Bits: " << "VK_QUEUE_GRAPHICS_BIT: " << ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "1" : "0") << ", " << "VK_QUEUE_COMPUTE_BIT: " << ((families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) ? "1" : "0") << ", " << "VK_QUEUE_TRANSFER_BIT: " << ((families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) ? "1" : "0") << std::endl;
		std::cout << "Number of Queues: " << families[i].queueCount << std::endl;

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, m_surface, &present_support);

		// Look for Presentation Queue
		if (present_support && m_queue_infos.presentation_queue_index == -1)
		{
			m_queue_infos.presentation_queue_index = i;
		}
		// Look for a graphics queue if the ideal one isn't found yet.
		if (m_queue_infos.graphics_queue_quality != 3)
		{
			if (is_queue_compatible(bits, 1, 1, 1))
			{
				// Ideally, a queue that supports everything.
				m_queue_infos.graphics_queue_index = i;
				m_queue_infos.graphics_queue_quality = 3;
			}
			else if (is_queue_compatible(bits, 1, -1, -1))
			{
				// If not, a queue that supports at least graphics.
				m_queue_infos.graphics_queue_index = i;
				m_queue_infos.graphics_queue_quality = 1;
			}
		}

		// Look for a compute queue if the ideal one isn't found yet.
		if (m_queue_infos.compute_queue_quality != 3)
		{
			if (is_queue_compatible(bits, 0, 1, 0))
			{
				// Ideally, a queue that only supports compute (for asynchronous compute).
				m_queue_infos.compute_queue_index = i;
				m_queue_infos.compute_queue_quality = 3;
			}
			else if (is_queue_compatible(bits, 0, 1, 1))
			{
				// Else, a queue that supports compute and transfer only (might allow asynchronous compute. Have to check).
				m_queue_infos.compute_queue_index = i;
				m_queue_infos.compute_queue_quality = 2;
			}
			else if (is_queue_compatible(bits, -1, 1, -1) && m_queue_infos.compute_queue_quality == 0)
			{
				// If not, a queue that supports at least graphics
				m_queue_infos.compute_queue_index = i;
				m_queue_infos.compute_queue_quality = 1;
			}
		}

		// Look for a Transfer queue if the ideal one isn't found yet.
		if (m_queue_infos.transfer_queue_quality != 3)
		{
			if (is_queue_compatible(bits, 0, 0, 1))
			{
				// Ideally, a queue that only supports transfer (for DMA).
				m_queue_infos.transfer_queue_index = i;
				m_queue_infos.transfer_queue_quality = 3;
			}
			else if (is_queue_compatible(bits, 0, 1, 1))
			{
				// Else, a queue that supports compute and transfer only.
				m_queue_infos.transfer_queue_index = i;
				m_queue_infos.transfer_queue_quality = 2;
			}
			else if (is_queue_compatible(bits, -1, -1, 1) && m_queue_infos.transfer_queue_quality == 0)
			{
				// If not, a queue that supports at least graphics
				m_queue_infos.transfer_queue_index = i;
				m_queue_infos.transfer_queue_quality = 1;
			}
		}
	}

	// Print summary of findings.
	if (m_queue_infos.presentation_queue_index == -1)
	{
#if defined(TE_VULKAN_DEBUG)
		std::cout << "No Presentation Queue Found" << std::endl;
#endif
		return false;
	}

	if (m_queue_infos.graphics_queue_quality == 0)
	{
#if defined(TE_VULKAN_DEBUG)
		std::cout << "No Graphics Queue Found" << std::endl;
#endif
		return false;
	}
#if defined(TE_VULKAN_DEBUG)
	if (m_queue_infos.graphics_queue_quality == 1)
		std::cout << "Found Graphics Queue Family (Average)" << std::endl;
	if (m_queue_infos.graphics_queue_quality == 2)
		std::cout << "Found Graphics Queue Family (Near-Ideal)" << std::endl;
	if (m_queue_infos.graphics_queue_quality == 3)
		std::cout << "Found Graphics Queue Family (Ideal)" << std::endl;

	if (m_queue_infos.compute_queue_quality == 0)
		std::cout << "No Compute Queue Found" << std::endl;
	if (m_queue_infos.compute_queue_quality == 1)
		std::cout << "Found Compute Queue Family (Average)" << std::endl;
	if (m_queue_infos.compute_queue_quality == 2)
		std::cout << "Found Compute Queue Family (Near-Ideal)" << std::endl;
	if (m_queue_infos.compute_queue_quality == 3)
		std::cout << "Found Compute Queue Family (Ideal)" << std::endl;

	if (m_queue_infos.transfer_queue_quality == 0)
		std::cout << "No Transfer Queue Found" << std::endl;
	if (m_queue_infos.transfer_queue_quality == 1)
		std::cout << "Found Transfer Queue Family (Average)" << std::endl;
	if (m_queue_infos.transfer_queue_quality == 2)
		std::cout << "Found Transfer Queue Family (Near-Ideal)" << std::endl;
	if (m_queue_infos.transfer_queue_quality == 3)
		std::cout << "Found Transfer Queue Family (Ideal)" << std::endl;
#endif

	float priority = 1.0f;

	VkDeviceQueueCreateInfo presentation_queue_info = {};
	presentation_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	presentation_queue_info.queueFamilyIndex = m_queue_infos.presentation_queue_index;
	presentation_queue_info.queueCount = 1;
	presentation_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo graphics_queue_info = {};
	graphics_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphics_queue_info.queueFamilyIndex = m_queue_infos.graphics_queue_index;
	graphics_queue_info.queueCount = 1;
	graphics_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo compute_queue_info = {};
	compute_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	compute_queue_info.queueFamilyIndex = m_queue_infos.compute_queue_index;
	compute_queue_info.queueCount = 1;
	compute_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo transfer_queue_info = {};
	transfer_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	transfer_queue_info.queueFamilyIndex = m_queue_infos.transfer_queue_index;
	transfer_queue_info.queueCount = 1;
	transfer_queue_info.pQueuePriorities = &priority;

	m_queue_infos.infos[m_queue_infos.queue_count++] = presentation_queue_info;

	if (m_queue_infos.graphics_queue_index != m_queue_infos.presentation_queue_index)
		m_queue_infos.infos[m_queue_infos.queue_count++] = graphics_queue_info;

	if (m_queue_infos.compute_queue_index != m_queue_infos.presentation_queue_index && m_queue_infos.compute_queue_index != m_queue_infos.graphics_queue_index)
		m_queue_infos.infos[m_queue_infos.queue_count++] = compute_queue_info;

	if (m_queue_infos.transfer_queue_index != m_queue_infos.presentation_queue_index && m_queue_infos.transfer_queue_index != m_queue_infos.graphics_queue_index && m_queue_infos.transfer_queue_index != m_queue_infos.compute_queue_index)
		m_queue_infos.infos[m_queue_infos.queue_count++] = transfer_queue_info;

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::create_logical_device()
{
	if (!find_queues())
		return false;

	if (m_queue_infos.queue_count == 0)
		return false;

	VkPhysicalDeviceFeatures features = {};

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pQueueCreateInfos = &m_queue_infos.infos[0];
	device_info.queueCreateInfoCount = static_cast<uint32_t>(m_queue_infos.queue_count);
	device_info.pEnabledFeatures = &features;
	device_info.enabledExtensionCount = sizeof(kDeviceExtensions)/sizeof(const char*);
	device_info.ppEnabledExtensionNames = &kDeviceExtensions[0];

#if defined(TE_VULKAN_DEBUG)
	device_info.enabledLayerCount = sizeof(kValidationLayers) / sizeof(const char*);
	device_info.ppEnabledLayerNames = &kValidationLayers[0];
#else
	device_info.enabledLayerCount = 0;
#endif

	if (vkCreateDevice(m_physical_device, &device_info, nullptr, &m_device) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create logical device!");
		return false;
	}

	// Get presentation queue
	vkGetDeviceQueue(m_device, m_queue_infos.presentation_queue_index, 0, &m_presentation_queue);

	// Get graphics queue
	if (m_queue_infos.graphics_queue_index == m_queue_infos.presentation_queue_index)
		m_graphics_queue = m_presentation_queue;
	else
		vkGetDeviceQueue(m_device, m_queue_infos.graphics_queue_index, 0, &m_graphics_queue);

	// Get compute queue
	if (m_queue_infos.compute_queue_index == m_queue_infos.presentation_queue_index)
		m_compute_queue = m_presentation_queue;
	else if (m_queue_infos.compute_queue_index == m_queue_infos.graphics_queue_index)
		m_compute_queue = m_graphics_queue;
	else
		vkGetDeviceQueue(m_device, m_queue_infos.compute_queue_index, 0, &m_compute_queue);

	// Get transfer queue
	if (m_queue_infos.transfer_queue_index == m_queue_infos.presentation_queue_index)
		m_transfer_queue = m_presentation_queue;
	else if (m_queue_infos.transfer_queue_index == m_queue_infos.graphics_queue_index)
		m_transfer_queue = m_graphics_queue;
	else if (m_queue_infos.transfer_queue_index == m_queue_infos.compute_queue_index)
		m_transfer_queue = m_transfer_queue;
	else
		vkGetDeviceQueue(m_device, m_queue_infos.transfer_queue_index, 0, &m_transfer_queue);

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::create_swap_chain()
{
	SwapChainSupportDetails details;
	query_swap_chain_support(m_physical_device, details);
	VkSurfaceFormatKHR surface_format = choose_swap_surface_format(details.format);
	VkPresentModeKHR present_mode = choose_swap_present_mode(details.present_modes);
	VkExtent2D extent = choose_swap_extent(details.capabilities);

	uint32_t image_count = details.capabilities.minImageCount + 1;

	if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount)
		image_count = details.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = m_surface;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	m_swap_chain_image_format = surface_format.format;
	m_swap_chain_extent = extent;

	uint32_t queue_family_indices[] = { (uint32_t)m_queue_infos.graphics_queue_index, (uint32_t)m_queue_infos.presentation_queue_index };

	if (m_queue_infos.presentation_queue_index != m_queue_infos.graphics_queue_index)
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices;
	}
	else
	{
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = nullptr;
	}

	create_info.preTransform = details.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swap_chain) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create Swap Chain!");
		return false;
	}

	uint32_t swap_image_count = 0;
	vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_image_count, nullptr);
	m_swap_chain_textures.resize(swap_image_count);
	m_swap_chain_framebuffers.resize(swap_image_count);

	VkImage images[32];

	if (vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_image_count, &images[0]) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to retrieve Swap Chain images!");
		return false;
	}

	for (int i = 0; i < swap_image_count; i++)
	{
		m_swap_chain_textures[i] = create_swap_chain_texture(m_swap_chain_extent.width, m_swap_chain_extent.height, images[i], m_swap_chain_image_format, VK_SAMPLE_COUNT_1_BIT);

		Texture* texture =m_swap_chain_textures[i];

		FramebufferCreateDesc desc;

		RenderTargetDesc color_attachments[] = 
		{
			{ m_swap_chain_textures[i], 0, 0 }
		};

		desc.render_target_count = 1;
		desc.render_targets = color_attachments;

		m_swap_chain_framebuffers[i] = create_framebuffer(desc);
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::shutdown_swap_chain()
{
	for (uint32_t i = 0; i < m_swap_chain_textures.size(); i++)
		TE_HEAP_DEALLOC(m_swap_chain_textures[i]);

	for (uint32_t i = 0; i < m_swap_chain_framebuffers.size(); i++)
		destroy_framebuffer(m_swap_chain_framebuffers[i]);

	vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::check_validation_layer_support()
{
	uint32_t layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	VkLayerProperties available_layers[32];
	vkEnumerateInstanceLayerProperties(&layer_count, &available_layers[0]);

	for (const char* layer_name : kValidationLayers)
	{
		bool layer_found = false;

		for (const auto& layer_properties : available_layers)
		{
			if (strcmp(layer_name, layer_properties.layerName) == 0) 
			{
				layer_found = true;
				break;
			}
		}

		if (!layer_found)
			return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::required_extensions(Vector<const char*>& extensions)
{
	uint32_t count = 0;

	SDL_Vulkan_GetInstanceExtensions((SDL_Window*)global::application()->handle(), &count, nullptr);
	extensions.resize(count);
	SDL_Vulkan_GetInstanceExtensions((SDL_Window*)global::application()->handle(), &count, &extensions[0]);

	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	for (auto& ext : extensions)
		std::cout << ext << std::endl;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::setup_debug_callback()
{
	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	create_info.pfnCallback = debug_callback;

	if (create_debug_report_callback_ext(m_instance, &create_info, nullptr, &m_debug_callback) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to set up debug callback!");
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::query_swap_chain_support(VkPhysicalDevice device, SwapChainSupportDetails& details)
{
	// Get surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, &details.present_modes[0]);
	}

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.format.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, &details.format[0]);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkSurfaceFormatKHR GfxDevice::choose_swap_surface_format(const Vector<VkSurfaceFormatKHR>& available_formats)
{
	if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	
	for (const auto& available_format : available_formats)
	{
		if (available_format.format == VK_FORMAT_B8G8R8A8_SNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return available_format;
	}

	return available_formats[0];
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkPresentModeKHR GfxDevice::choose_swap_present_mode(const Vector<VkPresentModeKHR>& available_modes)
{
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& available_mode : available_modes)
	{
		if (available_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			best_mode = available_mode;
		else if (available_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			best_mode = available_mode;
	}

	return best_mode;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkExtent2D GfxDevice::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities)
{	
	// Causes macro issue on windows.
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else
	{
		Application* app = global::application();
		VkExtent2D actual_extent = { app->drawable_width(), app->drawable_height() };

		// Make sure the window size is between the surfaces allowed max and min image extents.
		actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));

		return actual_extent;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::check_device_extension_support(VkPhysicalDevice device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	Vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, &available_extensions[0]);

	int unavailable_extensions = 0;

	for (auto& str : kDeviceExtensions)
	{
		for (const auto& extension : available_extensions)
		{
			if (strcmp(str, extension.extensionName) != 0)
				unavailable_extensions++;
		}
	}

	return unavailable_extensions == 0;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkResult GfxDevice::create_debug_report_callback_ext(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_debug_report_callback_ext(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
		func(instance, callback, pAllocator);
}

// -----------------------------------------------------------------------------------------------------------------------------------

Texture* GfxDevice::create_swap_chain_texture(uint32_t w, uint32_t h, VkImage image, VkFormat format, VkSampleCountFlagBits sample_count)
{
	Texture* texture = TE_HEAP_NEW Texture();

	texture->width = w;
	texture->height = h;
	texture->depth = 1;
	texture->image = image;
	texture->sample_count = sample_count;
	texture->vk_format = format;
	texture->vk_type = VK_IMAGE_TYPE_2D;
	texture->type = GFX_TEXTURE_2D;

	if (format == 25 || format == 26 || format == 27)
		texture->aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;
	else
		texture->aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;

	return texture;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::calc_image_size_and_extents(Texture* texture, uint32_t mip_level, uint32_t& w, uint32_t& h, uint32_t& d, size_t& size)
{
	int width = texture->width;
	int height = texture->height;
	int depth = texture->depth;

	for (int i = 0; i < mip_level; i++)
	{
		width = std::max(1, width / 2);
		height = std::max(1, height / 2);
		depth = std::max(1, depth / 2);
	}

	w = width;
	h = height;
	d = depth;

	size_t pixel_size = kPixelSizes[texture->format];
	size - w * h * d * pixel_size;
}

// -----------------------------------------------------------------------------------------------------------------------------------

Framebuffer* GfxDevice::accquire_next_framebuffer(SemaphoreGPU* signal_semaphore)
{
	// TODO: Implement THIS!
	uint32_t idx = 0;
	return m_swap_chain_framebuffers[idx];
}

// -----------------------------------------------------------------------------------------------------------------------------------

Texture* GfxDevice::create_texture(const TextureCreateDesc& desc)
{
	Texture* texture = TE_HEAP_NEW Texture();

	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = kImageTypeTable[desc.type];
	image_info.extent.width = desc.width;
	image_info.extent.height = desc.height;
	image_info.extent.depth = desc.depth;
	image_info.mipLevels = desc.mipmap_levels;
	image_info.arrayLayers = desc.array_slices;
	image_info.format = kFormatTable[desc.format];
	image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (TE_HAS_BIT_FLAG(GFX_TEXTURE_USAGE_READABLE, desc.usage))
		image_info.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

	if (TE_HAS_BIT_FLAG(GFX_TEXTURE_USAGE_WRITABLE, desc.usage))
		image_info.usage |= VK_IMAGE_USAGE_STORAGE_BIT;

	if (TE_HAS_BIT_FLAG(GFX_TEXTURE_USAGE_DRAWABLE, desc.usage))
	{
		if (desc.format == 25 || desc.format == 26 || desc.format == 27)
		{
			image_info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			texture->aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			image_info.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			texture->aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}

	image_info.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	image_info.samples = (VkSampleCountFlagBits)desc.samples;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	texture->width = desc.width;
	texture->height = desc.height;
	texture->depth = desc.depth;
	texture->type = desc.type;
	texture->format = desc.format;
	texture->sample_count = (VkSampleCountFlagBits)desc.samples;

	VmaAllocationInfo info;

	if (!allocate_image(m_device, m_allocator, image_info, VMA_MEMORY_USAGE_GPU_ONLY, 0, texture->image, texture->allocation, info))
	{
		TE_HEAP_DELETE(texture);
		return nullptr;
	}

	// Create image view that is only used for sampling from a shader.
	if (!create_image_view(m_device, m_allocator, texture, 0, desc.mipmap_levels, 0, desc.array_slices, texture->vk_image_view))
	{
		TE_LOG_ERROR("Failed to create texture!");
		destroy_texture(texture);
		return nullptr;
	}

	texture->device_memory = info.deviceMemory;

	return texture;
}

// -----------------------------------------------------------------------------------------------------------------------------------

Buffer* GfxDevice::create_buffer(const BufferCreateDesc& desc)
{
	return nullptr;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VertexArray* GfxDevice::create_vertex_array(const VertexArrayCreateDesc& desc)
{
	VertexArray* vertex_array = TE_HEAP_NEW VertexArray();

	vertex_array->index_buffer = desc.index_buffer;
	vertex_array->vertex_buffer = desc.vertex_buffer;
	vertex_array->layout = desc.layout;

	return vertex_array;
}

// -----------------------------------------------------------------------------------------------------------------------------------

InputLayout* GfxDevice::create_input_layout(const InputLayoutCreateDesc& desc)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

Shader* GfxDevice::create_shader_from_binary(const BinaryShaderCreateDesc& desc)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

Shader* GfxDevice::create_shader_from_source(const SourceShaderCreateDesc& desc)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

// @TODO: Handle stencil targets
Framebuffer* GfxDevice::create_framebuffer(const FramebufferCreateDesc& desc)
{
	Framebuffer* framebuffer = TE_HEAP_NEW Framebuffer();

	// Temporary image view array for Framebuffer creation
	VkImageView image_views[10];
	uint32_t attachment_count = desc.render_target_count;

	for (uint32_t i = 0; i < desc.render_target_count; i++)
	{
		if (!create_image_view(m_device, m_allocator, desc.render_targets[i].texture,
							   desc.render_targets[i].mip_slice,
							   1,
							   desc.render_targets[i].array_slice,
							   1,
							   framebuffer->color_image_views[i]))
		{
			TE_LOG_ERROR("Failed to create image view!");
			TE_HEAP_DELETE(framebuffer);
			return nullptr;
		}

		image_views[i] = framebuffer->color_image_views[i];
	}

	if (desc.depth_stencil_target.texture)
	{
		attachment_count++;

		if (!create_image_view(m_device, m_allocator, desc.depth_stencil_target.texture,
							   desc.depth_stencil_target.mip_slice,
							   1,
							   desc.depth_stencil_target.array_slice,
							   1,
							   framebuffer->depth_image_view))
		{
			TE_LOG_ERROR("Failed to create image view!");
			TE_HEAP_DELETE(framebuffer);
			return nullptr;
		}

		image_views[attachment_count - 1] = framebuffer->depth_image_view;
	}

	VkFramebufferCreateInfo framebuffer_info = {};

	Texture* texture = desc.render_targets[0].texture;

	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

	VkRenderPass render_pass = create_render_pass(m_device, m_allocator, desc);

	framebuffer_info.renderPass = render_pass;
	framebuffer_info.attachmentCount = attachment_count;
	framebuffer_info.pAttachments = image_views;
	framebuffer_info.width = texture->width;
	framebuffer_info.height = texture->height;
	framebuffer_info.layers = 1; // @TODO: Check what this actually is.

	if (vkCreateFramebuffer(m_device, &framebuffer_info, nullptr, &framebuffer->framebuffer) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create framebuffer!");
		TE_HEAP_DELETE(framebuffer);
		return nullptr;
	}

	framebuffer->color_attachment_count = desc.render_target_count;
	framebuffer->render_pass = render_pass;

	return framebuffer;
}

// -----------------------------------------------------------------------------------------------------------------------------------

PipelineState* GfxDevice::create_pipeline_state(const PipelineStateCreateDesc& desc)
{
	VkPipelineRasterizationStateCreateInfo rasterizer_state = {};

	rasterizer_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_state.cullMode = kCullModeTable[desc.rasterizer_state.cull_mode];
	rasterizer_state.polygonMode = kFillModeTable[desc.rasterizer_state.fill_mode];
	rasterizer_state.frontFace = desc.rasterizer_state.front_winding_ccw ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
	rasterizer_state.lineWidth = 1.0f;
	rasterizer_state.rasterizerDiscardEnable = VK_FALSE;

	rasterizer_state.depthBiasClamp = VK_FALSE;
	rasterizer_state.depthBiasEnable = VK_FALSE;
	rasterizer_state.depthBiasConstantFactor = 0.0f;
	rasterizer_state.depthBiasClamp = 0.0f;
	rasterizer_state.depthBiasSlopeFactor = 0.0f;

	VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {};

	depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil_state.flags = 0;
	depth_stencil_state.depthTestEnable = desc.depth_stencil_state.enable_depth_test;
	depth_stencil_state.depthWriteEnable = desc.depth_stencil_state.enable_depth_write;
	depth_stencil_state.depthCompareOp = kCompareOpTable[desc.depth_stencil_state.depth_cmp_func];
	depth_stencil_state.stencilTestEnable = desc.depth_stencil_state.enable_stencil_test;

	depth_stencil_state.front.failOp = kStencilOpTable[desc.depth_stencil_state.front_stencil_fail];
	depth_stencil_state.front.passOp = kStencilOpTable[desc.depth_stencil_state.front_stencil_pass_depth_pass];
	depth_stencil_state.front.depthFailOp = kStencilOpTable[desc.depth_stencil_state.front_stencil_pass_depth_fail];
	depth_stencil_state.front.compareOp = kCompareOpTable[desc.depth_stencil_state.front_stencil_cmp_func];
	depth_stencil_state.front.writeMask = kCompareOpTable[desc.depth_stencil_state.front_stencil_mask];
	depth_stencil_state.front.compareMask = 0; // TODO
	depth_stencil_state.front.reference = 0; // TODO

	depth_stencil_state.back.failOp = kStencilOpTable[desc.depth_stencil_state.back_stencil_fail];
	depth_stencil_state.back.passOp = kStencilOpTable[desc.depth_stencil_state.back_stencil_pass_depth_pass];
	depth_stencil_state.back.depthFailOp = kStencilOpTable[desc.depth_stencil_state.back_stencil_pass_depth_fail];
	depth_stencil_state.back.compareOp = kCompareOpTable[desc.depth_stencil_state.back_stencil_cmp_func];
	depth_stencil_state.back.writeMask = kCompareOpTable[desc.depth_stencil_state.back_stencil_mask];
	depth_stencil_state.back.compareMask = 0; // TODO
	depth_stencil_state.back.reference = 0; // TODO
}

// -----------------------------------------------------------------------------------------------------------------------------------

Fence* GfxDevice::create_fence()
{
	Fence* fence = TE_HEAP_NEW Fence();

	VkFenceCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	if (vkCreateFence(m_device, &info, nullptr, &fence->vk_fence) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create Fence!");
		TE_HEAP_DELETE(fence);
		return nullptr;
	}

	return fence;
}

// -----------------------------------------------------------------------------------------------------------------------------------

SemaphoreGPU* GfxDevice::create_semaphore()
{
	SemaphoreGPU* semaphore = TE_HEAP_NEW SemaphoreGPU();

	VkSemaphoreCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	if (vkCreateSemaphore(m_device, &info, nullptr, &semaphore->vk_semaphore) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create Semaphore!");
		TE_HEAP_DELETE(semaphore);
		return nullptr;
	}

	return semaphore;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_texture(Texture* texture)
{
	if (texture)
	{
		if (texture->vk_image_view != VK_NULL_HANDLE)
			vkDestroyImageView(m_device, texture->vk_image_view, nullptr);

		if (texture->image != VK_NULL_HANDLE && texture->allocation != VK_NULL_HANDLE)
			vmaDestroyImage(m_allocator, texture->image, texture->allocation);

		TE_HEAP_DELETE(texture);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_buffer(Buffer* buffer)
{
	if (buffer)
	{
		if (buffer->vk_buffer != VK_NULL_HANDLE && buffer->vma_allocation != VK_NULL_HANDLE)
			vmaDestroyBuffer(m_allocator, buffer->vk_buffer, buffer->vma_allocation);

		TE_HEAP_DELETE(buffer);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_vertex_array(VertexArray* vertex_array)
{
	if (vertex_array)
	{
		TE_HEAP_DELETE(vertex_array);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_input_layout(InputLayout* input_layout)
{
	if (input_layout)
	{
		TE_HEAP_DELETE(input_layout);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_framebuffer(Framebuffer* framebuffer)
{
	if (framebuffer)
	{
		for (int i = 0; i < framebuffer->color_attachment_count; i++)
			vkDestroyImageView(m_device, framebuffer->color_image_views[i], nullptr);

		if (framebuffer->depth_image_view != VK_NULL_HANDLE)
			vkDestroyImageView(m_device, framebuffer->depth_image_view, nullptr);

		vkDestroyFramebuffer(m_device, framebuffer->framebuffer, nullptr);

		if (framebuffer->render_pass != VK_NULL_HANDLE)
			vkDestroyRenderPass(m_device, framebuffer->render_pass, nullptr);

		TE_HEAP_DELETE(framebuffer);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_shader(Shader* shader)
{
	if (shader)
	{
		vkDestroyShaderModule(m_device, shader->vk_shader_module, nullptr);
		TE_HEAP_DELETE(shader);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destory_pipeline_state(PipelineState* pipeline_state)
{
	if (pipeline_state)
	{
		vkDestroyPipeline(m_device, pipeline_state->vk_pipeline, nullptr);
		TE_HEAP_DELETE(pipeline_state);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_fence(Fence* fence)
{
	if (fence)
	{
		vkDestroyFence(m_device, fence->vk_fence, nullptr);
		TE_HEAP_DELETE(fence);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::destroy_semaphore(SemaphoreGPU* semaphore)
{
	if (semaphore)
	{
		vkDestroySemaphore(m_device, semaphore->vk_semaphore, nullptr);
		TE_HEAP_DELETE(semaphore);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::update_texture(Texture* texture, uint32_t mip_slice, uint32_t array_slice, void* data)
{
	uint32_t w, h, d;
	size_t size;

	calc_image_size_and_extents(texture, mip_slice, w, d, h, size);

	VkBuffer staging_buffer = VK_NULL_HANDLE;
	VmaAllocation staging_buffer_alloc = VK_NULL_HANDLE;
	VmaAllocationInfo staging_buffer_alloc_info;

	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = 0;
	buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// Create CPU Staging buffer
	allocate_buffer(m_device, m_allocator, buffer_info, VMA_MEMORY_USAGE_CPU_ONLY, 0, staging_buffer, staging_buffer_alloc, staging_buffer_alloc_info);

	void* ptr = nullptr;
	vkMapMemory(m_device, staging_buffer_alloc_info.deviceMemory, 0, size, 0, &ptr);
	memcpy(ptr, data, size);
	vkUnmapMemory(m_device, staging_buffer_alloc_info.deviceMemory);

	VkBufferImageCopy buffer_copy_region = {};
	buffer_copy_region.imageSubresource.aspectMask = texture->aspect_flags;
	buffer_copy_region.imageSubresource.mipLevel = mip_slice;
	buffer_copy_region.imageSubresource.baseArrayLayer = array_slice;
	buffer_copy_region.imageSubresource.layerCount = 1;
	buffer_copy_region.imageExtent.width = w;
	buffer_copy_region.imageExtent.height = h;
	buffer_copy_region.imageExtent.depth = d;
	buffer_copy_region.bufferOffset = 0;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::update_buffer(Buffer* buffer, size_t size, void* data)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void* GfxDevice::map_buffer(Buffer* buffer, size_t offset, size_t size)
{
	void* ptr = nullptr;

	if (vkMapMemory(m_device, buffer->vk_device_memory, offset, size, 0, &ptr) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Buffer mapping failed!");
		return nullptr;
	}

	return ptr;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::unmap_buffer(Buffer* buffer)
{
	vkUnmapMemory(m_device, buffer->vk_device_memory);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::wait_for_fences(uint32_t count, Fence** fences, uint64_t timeout)
{
	vkWaitForFences(m_device, count, (VkFence*)fences, VK_TRUE, timeout);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::check_fences(uint32_t count, Fence** fences, bool* status)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::wait_for_idle()
{
	vkDeviceWaitIdle(m_device);
}

// -----------------------------------------------------------------------------------------------------------------------------------

CommandBuffer* GfxDevice::accquire_command_buffer()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_vertex_array(CommandBuffer* cmd, VertexArray* vertex_array)
{
	size_t offset = 0;
	vkCmdBindVertexBuffers(cmd->vk_cmd_buf, 0, 1, &vertex_array->vertex_buffer->vk_buffer, &offset);

	vkCmdBindIndexBuffer(cmd->vk_cmd_buf, vertex_array->index_buffer->vk_buffer, offset, kIndexTypeTable[vertex_array->index_buffer->index_type]);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_framebuffer(CommandBuffer* cmd, Framebuffer* framebuffer)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_pipeline_state(CommandBuffer* cmd, PipelineState* pipeline_state)
{
	vkCmdBindPipeline(cmd->vk_cmd_buf, kPipelineBindPointTable[pipeline_state->type], pipeline_state->vk_pipeline);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_resource_barrier(CommandBuffer* cmd)
{
	
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_draw(CommandBuffer* cmd, uint32_t  vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance)
{
	vkCmdDraw(cmd->vk_cmd_buf, vertex_count, instance_count, first_vertex, first_instance);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_draw_indexed(CommandBuffer* cmd, uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance)
{
	vkCmdDrawIndexed(cmd->vk_cmd_buf, index_count, instance_count, first_index, vertex_offset, first_instance);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_draw_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset, uint32_t draw_count, uint32_t stride)
{
	vkCmdDrawIndirect(cmd->vk_cmd_buf, buffer->vk_buffer, offset, draw_count, stride);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_draw_indexed_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset, uint32_t draw_count, uint32_t stride)
{
	vkCmdDrawIndexedIndirect(cmd->vk_cmd_buf, buffer->vk_buffer, offset, draw_count, stride);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_dispatch(CommandBuffer* cmd, uint32_t x, uint32_t y, uint32_t z)
{
	vkCmdDispatch(cmd->vk_cmd_buf, x, y, z);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_dispatch_indirect(CommandBuffer* cmd, Buffer* buffer, size_t offset)
{
	vkCmdDispatchIndirect(cmd->vk_cmd_buf, buffer->vk_buffer, offset);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::submit_graphics(uint32_t cmd_buf_count,
								CommandBuffer** command_buffers,
								uint32_t wait_sema_count,
								SemaphoreGPU** wait_semaphores,
								uint32_t signal_sema_count,
								SemaphoreGPU** signal_semaphores,
								Fence* fence)
{
	
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::submit_compute(uint32_t cmd_buf_count,
							   CommandBuffer** command_buffers,
							   uint32_t wait_sema_count,
							   SemaphoreGPU** wait_semaphores,
							   uint32_t signal_sema_count,
							   SemaphoreGPU** signal_semaphores,
							   Fence* fence)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::present(uint32_t wait_sema_count, SemaphoreGPU** wait_semaphores)
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

VkRenderPass create_render_pass(VkDevice device, VmaAllocator allocator, const FramebufferCreateDesc& desc)
{
	VkRenderPass render_pass;

	VkAttachmentDescription attachments[10] = {};
	VkAttachmentReference color_references[10] = {};
	VkAttachmentReference depth_reference = {};

	uint32_t attachment_count = desc.render_target_count;

	for (uint32_t i = 0; i < desc.render_target_count; i++)
	{
		Texture* texture = desc.render_targets[i].texture;

		attachments[i].format = texture->vk_format;
		attachments[i].samples = texture->sample_count;
		attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		color_references[i].attachment = i;
		color_references[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	if (desc.depth_stencil_target.texture)
	{
		attachment_count++;

		Texture* texture = desc.depth_stencil_target.texture;
		uint32_t depth_idx = desc.render_target_count;

		attachments[depth_idx].format = texture->vk_format;
		attachments[depth_idx].samples = texture->sample_count;
		attachments[depth_idx].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[depth_idx].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[depth_idx].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // @TODO: Set stencil related flags
		attachments[depth_idx].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[depth_idx].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[depth_idx].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		depth_reference.attachment = depth_idx;
		depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = desc.render_target_count;
	subpass.pColorAttachments = color_references;

	if (desc.depth_stencil_target.texture)
		subpass.pDepthStencilAttachment = &depth_reference;
	else
		subpass.pDepthStencilAttachment = VK_NULL_HANDLE;

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = attachment_count;
	render_pass_info.pAttachments = attachments;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;

	if (vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create render pass!");
		return nullptr;
	}

	return render_pass;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool allocate_buffer(VkDevice device, VmaAllocator allocator, VkBufferCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkBuffer& buffer, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info)
{
	VmaAllocationCreateInfo alloc_create_info = {};
	alloc_create_info.usage = vma_usage;
	alloc_create_info.flags = vma_flags;

	if (vmaCreateBuffer(allocator, &info, &alloc_create_info, &buffer, &vma_allocation, &alloc_info) != VK_SUCCESS)
	{
		buffer = VK_NULL_HANDLE;
		vma_allocation = VK_NULL_HANDLE;
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool allocate_image(VkDevice device, VmaAllocator allocator, VkImageCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkImage& image, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info)
{
	VmaAllocationCreateInfo alloc_create_info = {};
	alloc_create_info.usage = vma_usage;
	alloc_create_info.flags = vma_flags;

	if (vmaCreateImage(allocator, &info, &alloc_create_info, &image, &vma_allocation, &alloc_info) != VK_SUCCESS)
	{
		image = VK_NULL_HANDLE;
		vma_allocation = VK_NULL_HANDLE;
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool create_image_view(VkDevice device, VmaAllocator allocator, Texture* texture, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_layer, uint32_t layer_count, VkImageView& image_view)
{
	VkImageViewCreateInfo info = vk::image_view_create_info();

	info.image = texture->image;
	info.format = texture->vk_format;

	// Subresource info
	info.subresourceRange.aspectMask = texture->aspect_flags;
	info.subresourceRange.baseMipLevel = base_mip_level;
	info.subresourceRange.levelCount = mip_level_count;
	info.subresourceRange.baseArrayLayer = base_layer;
	info.subresourceRange.layerCount = layer_count;

	// Find image view type
	if (texture->type == GFX_TEXTURE_1D && layer_count == 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_1D;
	else if (texture->type == GFX_TEXTURE_1D && layer_count > 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
	else if (texture->type == GFX_TEXTURE_2D && layer_count == 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	else if (texture->type == GFX_TEXTURE_2D && layer_count > 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
	else if (texture->type == GFX_TEXTURE_3D && layer_count == 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_3D;
	else if (texture->type == GFX_TEXTURE_CUBE && layer_count == 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	else if (texture->type == GFX_TEXTURE_CUBE && layer_count > 1)
		info.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

	// Add flags
	if (texture->type == GFX_TEXTURE_CUBE)
		info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	else if (texture->type == GFX_TEXTURE_2D && layer_count > 1)
		info.flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;

	if (vkCreateImageView(device, &info, nullptr, &image_view) != VK_SUCCESS)
		return false;

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

TE_END_TERMINUS_NAMESPACE

#endif