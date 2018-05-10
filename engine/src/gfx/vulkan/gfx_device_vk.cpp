#include <gfx/vulkan/gfx_device_vk.hpp>
#include <core/engine_core.hpp>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

const char* kDeviceTypes[] = {
	"VK_PHYSICAL_DEVICE_TYPE_OTHER",
	"VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
	"VK_PHYSICAL_DEVICE_TYPE_CPU"
};

const char* kVendorNames[] = {
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

const char* kDeviceExtensions[] = {
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

	return true;
}

void GfxDevice::shutdown()
{
	destroy_debug_report_callback_ext(m_instance, m_debug_callback, nullptr);

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
		// LOG
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

void GfxDevice::find_queues(QueueInfos& queue_infos)
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

		if (m_device_properties.vendor_id == VK_VENDOR_NVIDIA)
		{
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, m_surface, &present_support);

			// Look for Presentation Queue
			if (present_support && queue_infos.presentation_queue_index == -1)
			{
				queue_infos.presentation_queue_index = i;
			}
			// Look for All-Round Queue
			if ((bits & VK_QUEUE_GRAPHICS_BIT) && (bits & VK_QUEUE_COMPUTE_BIT) && (bits & VK_QUEUE_TRANSFER_BIT) && queue_infos.graphics_queue_index == -1)
			{
				queue_infos.graphics_queue_index = i;
				std::cout << "Found Graphics Queue Family" << std::endl;
			}
			// Look for Transfer Queue
			if (!(bits & VK_QUEUE_GRAPHICS_BIT) && !(bits & VK_QUEUE_COMPUTE_BIT) && (bits & VK_QUEUE_TRANSFER_BIT) && queue_infos.transfer_queue_index == -1)
			{
				queue_infos.transfer_queue_index = i;
				std::cout << "Found Transfer Queue Family" << std::endl;
			}
			// Look for Async Compute Queue
			if (!(bits & VK_QUEUE_GRAPHICS_BIT) && (bits & VK_QUEUE_COMPUTE_BIT) && !(bits & VK_QUEUE_TRANSFER_BIT) && queue_infos.compute_queue_index == -1)
			{
				queue_infos.compute_queue_index = i;
				std::cout << "Found Async Compute Queue Family" << std::endl;
			}
		}
		// TODO: AMD, Intel etc queue selection
	}

	// At least graphics and presentation queue must be supported
	if (queue_infos.graphics_queue_index == -1)
	{
		std::cout << "No Graphics Queue Found" << std::endl;
		return;
	}

	if (queue_infos.presentation_queue_index == -1)
	{
		std::cout << "No Presentation Queue Found" << std::endl;
		return;
	}

	float priority = 1.0f;

	VkDeviceQueueCreateInfo presentation_queue_info = {};
	presentation_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	presentation_queue_info.queueFamilyIndex = queue_infos.presentation_queue_index;
	presentation_queue_info.queueCount = 1;
	presentation_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo graphics_queue_info = {};
	graphics_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphics_queue_info.queueFamilyIndex = queue_infos.graphics_queue_index;
	graphics_queue_info.queueCount = 1;
	graphics_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo compute_queue_info = {};
	compute_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	compute_queue_info.queueFamilyIndex = queue_infos.compute_queue_index;
	compute_queue_info.queueCount = 1;
	compute_queue_info.pQueuePriorities = &priority;

	VkDeviceQueueCreateInfo transfer_queue_info = {};
	transfer_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	transfer_queue_info.queueFamilyIndex = queue_infos.transfer_queue_index;
	transfer_queue_info.queueCount = 1;
	transfer_queue_info.pQueuePriorities = &priority;

	queue_infos.infos[queue_infos.queue_count++] = presentation_queue_info;

	if (queue_infos.graphics_queue_index != queue_infos.presentation_queue_index)
		queue_infos.infos[queue_infos.queue_count++] = graphics_queue_info;

	if (queue_infos.compute_queue_index != queue_infos.presentation_queue_index && queue_infos.compute_queue_index != queue_infos.graphics_queue_index)
		queue_infos.infos[queue_infos.queue_count++] = compute_queue_info;

	if (queue_infos.transfer_queue_index != queue_infos.presentation_queue_index && queue_infos.transfer_queue_index != queue_infos.graphics_queue_index && queue_infos.transfer_queue_index != queue_infos.compute_queue_index)
		queue_infos.infos[queue_infos.queue_count++] = transfer_queue_info;
}

bool GfxDevice::create_logical_device()
{
	QueueInfos queue_infos;
	find_queues(queue_infos);

	if (queue_infos.queue_count == 0)
		return false;

	VkPhysicalDeviceFeatures features = {};

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pQueueCreateInfos = &queue_infos.infos[0];
	device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.queue_count);
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
		return true;
	}

	// Get presentation queue
	vkGetDeviceQueue(m_device, queue_infos.presentation_queue_index, 0, &m_presentation_queue);

	// Get graphics queue
	if (queue_infos.graphics_queue_index == queue_infos.presentation_queue_index)
		m_graphics_queue = m_presentation_queue;
	else
		vkGetDeviceQueue(m_device, queue_infos.graphics_queue_index, 0, &m_graphics_queue);

	// Get compute queue
	if (queue_infos.compute_queue_index == queue_infos.presentation_queue_index)
		m_compute_queue = m_presentation_queue;
	else if (queue_infos.compute_queue_index == queue_infos.graphics_queue_index)
		m_compute_queue = m_graphics_queue;
	else
		vkGetDeviceQueue(m_device, queue_infos.compute_queue_index, 0, &m_compute_queue);

	// Get transfer queue
	if (queue_infos.transfer_queue_index == queue_infos.presentation_queue_index)
		m_transfer_queue = m_presentation_queue;
	else if (queue_infos.transfer_queue_index == queue_infos.graphics_queue_index)
		m_transfer_queue = m_graphics_queue;
	else if (queue_infos.transfer_queue_index == queue_infos.compute_queue_index)
		m_transfer_queue = m_transfer_queue;
	else
		vkGetDeviceQueue(m_device, queue_infos.transfer_queue_index, 0, &m_transfer_queue);

	return false;
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

VertexBuffer* GfxDevice::create(const VertexBufferDesc& desc)
{
	return nullptr;
}

void GfxDevice::destroy(VertexBuffer* buffer)
{

}

TE_END_TERMINUS_NAMESPACE