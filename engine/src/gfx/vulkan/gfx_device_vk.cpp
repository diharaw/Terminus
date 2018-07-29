#if defined(TE_GFX_BACKEND_VK)

#define VMA_IMPLEMENTATION
#include <gfx/vulkan/vk_mem_alloc.h>
#include <gfx/vulkan/gfx_device_vk.hpp>
#include <gfx/vulkan/gfx_types_vk.hpp>
#include <gfx/vulkan/gfx_initializers_vk.hpp>
#include <core/engine_core.hpp>
#include <stl/string_buffer.hpp>
#include <io/logger.hpp>
#include <concurrency/atomic.hpp>

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

const VkShaderStageFlagBits kShaderStageTable[] =
{
	VK_SHADER_STAGE_VERTEX_BIT,
	VK_SHADER_STAGE_FRAGMENT_BIT,
	VK_SHADER_STAGE_GEOMETRY_BIT,
	VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
	VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
	VK_SHADER_STAGE_COMPUTE_BIT
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkFormat kInputAttribFormatTable[][4] =
{
	{ VK_FORMAT_R8_SINT, VK_FORMAT_R8G8_SINT, VK_FORMAT_R8G8B8_SINT, VK_FORMAT_R8G8B8A8_SINT },
	{ VK_FORMAT_R8_UINT, VK_FORMAT_R8G8_UINT, VK_FORMAT_R8G8B8_UINT, VK_FORMAT_R8G8B8A8_UINT },
	{ VK_FORMAT_R16_SINT, VK_FORMAT_R16G16_SINT, VK_FORMAT_R16G16B16_SINT, VK_FORMAT_R16G16B16A16_SINT },
	{ VK_FORMAT_R32_SINT, VK_FORMAT_R32G32_SINT, VK_FORMAT_R32G32B32_SINT, VK_FORMAT_R32G32B32A32_SINT },
	{ VK_FORMAT_R16_UINT, VK_FORMAT_R16G16_UINT, VK_FORMAT_R16G16B16_UINT, VK_FORMAT_R16G16B16A16_UINT },
	{ VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32B32_UINT, VK_FORMAT_R32G32B32A32_UINT },
	{ VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT },
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkSampleCountFlagBits kSampleCountTable[] =
{
	VK_SAMPLE_COUNT_1_BIT,
	VK_SAMPLE_COUNT_2_BIT,
	VK_SAMPLE_COUNT_4_BIT,
	VK_SAMPLE_COUNT_8_BIT,
	VK_SAMPLE_COUNT_16_BIT,
	VK_SAMPLE_COUNT_32_BIT,
	VK_SAMPLE_COUNT_64_BIT
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkPrimitiveTopology kPrimitiveTopologyTable[] =
{
	VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkBlendFactor kBlendFactorTable[] = 
{
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_SRC_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	VK_BLEND_FACTOR_DST_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_FACTOR_DST_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	VK_BLEND_FACTOR_CONSTANT_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	VK_BLEND_FACTOR_CONSTANT_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
	VK_BLEND_FACTOR_SRC1_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	VK_BLEND_FACTOR_SRC1_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkBlendOp kBlendOpTable[] =
{
	VK_BLEND_OP_ADD,
	VK_BLEND_OP_SUBTRACT,
	VK_BLEND_OP_REVERSE_SUBTRACT,
	VK_BLEND_OP_MIN,
	VK_BLEND_OP_MAX
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkDescriptorType kDescriptorTypeTable[] =
{
	VK_DESCRIPTOR_TYPE_SAMPLER,
	VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
	VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
};

// -----------------------------------------------------------------------------------------------------------------------------------

const VkAttachmentLoadOp kLoadOpTable[] =
{
	VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	VK_ATTACHMENT_LOAD_OP_LOAD,
	VK_ATTACHMENT_LOAD_OP_CLEAR
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

// Thread-local Submission State
thread_local static VkPipelineStageFlags  m_submit_pipeline_stage_flags[32];
thread_local static VkCommandBuffer		  m_submit_cmd_buf[32];
thread_local static VkSemaphore			  m_submit_wait_semaphores[32];
thread_local static VkSemaphore			  m_submit_signal_semaphores[32];
thread_local static VkSemaphore			  m_present_semaphores[32];
thread_local static VkClearValue		  m_clear_values[MAX_COLOR_ATTACHMENTS + 1];
thread_local static VkFence				  m_wait_fences[32];
thread_local static VkImageMemoryBarrier  m_image_memory_barriers[32];
thread_local static VkBufferMemoryBarrier m_buffer_memory_barriers[32];

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
	TE_LOG_ERROR("Vulkan Validation Layer : " + std::string(msg));

	return VK_FALSE;
}

// -----------------------------------------------------------------------------------------------------------------------------------

// Helpers Declaration
VkRenderPass create_render_pass(VkDevice device, VmaAllocator allocator, const FramebufferCreateDesc& desc);
VkRenderPass create_render_pass(VkDevice device, VmaAllocator allocator, uint32_t render_target_count, TextureFormat* color_attachment_formats, uint32_t sample_count, TextureFormat depth_stencil_format);
bool allocate_buffer(VkDevice device, VmaAllocator allocator, VkBufferCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkBuffer& buffer, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info);
bool allocate_image(VkDevice device, VmaAllocator allocator, VkImageCreateInfo info, VmaMemoryUsage vma_usage, VmaAllocationCreateFlags vma_flags, VkImage& image, VmaAllocation& vma_allocation, VmaAllocationInfo& alloc_info);
bool create_image_view(VkDevice device, VmaAllocator allocator, Texture* texture, uint32_t base_mip_level, uint32_t mip_level_count, uint32_t base_layer, uint32_t layer_count, VkImageView& image_view);
bool vk_create_command_pool(VkDevice device, uint32_t queue_index, VkCommandPool* pool);
VkShaderStageFlags find_stage_flags(ShaderStageBit bits);
void vk_queue_submit(VkQueue queue, uint32_t cmd_buf_count, CommandBuffer** command_buffers, uint32_t wait_sema_count, SemaphoreGPU** wait_semaphores, uint32_t signal_sema_count, SemaphoreGPU** signal_semaphores, Fence* fence);
bool is_stencil(TextureFormat format);
VkAccessFlags vk_access_flags(ResourceState state);
VkImageLayout vk_image_layout(ResourceState usage);

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::GfxDevice() : m_framebuffer_index(0) {}

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::~GfxDevice() {}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::initialize()
{
	// Create Vulkan instance
	if (!create_instance())
	{
		TE_LOG_ERROR("Faild to create Vulkan Instance!");
		return false;
	}

#if defined(TE_VULKAN_DEBUG)
	// Setup debug callback
	if (!setup_debug_callback())
	{
		TE_LOG_ERROR("Faild to create Vulkan debug callback!");
		return false;
	}
#endif

	// Create surface
	if (!create_surface())
	{
		TE_LOG_ERROR("Faild to create Vulkan surface!");
		return false;
	}

	// Choose physical device
	if (!choose_physical_device())
	{
		TE_LOG_ERROR("Faild to choose Vulkan device!");
		return false;
	}

	// Create logical device
	if (!create_logical_device())
	{
		TE_LOG_ERROR("Faild to create Vulkan queues!");
		return false;
	}

	// Create swap chain
	if (!create_swap_chain())
	{
		TE_LOG_ERROR("Faild to create Vulkan swap chain!");
		return false;
	}

	VmaAllocatorCreateInfo allocator_info = {};
	allocator_info.physicalDevice = m_physical_device;
	allocator_info.device = m_device;

	vmaCreateAllocator(&allocator_info, &m_allocator);

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::recreate_swap_chain()
{
	shutdown_swap_chain();

	if (!create_swap_chain())
		TE_LOG_ERROR("Failed to create swap chain");
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
		TE_HEAP_DELETE(m_swap_chain_textures[i]);

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
	if (!signal_semaphore->signaled)
		signal_semaphore->signaled = true;

	VkResult res = vkAcquireNextImageKHR(m_device, m_swap_chain, UINT64_MAX, signal_semaphore->vk_semaphore, VK_NULL_HANDLE, &m_framebuffer_index);
	assert(res == VK_SUCCESS);

	return m_swap_chain_framebuffers[m_framebuffer_index];
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
	image_info.samples = kSampleCountTable[desc.samples];
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	texture->width = desc.width;
	texture->height = desc.height;
	texture->depth = desc.depth;
	texture->type = desc.type;
	texture->format = desc.format;
	texture->sample_count = kSampleCountTable[desc.samples];

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
	InputLayout* il = TE_HEAP_NEW InputLayout();

	il->input_binding_desc.binding = 0;
	il->input_binding_desc.stride = desc.vertex_size;
	il->input_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	for (uint32_t i = 0; i < desc.num_elements; i++)
	{
		il->input_attrib_descs[i].binding = 0;
		il->input_attrib_descs[i].location = i;

		assert(desc.elements[i].num_sub_elements > 0);
		assert(desc.elements[i].type >= 0);
		assert(desc.elements[i].type < 7);

		il->input_attrib_descs[i].format = kInputAttribFormatTable[desc.elements[i].type][desc.elements[i].num_sub_elements - 1];
		il->input_attrib_descs[i].offset = desc.elements[i].offset;
	}

	il->input_state_nfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	il->input_state_nfo.vertexBindingDescriptionCount = 1;
	il->input_state_nfo.pVertexBindingDescriptions = &il->input_binding_desc;
	il->input_state_nfo.vertexAttributeDescriptionCount = desc.num_elements;
	il->input_state_nfo.pVertexAttributeDescriptions = &il->input_attrib_descs[0];

	return il;
}

// -----------------------------------------------------------------------------------------------------------------------------------

Shader* GfxDevice::create_shader_from_binary(const BinaryShaderCreateDesc& desc)
{
	Shader* shader = TE_HEAP_NEW Shader();

	VkShaderModuleCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = desc.size;
	create_info.pCode = reinterpret_cast<const uint32_t*>(desc.data);

	if (vkCreateShaderModule(m_device, &create_info, nullptr, &shader->vk_shader_module) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create shader module!");
		TE_HEAP_DELETE(shader);
		return nullptr;
	}

	return shader;
}

// -----------------------------------------------------------------------------------------------------------------------------------

Shader* GfxDevice::create_shader_from_source(const SourceShaderCreateDesc& desc)
{
	Shader* shader = TE_HEAP_NEW Shader();

	return shader;
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

		framebuffer->color_load_ops[i] = desc.render_targets[i].load_op;
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

		framebuffer->depth_stencil_load_op = desc.depth_stencil_target.load_op;
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

PipelineLayout* GfxDevice::create_pipeline_layout(const PipelineLayoutCreateDesc& desc)
{
	PipelineLayout* pipeline_layout = TE_HEAP_NEW PipelineLayout();

	pipeline_layout->ds_layouts.reserve(desc.descriptor_set_count);

	for (uint32_t i = 0; i < desc.descriptor_set_count; i++)
	{
		Vector<VkDescriptorSetLayoutBinding> bindings(desc.descriptor_sets[i].descriptor_count);

		for (uint32_t j = 0; j < desc.descriptor_sets[i].descriptor_count; j++)
		{
			VkDescriptorSetLayoutBinding binding_info = {};

			binding_info.descriptorCount = 1;
			binding_info.stageFlags = find_stage_flags(desc.descriptor_sets[i].descriptors[j].stages);
			binding_info.binding = desc.descriptor_sets[i].descriptors[j].binding;
			binding_info.descriptorType = kDescriptorTypeTable[desc.descriptor_sets[i].descriptors[j].type];
			binding_info.pImmutableSamplers = nullptr;

			bindings.push_back(binding_info);
		}

		VkDescriptorSetLayoutCreateInfo ds_layout_info = {};

		ds_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		ds_layout_info.pNext = nullptr;
		ds_layout_info.flags = 0;
		ds_layout_info.bindingCount = bindings.size();
		ds_layout_info.pBindings = bindings.data();

		VkDescriptorSetLayout layout;
		
		if (vkCreateDescriptorSetLayout(m_device, &ds_layout_info, nullptr, &layout) != VK_SUCCESS)
		{
			TE_LOG_ERROR("Failed to create descriptor set layout!");
			return nullptr;
		}

		pipeline_layout->ds_layouts.push_back(layout);
	}

	Vector<VkPushConstantRange> pc_ranges(desc.push_constant_range_count);

	for (uint32_t i = 0; i < desc.push_constant_range_count; i++)
	{
		VkPushConstantRange range = {};

		range.stageFlags = find_stage_flags(desc.push_constant_ranges[i].stages);
		range.offset = desc.push_constant_ranges[i].offset;
		range.size = desc.push_constant_ranges[i].size;

		pc_ranges.push_back(range);
	}

	VkPipelineLayoutCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.setLayoutCount = pipeline_layout->ds_layouts.size();
	info.pSetLayouts = pipeline_layout->ds_layouts.data();
	info.pushConstantRangeCount = pc_ranges.size();
	info.pPushConstantRanges = pc_ranges.data();

	if (vkCreatePipelineLayout(m_device, &info, nullptr, &pipeline_layout->vk_pipeline_layout) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create Pipeline Layout!");

		for (auto ds_layout : pipeline_layout->ds_layouts)
			vkDestroyDescriptorSetLayout(m_device, ds_layout, nullptr);

		TE_HEAP_DELETE(pipeline_layout);
		return nullptr;
	}

	return pipeline_layout;
}

// -----------------------------------------------------------------------------------------------------------------------------------

PipelineState* GfxDevice::create_pipeline_state(const PipelineStateCreateDesc& desc)
{
	if (desc.type == GFX_PIPELINE_GRAPHICS)
	{
		// -------------------------------------------------------------------------------------
		// Rasterizer State
		// -------------------------------------------------------------------------------------
		VkPipelineRasterizationStateCreateInfo rasterizer_state = {};

		rasterizer_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer_state.pNext = nullptr;
		rasterizer_state.cullMode = kCullModeTable[desc.graphics.rasterizer_state.cull_mode];
		rasterizer_state.polygonMode = kFillModeTable[desc.graphics.rasterizer_state.fill_mode];
		rasterizer_state.frontFace = desc.graphics.rasterizer_state.front_winding_ccw ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizer_state.lineWidth = 1.0f;
		rasterizer_state.rasterizerDiscardEnable = VK_FALSE;

		rasterizer_state.depthBiasClamp = VK_FALSE;
		rasterizer_state.depthBiasEnable = VK_FALSE;
		rasterizer_state.depthBiasConstantFactor = 0.0f;
		rasterizer_state.depthBiasClamp = 0.0f;
		rasterizer_state.depthBiasSlopeFactor = 0.0f;

		// -------------------------------------------------------------------------------------
		// Depth Stencil State
		// -------------------------------------------------------------------------------------
		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {};

		depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil_state.pNext = nullptr;
		depth_stencil_state.flags = 0;
		depth_stencil_state.depthTestEnable = desc.graphics.depth_stencil_state.enable_depth_test;
		depth_stencil_state.depthWriteEnable = desc.graphics.depth_stencil_state.enable_depth_write;
		depth_stencil_state.depthCompareOp = kCompareOpTable[desc.graphics.depth_stencil_state.depth_cmp_func];
		depth_stencil_state.stencilTestEnable = desc.graphics.depth_stencil_state.enable_stencil_test;

		depth_stencil_state.front.failOp = kStencilOpTable[desc.graphics.depth_stencil_state.front_stencil_fail];
		depth_stencil_state.front.passOp = kStencilOpTable[desc.graphics.depth_stencil_state.front_stencil_pass_depth_pass];
		depth_stencil_state.front.depthFailOp = kStencilOpTable[desc.graphics.depth_stencil_state.front_stencil_pass_depth_fail];
		depth_stencil_state.front.compareOp = kCompareOpTable[desc.graphics.depth_stencil_state.front_stencil_cmp_func];
		depth_stencil_state.front.writeMask = kCompareOpTable[desc.graphics.depth_stencil_state.front_stencil_mask];
		depth_stencil_state.front.compareMask = 0; // TODO
		depth_stencil_state.front.reference = 0; // TODO

		depth_stencil_state.back.failOp = kStencilOpTable[desc.graphics.depth_stencil_state.back_stencil_fail];
		depth_stencil_state.back.passOp = kStencilOpTable[desc.graphics.depth_stencil_state.back_stencil_pass_depth_pass];
		depth_stencil_state.back.depthFailOp = kStencilOpTable[desc.graphics.depth_stencil_state.back_stencil_pass_depth_fail];
		depth_stencil_state.back.compareOp = kCompareOpTable[desc.graphics.depth_stencil_state.back_stencil_cmp_func];
		depth_stencil_state.back.writeMask = kCompareOpTable[desc.graphics.depth_stencil_state.back_stencil_mask];
		depth_stencil_state.back.compareMask = 0; // TODO
		depth_stencil_state.back.reference = 0; // TODO

		// -------------------------------------------------------------------------------------
		// Shader Modules
		// -------------------------------------------------------------------------------------
		Vector<VkPipelineShaderStageCreateInfo> shader_stages(desc.graphics.shader_count);

		for (uint32_t i = 0; i < desc.graphics.shader_count; i++)
		{
			VkPipelineShaderStageCreateInfo shader_stage_info = {};
			shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shader_stage_info.pNext = nullptr;
			shader_stage_info.stage = kShaderStageTable[desc.graphics.shaders[i]->stage];
			shader_stage_info.module = desc.graphics.shaders[i]->vk_shader_module;
			shader_stage_info.pName = desc.graphics.shaders[i]->entry_point.c_str();

			shader_stages.push_back(shader_stage_info);
		}

		// -------------------------------------------------------------------------------------
		// Render Pass
		// -------------------------------------------------------------------------------------
		VkRenderPass temp_render_pass = create_render_pass(m_device, m_allocator, desc.graphics.render_target_count, desc.graphics.color_attachment_formats, desc.graphics.sample_count, desc.graphics.depth_stencil_format);

		// -------------------------------------------------------------------------------------
		// Input Assembly State
		// -------------------------------------------------------------------------------------
		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
		input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly_state.pNext = nullptr;
		input_assembly_state.topology = kPrimitiveTopologyTable[desc.graphics.primitive];

		// -------------------------------------------------------------------------------------
		// Blend State
		// -------------------------------------------------------------------------------------
		Vector<VkPipelineColorBlendAttachmentState> blend_attachment_states(desc.graphics.render_target_count);

		for (uint32_t i = 0; i < desc.graphics.render_target_count; i++)
		{
			VkPipelineColorBlendAttachmentState state = {};

			state.blendEnable = desc.graphics.blend_states[i].enable;
			state.srcColorBlendFactor = kBlendFactorTable[desc.graphics.blend_states[i].src_func];
			state.dstColorBlendFactor = kBlendFactorTable[desc.graphics.blend_states[i].dst_func];
			state.colorBlendOp = kBlendOpTable[desc.graphics.blend_states[i].blend_op];
			state.srcAlphaBlendFactor = kBlendFactorTable[desc.graphics.blend_states[i].src_func_alpha];
			state.dstAlphaBlendFactor = kBlendFactorTable[desc.graphics.blend_states[i].dst_func_alpha];
			state.alphaBlendOp = kBlendOpTable[desc.graphics.blend_states[i].blend_op_alpha];
			// TODO: Check if other API's support this.
			state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; 

			blend_attachment_states.push_back(state);
		}
		
		VkPipelineColorBlendStateCreateInfo color_blend_state = {};
		color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_state.pNext = nullptr;
		color_blend_state.attachmentCount = blend_attachment_states.size();
		color_blend_state.pAttachments = blend_attachment_states.data();

		// -------------------------------------------------------------------------------------
		// Viewport State
		// -------------------------------------------------------------------------------------
		VkPipelineViewportStateCreateInfo viewport_state = {};
		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.pNext = nullptr;
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		// -------------------------------------------------------------------------------------
		// Multisample State
		// -------------------------------------------------------------------------------------
		VkPipelineMultisampleStateCreateInfo multisample_state = {};
		multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisample_state.pNext = nullptr;
		multisample_state.rasterizationSamples = kSampleCountTable[desc.graphics.sample_count];
		multisample_state.pSampleMask = nullptr;

		// -------------------------------------------------------------------------------------
		// Dynamic State
		// -------------------------------------------------------------------------------------
		VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamic_state = {};
		dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.pNext = nullptr;
		dynamic_state.pDynamicStates = dynamic_states;
		dynamic_state.dynamicStateCount = 2;

		// -------------------------------------------------------------------------------------
		// Pipeline Creation
		// -------------------------------------------------------------------------------------
		VkGraphicsPipelineCreateInfo info = {};

		info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		info.basePipelineHandle = VK_NULL_HANDLE;
		info.pNext = nullptr;
		info.flags = 0;
		info.layout = desc.pipeline_layout->vk_pipeline_layout;
		info.renderPass = temp_render_pass;
		info.stageCount = desc.graphics.shader_count;
		info.pStages = shader_stages.data();
		info.pInputAssemblyState = &input_assembly_state;
		info.pDepthStencilState = &depth_stencil_state;
		info.pRasterizationState = &rasterizer_state;
		info.pColorBlendState = &color_blend_state;
		info.pMultisampleState = &multisample_state;
		info.pDynamicState = &dynamic_state;
		info.pViewportState - &viewport_state;
		info.pVertexInputState = &desc.graphics.input_layout->input_state_nfo;

		PipelineState* pipeline = TE_HEAP_NEW PipelineState();

		pipeline->type = desc.type;

		if (vkCreateGraphicsPipelines(m_device, nullptr, 1, &info, nullptr, &pipeline->vk_pipeline) != VK_SUCCESS)
		{
			TE_LOG_ERROR("Failed to create graphics pipeline!");
			TE_HEAP_DELETE(pipeline);

			vkDestroyRenderPass(m_device, temp_render_pass, nullptr);

			return nullptr;
		}

		vkDestroyRenderPass(m_device, temp_render_pass, nullptr);

		return pipeline;
	}
	else if (desc.type == GFX_PIPELINE_COMPUTE)
	{
		VkComputePipelineCreateInfo info = {};

		info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		info.layout = desc.pipeline_layout->vk_pipeline_layout;
		info.flags = 0;
		info.pNext = nullptr;
		info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.stage.stage = kShaderStageTable[desc.compute.shader->stage];
		info.stage.module = desc.compute.shader->vk_shader_module;
		info.stage.pName = desc.compute.shader->entry_point.c_str();
		info.basePipelineHandle = VK_NULL_HANDLE;

		PipelineState* pipeline = TE_HEAP_NEW PipelineState();

		pipeline->type = desc.type;

		if (vkCreateComputePipelines(m_device, nullptr, 1, &info, nullptr, &pipeline->vk_pipeline) != VK_SUCCESS)
		{
			TE_LOG_ERROR("Failed to create compute pipeline!");
			TE_HEAP_DELETE(pipeline);

			return nullptr;
		}

		return pipeline;
	}
	else
		return nullptr;
}

// -----------------------------------------------------------------------------------------------------------------------------------

CommandPool* GfxDevice::create_command_pool(CommandPoolType type)
{
	CommandPool* cmd_pool = TE_HEAP_NEW CommandPool();

	if (vk_create_command_pool(m_device, type == GFX_CMD_POOL_GRAPHICS ? m_queue_infos.graphics_queue_index : m_queue_infos.compute_queue_index, &cmd_pool->vk_cmd_pool))
	{
		TE_HEAP_DELETE(cmd_pool);
		return nullptr;
	}

	return cmd_pool;
}

// -----------------------------------------------------------------------------------------------------------------------------------

CommandBuffer* GfxDevice::create_command_buffer(CommandPool* cmd_pool)
{
	CommandBuffer* cmd_buf = TE_HEAP_NEW CommandBuffer();

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = cmd_pool->vk_cmd_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(m_device, &alloc_info, &cmd_buf->vk_cmd_buf))
	{
		TE_HEAP_DELETE(cmd_buf);
		return nullptr;
	}

	return cmd_buf;
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

	semaphore->signaled = false;

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

void GfxDevice::destroy_pipeline_layout(PipelineLayout* pipeline_layout)
{
	if (pipeline_layout)
	{
		for (auto ds_layout : pipeline_layout->ds_layouts)
			vkDestroyDescriptorSetLayout(m_device, ds_layout, nullptr);

		vkDestroyPipelineLayout(m_device, pipeline_layout->vk_pipeline_layout, nullptr);
		TE_HEAP_DELETE(pipeline_layout);
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

void GfxDevice::destroy_command_pool(CommandPool* cmd_pool)
{
	if (cmd_pool)
	{
		vkDestroyCommandPool(m_device, cmd_pool->vk_cmd_pool, nullptr);
		TE_HEAP_DELETE(cmd_pool);
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

void GfxDevice::reset_command_pool(CommandPool* cmd_pool)
{
	vkResetCommandPool(m_device, cmd_pool->vk_cmd_pool, 0);
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
	uint32_t unsubmitted_count = 0;

	for (uint32_t i = 0; i < count; i++)
	{
		if (fences[i]->submitted)
		{
			m_wait_fences[unsubmitted_count] = fences[i]->vk_fence;
			unsubmitted_count++;
		}
	}

	vkWaitForFences(m_device, unsubmitted_count, &m_wait_fences[0], VK_TRUE, timeout);
	vkResetFences(m_device, unsubmitted_count, &m_wait_fences[0]);

	for (uint32_t i = 0; i < count; i++)
		fences[i]->submitted = false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::check_fence(Fence* fence)
{
	bool status = false;
	VkResult vkRes = vkGetFenceStatus(m_device, fence->vk_fence);

	if (vkRes == VK_SUCCESS)
	{
		vkResetFences(m_device, 1, &fence->vk_fence);
		fence->submitted = false;
		status = true;
	}

	return status;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::wait_for_idle()
{
	vkDeviceWaitIdle(m_device);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_begin_recording(CommandBuffer* cmd)
{
	assert(cmd != nullptr);
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	begin_info.pInheritanceInfo = nullptr;

	vkBeginCommandBuffer(cmd->vk_cmd_buf, &begin_info);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_end_recording(CommandBuffer* cmd)
{
	assert(cmd != nullptr);

	// If a framebuffer is already bound, it means there is already an active Render Pass. Finish it first before starting a new one.
	if (cmd->current_framebuffer)
	{
		vkCmdEndRenderPass(cmd->vk_cmd_buf);
		cmd->current_framebuffer = nullptr;
	}

	vkEndCommandBuffer(cmd->vk_cmd_buf);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_set_viewport(CommandBuffer* cmd, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	assert(cmd != nullptr);
	cmd->vk_viewport.x = x;
	cmd->vk_viewport.y = y;
	cmd->vk_viewport.minDepth = 0.0f;
	cmd->vk_viewport.maxDepth = 1.0f;
	cmd->vk_viewport.width = w;
	cmd->vk_viewport.height = h;

	vkCmdSetViewport(cmd->vk_cmd_buf, 0, 1, &cmd->vk_viewport);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_set_scissor(CommandBuffer* cmd, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	assert(cmd != nullptr);

	VkRect2D rect = {};
	rect.offset.x = x;
	rect.offset.y = y;
	rect.extent.width = w;
	rect.extent.height = h;

	vkCmdSetScissor(cmd->vk_cmd_buf, 0, 1, &rect);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_vertex_array(CommandBuffer* cmd, VertexArray* vertex_array)
{
	assert(cmd != nullptr);
	size_t offset = 0;
	vkCmdBindVertexBuffers(cmd->vk_cmd_buf, 0, 1, &vertex_array->vertex_buffer->vk_buffer, &offset);

	vkCmdBindIndexBuffer(cmd->vk_cmd_buf, vertex_array->index_buffer->vk_buffer, offset, kIndexTypeTable[vertex_array->index_buffer->index_type]);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_framebuffer(CommandBuffer* cmd, Framebuffer* framebuffer, ClearValue* color_clear_values, ClearValue ds_clear_value)
{
	assert(cmd != nullptr);

	// If a framebuffer is already bound, it means there is already an active Render Pass. Finish it first before starting a new one.
	if (cmd->current_framebuffer && cmd->current_framebuffer != framebuffer)
	{
		vkCmdEndRenderPass(cmd->vk_cmd_buf);
		cmd->current_framebuffer = nullptr;
	}

	VkRenderPassBeginInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = framebuffer->render_pass;
	render_pass_info.framebuffer = framebuffer->framebuffer;
	render_pass_info.renderArea.offset = { cmd->vk_viewport.x, cmd->vk_viewport.y };	
	render_pass_info.renderArea.extent = { cmd->vk_viewport.width, cmd->vk_viewport.height };
	
	uint32_t clear_color_count = 0;

	for (uint32_t i = 0; i < clear_color_count; i++)
	{
		if (framebuffer->color_load_ops[i] == GFX_LOAD_OP_CLEAR)
		{
			m_clear_values[i].color = { color_clear_values[i].color[0], color_clear_values[i].color[1], color_clear_values[i].color[2], color_clear_values[i].color[3] };
			m_clear_values[i].depthStencil = { color_clear_values[i].depth, color_clear_values[i].stencil };
			clear_color_count++;
		}
	}

	if (framebuffer->depth_image_view != VK_NULL_HANDLE && framebuffer->depth_stencil_load_op == GFX_LOAD_OP_CLEAR)
	{
		m_clear_values[clear_color_count].depthStencil = { ds_clear_value.depth, ds_clear_value.stencil };
		clear_color_count++;
	}

	render_pass_info.clearValueCount = clear_color_count;
	render_pass_info.pClearValues = &m_clear_values[0];

	cmd->current_framebuffer = framebuffer;

	vkCmdBeginRenderPass(cmd->vk_cmd_buf, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_bind_pipeline_state(CommandBuffer* cmd, PipelineState* pipeline_state)
{
	assert(cmd != nullptr);
	vkCmdBindPipeline(cmd->vk_cmd_buf, kPipelineBindPointTable[pipeline_state->type], pipeline_state->vk_pipeline);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::cmd_resource_barrier(CommandBuffer* cmd, uint32_t texture_barrier_count, TextureResourceBarrier* texture_barriers, uint32_t buffer_barrier_count, BufferResourceBarrier* buffer_barriers)
{
	assert(cmd != nullptr);

	uint32_t actual_texture_barrier_count = 0;

	for (uint32_t i = 0; i < texture_barrier_count; i++)
	{
		TextureResourceBarrier& barrier = texture_barriers[i];

		if (!(barrier.target_state & barrier.texture->current_state))
		{
			VkImageMemoryBarrier* img_barrier = &m_image_memory_barriers[actual_texture_barrier_count];

			img_barrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			img_barrier->pNext = NULL;
			img_barrier->image = barrier.texture->image;
			img_barrier->subresourceRange.aspectMask = barrier.texture->aspect_flags;
			img_barrier->subresourceRange.baseMipLevel = 0;
			img_barrier->subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
			img_barrier->subresourceRange.baseArrayLayer = 0;
			img_barrier->subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			img_barrier->srcAccessMask = vk_access_flags(barrier.texture->current_state);
			img_barrier->dstAccessMask = vk_access_flags(barrier.target_state);
			img_barrier->oldLayout = vk_image_layout(barrier.texture->current_state);
			img_barrier->newLayout = vk_image_layout(barrier.target_state);
			img_barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			img_barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.texture->current_state = barrier.target_state;

			actual_texture_barrier_count++;
		}
	}

	uint32_t actual_buffer_barrier_count = 0;

	for (uint32_t i = 0; i < buffer_barrier_count; i++)
	{
		BufferResourceBarrier& barrier = buffer_barriers[i];

		if (!(barrier.target_state & barrier.buffer->current_state))
		{
			VkBufferMemoryBarrier* buf_barrier = &m_buffer_memory_barriers[actual_buffer_barrier_count];

			buf_barrier->sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			buf_barrier->pNext = NULL;
			buf_barrier->buffer = barrier.buffer->vk_buffer;
			buf_barrier->size = VK_WHOLE_SIZE;
			buf_barrier->offset = 0;
			buf_barrier->srcAccessMask = vk_access_flags(barrier.buffer->current_state);
			buf_barrier->dstAccessMask = vk_access_flags(barrier.target_state);
			buf_barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			buf_barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.buffer->current_state = barrier.target_state;

			actual_buffer_barrier_count++;
		}
	}

	if (actual_texture_barrier_count || actual_buffer_barrier_count)
	{
		VkPipelineStageFlags src_pipeline_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		VkPipelineStageFlags dst_pipeline_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		vkCmdPipelineBarrier(cmd->vk_cmd_buf, src_pipeline_flags, dst_pipeline_flags, 0, 0, NULL, actual_buffer_barrier_count, &m_buffer_memory_barriers[0], actual_texture_barrier_count, &m_image_memory_barriers[0]);
	}
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
	vk_queue_submit(m_graphics_queue, cmd_buf_count, command_buffers, wait_sema_count, wait_semaphores, signal_sema_count, signal_semaphores, fence);
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
	vk_queue_submit(m_compute_queue, cmd_buf_count, command_buffers, wait_sema_count, wait_semaphores, signal_sema_count, signal_semaphores, fence);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::present(uint32_t wait_sema_count, SemaphoreGPU** wait_semaphores)
{
	uint32_t wait_count = 0;

	for (uint32_t i = 0; i < wait_sema_count; i++)
	{
		if (wait_semaphores[i]->signaled)
		{
			m_present_semaphores[wait_count] = wait_semaphores[i]->vk_semaphore;
			wait_semaphores[i]->signaled = false;

			wait_count++;
		}
	}

	VkPresentInfoKHR present_info = {};

	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext = NULL;
	present_info.waitSemaphoreCount = wait_count;
	present_info.pWaitSemaphores = &m_present_semaphores[0];
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &(m_swap_chain);
	present_info.pImageIndices = &m_framebuffer_index;
	present_info.pResults = NULL;

	if (vkQueuePresentKHR(m_presentation_queue, &present_info) != VK_SUCCESS)
		TE_LOG_ERROR("Failed to present");
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
		attachments[i].loadOp = kLoadOpTable[desc.render_targets[i].load_op];
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
		attachments[depth_idx].loadOp = kLoadOpTable[desc.depth_stencil_target.load_op];
		attachments[depth_idx].storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		if (is_stencil(texture->format))
		{
			attachments[depth_idx].stencilLoadOp = kLoadOpTable[desc.depth_stencil_target.load_op];
			attachments[depth_idx].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
		else
		{
			attachments[depth_idx].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[depth_idx].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}

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

VkRenderPass create_render_pass(VkDevice device, VmaAllocator allocator, uint32_t render_target_count, TextureFormat* color_attachment_formats, uint32_t sample_count, TextureFormat depth_stencil_format)
{
	VkRenderPass render_pass;

	VkAttachmentDescription attachments[10] = {};
	VkAttachmentReference color_references[10] = {};
	VkAttachmentReference depth_reference = {};

	uint32_t attachment_count = render_target_count;

	for (uint32_t i = 0; i < render_target_count; i++)
	{
		attachments[i].format = kFormatTable[color_attachment_formats[i]];
		attachments[i].samples = kSampleCountTable[sample_count];
		attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		color_references[i].attachment = i;
		color_references[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	if (depth_stencil_format != GFX_FORMAT_UNKNOWN)
	{
		attachment_count++;

		uint32_t depth_idx = render_target_count;

		attachments[depth_idx].format = kFormatTable[depth_stencil_format];
		attachments[depth_idx].samples = kSampleCountTable[sample_count];
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
	subpass.colorAttachmentCount = render_target_count;
	subpass.pColorAttachments = color_references;

	if (depth_stencil_format != GFX_FORMAT_UNKNOWN)
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
	// TODO: add HOST_COHERENT etc options

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

bool vk_create_command_pool(VkDevice device, uint32_t queue_index, VkCommandPool* pool)
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queue_index;
	pool_info.flags = 0;

	if (vkCreateCommandPool(device, &pool_info, nullptr, pool) != VK_SUCCESS)
	{
		TE_LOG_ERROR("Failed to create command pool");
		return false;
	}

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkShaderStageFlags find_stage_flags(ShaderStageBit bits)
{
	VkShaderStageFlags flags = 0;

	if (bits & GFX_SHADER_STAGE_VERTEX_BIT)
		flags |= VK_SHADER_STAGE_VERTEX_BIT;

	if (bits & GFX_SHADER_STAGE_FRAGMENT_BIT)
		flags |= VK_SHADER_STAGE_FRAGMENT_BIT;

	if (bits & GFX_SHADER_STAGE_GEOMETRY_BIT)
		flags |= VK_SHADER_STAGE_GEOMETRY_BIT;

	if (bits & GFX_SHADER_STAGE_TESS_CONTROL_BIT)
		flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

	if (bits & GFX_SHADER_STAGE_TESS_EVAL_BIT)
		flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

	if (bits & GFX_SHADER_STAGE_COMPUTE_BIT)
		flags |= VK_SHADER_STAGE_COMPUTE_BIT;

	return flags;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void vk_queue_submit(VkQueue queue, uint32_t cmd_buf_count, CommandBuffer** command_buffers, uint32_t wait_sema_count, SemaphoreGPU** wait_semaphores, uint32_t signal_sema_count, SemaphoreGPU** signal_semaphores, Fence* fence)
{
	for (uint32_t i = 0; i < cmd_buf_count; i++)
		m_submit_cmd_buf[i] = command_buffers[i]->vk_cmd_buf;

	uint32_t wait_count = 0;

	for (uint32_t i = 0; i < wait_sema_count; i++)
	{
		if (wait_semaphores[i]->signaled)
		{
			m_submit_wait_semaphores[wait_count] = wait_semaphores[i]->vk_semaphore;
			m_submit_pipeline_stage_flags[wait_count] = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			wait_count++;

			wait_semaphores[i]->signaled = false;
		}
	}

	uint32_t signal_count = 0;

	for (uint32_t i = 0; i < signal_sema_count; i++)
	{
		if (!signal_semaphores[i]->signaled)
		{
			m_submit_signal_semaphores[signal_count] = signal_semaphores[i]->vk_semaphore;
			signal_count++;

			signal_semaphores[i]->signaled = true;
		}
	}

	VkSubmitInfo submit_info;

	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = NULL;
	submit_info.waitSemaphoreCount = wait_count;
	submit_info.pWaitSemaphores = &m_submit_wait_semaphores[0];
	submit_info.pWaitDstStageMask = &m_submit_pipeline_stage_flags[0];
	submit_info.commandBufferCount = cmd_buf_count;
	submit_info.pCommandBuffers = &m_submit_cmd_buf[0];
	submit_info.signalSemaphoreCount = signal_count;
	submit_info.pSignalSemaphores = &m_submit_signal_semaphores[0];

	VkFence vk_fence = VK_NULL_HANDLE;

	if (fence)
	{
		vk_fence = fence->vk_fence;
		fence->submitted = true;
	}

	vkQueueSubmit(queue, 1, &submit_info, vk_fence);
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool is_stencil(TextureFormat format)
{
	if (format == GFX_FORMAT_D32_FLOAT_S8_UINT || format == GFX_FORMAT_D24_FLOAT_S8_UINT)
		return true;
	else
		return false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkAccessFlags vk_access_flags(ResourceState state)
{
	VkAccessFlags ret = 0;

	if (state & GFX_RESOURCE_STATE_COPY_SOURCE)
		ret |= VK_ACCESS_TRANSFER_READ_BIT;
	
	if (state & GFX_RESOURCE_STATE_COPY_DEST)
		ret |= VK_ACCESS_TRANSFER_WRITE_BIT;
	
	if (state & GFX_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER)
		ret |= VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	
	if (state & GFX_RESOURCE_STATE_INDEX_BUFFER)
		ret |= VK_ACCESS_INDEX_READ_BIT;
	
	if (state & GFX_RESOURCE_STATE_UNORDERED_ACCESS)
		ret |= VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
	
	if (state & GFX_RESOURCE_STATE_INDIRECT_ARGUMENT)
		ret |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
	
	if (state & GFX_RESOURCE_STATE_RENDER_TARGET)
		ret |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	
	if (state & GFX_RESOURCE_STATE_DEPTH_WRITE)
		ret |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	if ((state & GFX_RESOURCE_STATE_SHADER_RESOURCE) || (state & GFX_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE))
		ret |= VK_ACCESS_SHADER_READ_BIT;

	if (state & GFX_RESOURCE_STATE_PRESENT)
		ret |= VK_ACCESS_MEMORY_READ_BIT;

	return ret;
}

// -----------------------------------------------------------------------------------------------------------------------------------

VkImageLayout vk_image_layout(ResourceState usage)
{
	if (usage & GFX_RESOURCE_STATE_COPY_SOURCE)
		return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

	if (usage & GFX_RESOURCE_STATE_COPY_DEST)
		return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

	if (usage & GFX_RESOURCE_STATE_RENDER_TARGET)
		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	if (usage & GFX_RESOURCE_STATE_DEPTH_WRITE)
		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	if (usage & GFX_RESOURCE_STATE_UNORDERED_ACCESS)
		return VK_IMAGE_LAYOUT_GENERAL;

	if ((usage & GFX_RESOURCE_STATE_SHADER_RESOURCE) || (usage & GFX_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE))
		return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	if (usage & GFX_RESOURCE_STATE_PRESENT)
		return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	if (usage == GFX_RESOURCE_STATE_COMMON)
		return VK_IMAGE_LAYOUT_GENERAL;

	return VK_IMAGE_LAYOUT_UNDEFINED;
}

// -----------------------------------------------------------------------------------------------------------------------------------

TE_END_TERMINUS_NAMESPACE

#endif