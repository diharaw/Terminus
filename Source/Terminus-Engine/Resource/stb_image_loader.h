#pragma once

#ifndef STBIMAGELOADER_H
#define STBIMAGELOADER_H

#include <Resource/asset_common.h>
#include <types.h>

namespace terminus
{
	namespace stb_image_loader
	{
		extern asset_common::ImageLoadData* load(std::string id);
	};
}

#endif
