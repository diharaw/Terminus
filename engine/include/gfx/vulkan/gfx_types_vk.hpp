#pragma once

#if defined(TE_GFX_BACKEND_VK)

#include <core/terminus_macros.hpp>
#include <vulkan/vulkan.h>
#include <gfx/gfx_enums.hpp>
#include <stl/string_buffer.hpp>
#include <stl/vector.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

#define MAX_COLOR_ATTACHMENTS 8
#define MAX_VERTEX_ATTRIBUTES 8

struct Buffer
{
	struct VmaAllocation_T* vma_allocation;
	VkDeviceMemory vk_device_memory;
	VkBuffer vk_buffer;
	DataType index_type;

	Buffer()
	{
		vma_allocation = VK_NULL_HANDLE;
		vk_device_memory = VK_NULL_HANDLE;
		vk_buffer = VK_NULL_HANDLE;
	}
};

struct Texture
{
	TextureType type;
	TextureFormat format;
	uint32_t	width;
	uint32_t	height;
	uint32_t	depth;
	VkImage image;
	VkFormat vk_format;
	VkImageType vk_type;
	// For accessing images in shaders.
	VkImageView vk_image_view; 
	VkImageAspectFlags aspect_flags;
	VkSampleCountFlagBits sample_count;
	struct VmaAllocation_T* allocation;
	VkDeviceMemory device_memory;

	Texture()
	{
		image = VK_NULL_HANDLE;
		vk_image_view = VK_NULL_HANDLE;
		allocation = VK_NULL_HANDLE;
		device_memory = VK_NULL_HANDLE;
		vk_format = VK_FORMAT_UNDEFINED;
	}
};

struct Framebuffer
{
	uint32_t	  color_attachment_count;
	VkFramebuffer framebuffer;
	VkImageView   color_image_views[MAX_COLOR_ATTACHMENTS];
	LoadOp		  color_load_ops[MAX_COLOR_ATTACHMENTS];
	VkImageView   depth_image_view;
	LoadOp		  depth_stencil_load_op;
	VkRenderPass  render_pass;

	Framebuffer()
	{
		framebuffer = VK_NULL_HANDLE;
		
		for (int i = 0; i < MAX_COLOR_ATTACHMENTS; i++)
			color_image_views[i] = VK_NULL_HANDLE;

		depth_image_view = VK_NULL_HANDLE;
	}
};

struct CommandPool
{
	VkCommandPool vk_cmd_pool;
};

struct CommandBuffer
{
	VkCommandBuffer vk_cmd_buf;
	VkViewport		vk_viewport;
	VkClearValue	vk_clear_value;
	Framebuffer*	current_framebuffer;

	CommandBuffer()
	{
		current_framebuffer = nullptr;
	}
};

struct Shader
{
	ShaderStage	   stage;
	VkShaderModule vk_shader_module;
	StringBuffer32 entry_point;
};

struct PipelineState
{
	PipelineType type;
	VkPipeline vk_pipeline;
};

struct Fence
{
	VkFence vk_fence;
	bool	submitted;

	Fence()
	{
		submitted = false;
	}
};

// TODO: Namespace the GFX stuff!!!
struct SemaphoreGPU
{
	VkSemaphore vk_semaphore;
	bool		signaled;

	SemaphoreGPU()
	{
		signaled = false;
	}
};

struct InputLayout
{
	VkVertexInputBindingDescription		 input_binding_desc;
	VkVertexInputAttributeDescription	 input_attrib_descs[MAX_VERTEX_ATTRIBUTES];
	VkPipelineVertexInputStateCreateInfo input_state_nfo;
};

struct VertexArray
{
	Buffer*		 vertex_buffer;
	Buffer*		 index_buffer;
	InputLayout* layout;
};

struct PipelineLayout
{
	VkPipelineLayout			  vk_pipeline_layout;
	Vector<VkDescriptorSetLayout> ds_layouts;
};

TE_END_TERMINUS_NAMESPACE

#endif