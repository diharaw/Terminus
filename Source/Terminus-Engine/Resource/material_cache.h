#pragma once

#ifndef MATERIALCACHE_H
#define MATERIALCACHE_H

#include <Resource/asset_cache.h>
#include <Resource/material_factory.h>
#include <Resource/texture_cache.h>

namespace terminus
{
	class MaterialCache : public IAssetCache<MaterialFactory>
	{
	private:
		std::unordered_map<std::string, Material*> m_MaterialMap;

	public:
		MaterialCache();
		~MaterialCache();
		void Initialize();
		Material* Load(String key);
		void Unload(Material* material);
	};
}

#endif
