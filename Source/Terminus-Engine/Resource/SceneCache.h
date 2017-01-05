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

		public:
			SceneCache();
			~SceneCache();
            void Initialize();
			Scene* Load(String _ID);
			void Unload(Scene* scene);
		};
}

#endif
