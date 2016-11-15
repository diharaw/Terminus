#pragma once

#ifndef MATERIALCACHE_H
#define MATERIALCACHE_H

#include "../Resource/AssetCache.h"
#include "MaterialFactory.h"
#include "TextureCache.h"

namespace Terminus { namespace Resource {

	class MaterialCache : public IAssetCache<MaterialFactory>
	{
	private:
		std::unordered_map<std::string, Graphics::Material*> m_MaterialMap;
		TextureCache* m_TextureCache;
		Graphics::RenderDevice* m_device;

	public:
		MaterialCache();
		~MaterialCache();
		void Initialize(Graphics::RenderDevice* device, TextureCache* textureCache);
		Graphics::Material* Load(String key);
		void Unload(Graphics::Material* material);
	};

} }

#endif
