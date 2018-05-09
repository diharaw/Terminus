#include <gfx/vulkan/gfx_device_vk.hpp>
#include <core/engine_core.hpp>
#include <iostream>

#define TE_VULKAN_DEBUG

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

namespace VkVendorID
{
	enum
	{
		AMD = 0x1002,
		IMAGINATION = 0x1010,
		NVIDIA = 0x10DE,
		ARM = 0x13B5,
		QUALCOMM = 0x5143,
		INTEL = 0x8086,
	};
}

const char* GetVendorName(uint32_t id)
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

	// Choose physical device
	if (!choose_physical_device())
		return false;

	return true;
}

void GfxDevice::shutdown()
{
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

	uint32_t vendorId = 0;

	for (uint32_t i = 0; i < device_count; i++)
	{
		VkPhysicalDevice& device = devices[i];
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		vendorId = properties.vendorID;

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physical_device = device;
			m_device_properties.vendor_id = properties.vendorID;
			m_device_properties.name = properties.deviceName;
			m_device_properties.type = kDeviceTypes[properties.deviceType];
			m_device_properties.driver = properties.driverVersion;

			std::cout << std::endl;
			std::cout << "Vendor: " << GetVendorName(properties.vendorID) << std::endl;
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
	return false;
}

bool GfxDevice::create_logical_device()
{
	return false;
}

bool GfxDevice::create_queues()
{
	uint32_t family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &family_count, nullptr);

	std::cout << std::endl;
	std::cout << "Number of Queue families: " << family_count << std::endl;

	VkQueueFamilyProperties families[32];
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &family_count, &families[0]);

	uint32_t graphics_queue_index = 0;
	uint32_t compute_queue_index = 0;
	uint32_t transfer_queue_index = 0;

	for (uint32_t i = 0; i < family_count; i++)
	{
		VkQueueFlags bits = families[i].queueFlags;

		std::cout << std::endl;
		std::cout << "Family " << i << std::endl;
		std::cout << "Supported Bits: " << "VK_QUEUE_GRAPHICS_BIT: " << ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "1" : "0") << ", " << "VK_QUEUE_COMPUTE_BIT: " << ((families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) ? "1" : "0") << ", " << "VK_QUEUE_TRANSFER_BIT: " << ((families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) ? "1" : "0") << std::endl;
		std::cout << "Number of Queues: " << families[i].queueCount << std::endl;

		if (m_device_properties.vendor_id == VkVendorID::NVIDIA)
		{
			// Look for Transfer Queue
			if ((bits & VK_QUEUE_GRAPHICS_BIT) && (bits & VK_QUEUE_COMPUTE_BIT) && (bits & VK_QUEUE_TRANSFER_BIT))
			{
				graphics_queue_index = i;
				std::cout << "Found Graphics Queue Family" << std::endl;
			}
			// Look for Transfer Queue
			if (!(bits & VK_QUEUE_GRAPHICS_BIT) && !(bits & VK_QUEUE_COMPUTE_BIT) && (bits & VK_QUEUE_TRANSFER_BIT))
			{
				transfer_queue_index = i;
				std::cout << "Found Transfer Queue Family" << std::endl;
			}
			// Look for Async Compute Queue
			if (!(bits & VK_QUEUE_GRAPHICS_BIT) && (bits & VK_QUEUE_COMPUTE_BIT) && !(bits & VK_QUEUE_TRANSFER_BIT))
			{
				compute_queue_index = i;
				std::cout << "Found Async Compute Queue Family" << std::endl;
			}
		}
		// TODO: AMD, Intel etc queue selection
	}

	return true;
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

VertexBuffer* GfxDevice::create(const VertexBufferDesc& desc)
{
	return nullptr;
}

void GfxDevice::destroy(VertexBuffer* buffer)
{

}

TE_END_TERMINUS_NAMESPACE