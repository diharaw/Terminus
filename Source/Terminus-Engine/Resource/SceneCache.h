#pragma once

#ifndef  SCENECACHE_H
#define SCENECACHE_H

#include "AssetCache.h"
#include "SceneFactory.h"

namespace terminus
{
		class SceneCache : public IAssetCache<SceneFactory>
		{
		private:
			std::unordered_map<String, Scene*> m_AssetMap;
            MeshCache* m_mesh_cache;

		public:
			SceneCache();
			~SceneCache();
            void Initialize(MeshCache* meshCache);
			Scene* Load(String _ID);
			void Unload(Scene* scene);
		};
}

#endif
