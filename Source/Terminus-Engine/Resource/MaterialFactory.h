#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "../Graphics/Material.h"
#include "../Resource/TextureCache.h"
#include <string>

namespace terminus
{
	class MaterialFactory
	{
	public:
		MaterialFactory();
		~MaterialFactory();
		Material* Create(AssetCommon::TextLoadData* data, TextureCache* textureCache);
	};
}

#endif
