#pragma once

#include <resource/asset_common.h>
#include <core/types.h>

namespace terminus
{
	namespace stb_image_loader
	{
		extern asset_common::ImageLoadData* load(std::string id);
	};
}
