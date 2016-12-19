#pragma once

#ifndef  SCENEFACTORY_H
#define SCENEFACTORY_H

#include "../ECS/Scene.h"
#include "AssetCommon.h"
#include "MeshCache.h"

namespace Terminus { namespace Resource {

		class SceneFactory
		{
        private:
            MeshCache* m_mesh_cache;
            
		public:
			SceneFactory();
			~SceneFactory();
            void Initialize(MeshCache* meshCache);
			ECS::Scene* Create(AssetCommon::TextLoadData* _data);
            
        private:
            void CreateTransformComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene);
            void CreateCameraComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene);
            void CreateMeshComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene);
		};

} }

#endif
