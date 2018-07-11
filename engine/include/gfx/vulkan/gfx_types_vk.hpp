#pragma once

#include <core/terminus_macros.hpp>
#include <vulkan/vulkan.h>
#include <gfx/vulkan/vk_mem_alloc.h>
#include <gfx/gfx_types.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

#define MAX_COLOR_ATTACHMENTS 8

struct BufferVK : Buffer
{
	VmaAllocation allocation;
	VmaAllocationInfo alloc_info;
	VkBuffer buffer;

	BufferVK()
	{
		allocation = VK_NULL_HANDLE;
		buffer = VK_NULL_HANDLE;
	}
};

struct TextureVK : Texture
{
	VkImage image;
	VkFormat vk_format;
	VkImageType vk_type;
	VkImageAspectFlags aspect_flags;
	VkSampleCountFlagBits sample_count;
	VmaAllocation allocation;
	VmaAllocationInfo alloc_info;

	TextureVK()
	{
		image = VK_NULL_HANDLE;
		allocation = VK_NULL_HANDLE;
		vk_format = VK_FORMAT_UNDEFINED;
	}
};

struct FramebufferVK : Framebuffer
{
	VkFramebuffer framebuffer;
	VkImageView   color_image_views[MAX_COLOR_ATTACHMENTS];
	VkImageView   depth_image_view;
	VkRenderPass  render_pass;

	FramebufferVK()
	{
		framebuffer = VK_NULL_HANDLE;
		
		for (int i = 0; i < MAX_COLOR_ATTACHMENTS; i++)
			color_image_views[i] = VK_NULL_HANDLE;

		depth_image_view = VK_NULL_HANDLE;
	}
};

TE_END_TERMINUS_NAMESPACE