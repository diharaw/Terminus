#pragma once

#include <core/terminus_macros.hpp>
#include <vulkan/vulkan.h>
#include <gfx/gfx_types.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct BufferVK : Buffer
{
	VkDeviceMemory memory;
	VkBuffer buffer;

	BufferVK()
	{
		memory = VK_NULL_HANDLE;
		buffer = VK_NULL_HANDLE;
	}
};

struct TextureVK : Texture
{
	VkImage image;
	VkImageView image_view;

	TextureVK()
	{
		image = VK_NULL_HANDLE;
		image_view = VK_NULL_HANDLE;
	}
};

struct FramebufferVK : Framebuffer
{
	VkFramebuffer framebuffer;

	FramebufferVK()
	{
		framebuffer = VK_NULL_HANDLE;
	}
};

TE_END_TERMINUS_NAMESPACE