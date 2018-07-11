#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <gfx/gfx_enums.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct Buffer {};
struct VertexArray {};
struct GraphicsPipeline {};
struct ComputePipeline {};
struct CommandBuffer {};

struct Framebuffer 
{
	uint32_t color_attachment_count;
};

struct Texture 
{
	TextureType type;
	TextureFormat format;
	uint32_t	width;
	uint32_t	height;
	uint32_t	depth;
};

TE_END_TERMINUS_NAMESPACE