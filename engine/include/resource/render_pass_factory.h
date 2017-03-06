#pragma once

#include <graphics/render_pass.h>
#include <graphics/render_device.h>
#include <core/types.h>

namespace terminus
{
	namespace render_pass_factory
	{
		extern RenderPass* create(String render_pass_name);
	};
} // namespace terminus
