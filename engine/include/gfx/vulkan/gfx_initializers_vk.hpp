#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <vulkan/vulkan.h>

TE_BEGIN_TERMINUS_NAMESPACE

namespace vk
{
	VkImageCreateInfo image_create_info()
	{
		VkImageCreateInfo create_info = {};
		create_info.imageType = VK_IMAGE_TYPE_2D;
		create_info.format = VK_FORMAT_UNDEFINED;
		create_info.extent = { 1, 1, 1 };
		create_info.mipLevels = 1;
		create_info.arrayLayers = 1;
		create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		create_info.flags = 0;

		return create_info;
	}

	VkImageViewCreateInfo image_view_create_info()
	{
		VkImageViewCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image = VK_NULL_HANDLE;
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = VK_FORMAT_UNDEFINED;
		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;

		return create_info;
	}

	VkBufferCreateInfo buffer_create_info(VkBufferUsageFlags usage, size_t size)
	{
		VkBufferCreateInfo buffer_info = {};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size = size;
		buffer_info.usage = usage;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_info.queueFamilyIndexCount = 0;
		buffer_info.pQueueFamilyIndices = nullptr;

		return buffer_info;
	}
}

TE_END_TERMINUS_NAMESPACE