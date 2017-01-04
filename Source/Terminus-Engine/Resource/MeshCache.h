#pragma once

#ifndef MESHCACHE_H
#define MESHCACHE_H

#include "AssetCache.h"
#include "MeshFactory.h"
#include "MaterialCache.h"

namespace terminus
{
	class MeshCache : public IAssetCache<MeshFactory>
	{
	private:
		std::unordered_map<std::string, Mesh*> m_AssetMap;
		RenderDevice* m_device;
		MaterialCache* m_material_cache;

	public:
		MeshCache();
		~MeshCache();
		void Initialize(RenderDevice* device, MaterialCache* materialCache);

		Mesh* Load(std::string _ID);
		void Unload(Mesh* mesh);
	};
}

#endif
