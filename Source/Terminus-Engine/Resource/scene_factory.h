#pragma once

#include <ECS/scene.h>
#include <Resource/asset_common.h>
#include <Resource/mesh_cache.h>

namespace terminus
{
		class SceneFactory
		{
		public:
			SceneFactory();
			~SceneFactory();
            void Initialize();
			Scene* Create(AssetCommon::TextLoadData* _data);
            
        private:
            void CreateTransformComponent(rapidjson::Value& value, Entity entity, Scene* scene);
            void CreateCameraComponent(rapidjson::Value& value, Entity entity, Scene* scene);
            void CreateMeshComponent(rapidjson::Value& value, Entity entity, Scene* scene);
		};
} // namespace terminus
