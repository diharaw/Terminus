#pragma once

#include "../ECS/Scene.h"
#include "AssetCommon.h"
#include "MeshCache.h"

namespace terminus
{
		class SceneFactory
		{
        private:
            MeshCache* m_mesh_cache;
            
		public:
			SceneFactory();
			~SceneFactory();
            void Initialize(MeshCache* meshCache);
			Scene* Create(AssetCommon::TextLoadData* _data);
            
        private:
            void CreateTransformComponent(rapidjson::Value& value, Entity entity, Scene* scene);
            void CreateCameraComponent(rapidjson::Value& value, Entity entity, Scene* scene);
            void CreateMeshComponent(rapidjson::Value& value, Entity entity, Scene* scene);
		};
} // namespace terminus
