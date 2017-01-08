#pragma once

#ifndef MESHCACHE_H
#define MESHCACHE_H

#include <Resource/asset_cache.h>
#include <Resource/mesh_factory.h>
#include <Resource/material_cache.h>

namespace terminus
{
	class MeshCache : public IAssetCache<MeshFactory>
	{
	private:
		std::unordered_map<std::string, Mesh*> m_AssetMap;

	public:
		MeshCache();
		~MeshCache();
		void Initialize();

		Mesh* Load(std::string _ID);
		void Unload(Mesh* mesh);
	};
}

#endif
