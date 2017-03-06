#pragma once

#include <resource/mesh_factory.h>
#include <resource/material_cache.h>
#include <core/types.h>

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
