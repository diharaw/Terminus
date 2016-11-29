#pragma once

#ifndef  SCENEFACTORY_H
#define SCENEFACTORY_H

#include "../ECS/Scene.h"
#include "AssetCommon.h"

namespace Terminus { namespace Resource {

		class SceneFactory
		{
		public:
			SceneFactory();
			~SceneFactory();
			ECS::Scene* Create(AssetCommon::TextLoadData* _data);
		};

} }

#endif
