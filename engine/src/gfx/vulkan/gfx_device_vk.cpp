#include <gfx/vulkan/gfx_device_vk.hpp>
#include <gfx/vulkan/gfx_types_vk.hpp>
#include <core/engine_core.hpp>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

const char* kDeviceTypes[] = 
{
	"VK_PHYSICAL_DEVICE_TYPE_OTHER",
	"VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_CPU"
};

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

const char* kValidationLayers[] =
{
	"VK_LAYER_LUNARG_standard_validation"
};

const char* kDeviceExtensions[] = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

enum VkVendor
{
	VK_VENDOR_AMD = 0x1002,
	VK_VENDOR_IMAGINATION = 0x1010,
	VK_VENDOR_NVIDIA = 0x10DE,
	VK_VENDOR_ARM = 0x13B5,
	VK_VENDOR_QUALCOMM = 0x5143,
	VK_VENDOR_INTEL = 0x8086,
};

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

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT obj_type,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layer_prefix,
	const char* msg,
	void* user_data)
{
	std::cerr << "Validation Layer : " << msg << std::endl;

	return VK_FALSE;
}

GfxDevice::GfxDevice()
{

}

GfxDevice::~GfxDevice()
{

}

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

	return true;
}

void GfxDevice::shutdown()
{
	destroy_debug_report_callback_ext(m_instance, m_debug_callback, nullptr);

	shutdown_swap_chain();

	vkDestroyDevice(m_device, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

bool GfxDevice::create_instance()
{
#if defined(TE_VULKAN_DEBUG)
	if (!check_validation_layer_support())
		std::cout << "Validation layers requested, but not available!" << std::endl;
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
		std::cout << "Failed to create Vulkan instance" << std::endl;
		return false;
	}

	return true;
}

bool GfxDevice::choose_physical_device()
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

	if (device_count == 0)
	{
		std::cout << "No Vulkan-compatible devices found!" << std::endl;
		return false;
	}

	VkPhysicalDevice devices[32];
	vkEnumeratePhysicalDevices(m_instance, &device_count, &devices[0]);

	std::cout << "Number of Physical Devices found: " << device_count << std::endl;

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

bool GfxDevice::create_surface()
{
	return SDL_Vulkan_CreateSurface((SDL_Window*)global::application()->handle(), m_instance, &m_surface);
}

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
				// Else, a queue that supports compute and transfer only (might allow a asynchronous compute. Have to check).
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
		std::cout << "Failed to create logical device!" << std::endl;
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
		std::cout << "Failed to create Swap Chain!" << std::endl;
		return false;
	}

	uint32_t swap_image_count = 0;
	vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_image_count, nullptr);
	m_swap_chain_textures.resize(swap_image_count);
	m_swap_chain_framebuffers.resize(swap_image_count);

	VkImage images[32];

	vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_image_count, &images[32]);

	for (int i = 0; i < swap_image_count; i++)
	{
		TextureVK* texture = static_cast<TextureVK*>(m_swap_chain_textures[i]);
		texture->image = images[i];

		// Create image view.
		VkImageViewCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image = texture->image;
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = m_swap_chain_image_format;
		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_device, &create_info, nullptr, &texture->image_view) != VK_SUCCESS)
		{
			std::cout << "Failed to create image view!" << std::endl;
			return false;
		}


	}

	return true;
}

void GfxDevice::shutdown_swap_chain()
{
	vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
}

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

bool GfxDevice::setup_debug_callback()
{
	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	create_info.pfnCallback = debug_callback;

	if (create_debug_report_callback_ext(m_instance, &create_info, nullptr, &m_debug_callback) != VK_SUCCESS)
	{
		std::cout << "Failed to set up debug callback!" << std::endl;
		return false;
	}

	return true;
}

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

VkResult GfxDevice::create_debug_report_callback_ext(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void GfxDevice::destroy_debug_report_callback_ext(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
		func(instance, callback, pAllocator);
}

Texture* GfxDevice::create_texture(const TextureDesc& desc)
{

}

void GfxDevice::destroy_texture(Texture* texture)
{

}

Buffer* GfxDevice::create_buffer(const BufferDesc& desc)
{

}

void GfxDevice::destroy_buffer(Buffer* buffer)
{

}

Framebuffer* GfxDevice::create_framebuffer(const FramebufferDesc& desc)
{

}

void GfxDevice::destroy_framebuffer(Framebuffer* framebuffer)
{

}

TE_END_TERMINUS_NAMESPACE