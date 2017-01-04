#pragma once

#ifndef MATERIALCACHE_H
#define MATERIALCACHE_H

#include "../Resource/AssetCache.h"
#include "MaterialFactory.h"
#include "TextureCache.h"

namespace terminus
{
	class MaterialCache : public IAssetCache<MaterialFactory>
	{
	private:
		std::unordered_map<std::string, Material*> m_MaterialMap;
		TextureCache* m_TextureCache;
		RenderDevice* m_device;

	public:
		MaterialCache();
		~MaterialCache();
		void Initialize(RenderDevice* device, TextureCache* textureCache);
		Material* Load(String key);
		void Unload(Material* material);
	};
}

#endif
