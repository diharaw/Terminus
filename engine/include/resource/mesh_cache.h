#pragma once

#include <Resource/mesh_factory.h>
#include <Resource/material_cache.h>
#include <types.h>

#include <unordered_map>

namespace terminus
{
	class MeshCache
	{
	private:
		std::unordered_map<std::string, Mesh*> m_AssetMap;

	public:
		MeshCache();
		~MeshCache();

		Mesh* load(String id);
		void unload(Mesh* mesh);
	};
} // namespace terminus
