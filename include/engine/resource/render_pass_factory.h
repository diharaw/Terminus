#pragma once

#include <Graphics/render_pass.h>
#include <Graphics/render_device.h>
#include <types.h>

namespace terminus
{
	namespace render_pass_factory
	{
		extern RenderPass* create(String render_pass_name);
	};
} // namespace terminus
