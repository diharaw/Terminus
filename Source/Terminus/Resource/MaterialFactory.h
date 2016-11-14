#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "../Graphics/Material.h"
#include "../Resource/TextureCache.h"
#include <string>

namespace Terminus { namespace Resource {

	class MaterialFactory
	{
	public:
		MaterialFactory();
		~MaterialFactory();
		Graphics::Material* Create(asset_common::TextLoadData* data, TextureCache* textureCache);
	};

} }

#endif
