#pragma once

#include <graphics/render_common.h>
#include <graphics/render_device.h>
#include <resource/asset_common.h>
#include <core/global.h>

namespace terminus
{
	namespace texture_factory
	{
		extern Texture* create(String image_name);
	};
}
