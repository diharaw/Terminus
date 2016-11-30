#pragma once

#ifndef  SCENECACHE_H
#define SCENECACHE_H

#include "AssetCache.h"
#include "SceneFactory.h"

namespace Terminus { namespace Resource {

		class SceneCache : public IAssetCache<SceneFactory>
		{
		private:
			std::unordered_map<String, ECS::Scene*> m_AssetMap;

		public:
			SceneCache();
			~SceneCache();
			ECS::Scene* Load(String _ID);
			void Unload(ECS::Scene* scene);
		};

} }

#endif