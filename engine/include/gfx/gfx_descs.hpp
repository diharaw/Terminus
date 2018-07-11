#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <stl/string_buffer.hpp>
#include <gfx/gfx_enums.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct Texture;

struct DeviceProperties
{
	uint32_t	   vendor_id;
	uint32_t	   driver;
	StringBuffer32 name;
	StringBuffer32 type;
};

struct DeviceLimits
{

};

struct TextureDesc
{
	TextureType   type;
	TextureFormat format;
	TextureUsage  usage;
	uint32_t	  width;
	uint32_t	  height;
	uint32_t	  depth;
	uint32_t	  mip_levels;
	uint32_t	  array_layers;
	uint32_t	  samples;

	TextureDesc()
	{
		type = GFX_TEXTURE_2D;
		format = GFX_FORMAT_R8G8B8A8_UNORM;
		usage = GFX_TEXTURE_USAGE_NONE;
		width = 0;
		height = 0;
		depth = 0;
		mip_levels = 1;
		array_layers = 1;
		samples = 1;
	}
};

struct BufferDesc
{

};

struct AttachmentDesc
{
	uint32_t base_mip_level;
	uint32_t mip_level_count;
	uint32_t base_layer;
	uint32_t layer_count;
	Texture* texture;

	AttachmentDesc()
	{
		base_mip_level = 0;
		mip_level_count = 0;
		base_layer = 0;
		layer_count = 0;
		texture = nullptr;
	}

	AttachmentDesc(uint32_t base_mip_level, 
				  uint32_t mip_level_count,
				  uint32_t base_layer,
			      uint32_t layer_count,
				  Texture* texture)
	{
		this->base_mip_level = base_mip_level;
		this->mip_level_count = mip_level_count;
		this->base_layer = base_layer;
		this->layer_count = layer_count;
		this->texture = texture;
	}
};

struct FramebufferDesc
{
	uint32_t		num_color_attachments;
	AttachmentDesc* color_attachments;
	AttachmentDesc  depth_attachment;

	FramebufferDesc()
	{
		num_color_attachments = 0;
		color_attachments = nullptr;
	}
};

TE_END_TERMINUS_NAMESPACE