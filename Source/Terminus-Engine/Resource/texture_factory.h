#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include <Graphics/render_common.h>
#include <Graphics/render_device.h>
#include <Resource/asset_common.h>
#include <global.h>

namespace terminus
{
	namespace texture_factory
	{
		extern Texture* create(String image_name);
	};
}

#endif
