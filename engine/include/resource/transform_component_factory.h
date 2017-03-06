#pragma once

#include <core/types.h>
#include <gameplay/scene.h>

namespace terminus
{
	namespace transform_component_factory
	{
        void create_internal(TransformComponent& component, JsonValue& json)
        {
            rapidjson::Value& position = json["position"];
            
            component._position.x = position["x"].GetFloat();
            component._position.y = position["y"].GetFloat();
            component._position.z = position["z"].GetFloat();
            
            rapidjson::Value& rotation = json["rotation"];
            
            component._euler_angles = Vector3(rotation["x"].GetFloat(),
                                              rotation["y"].GetFloat(),
                                              rotation["z"].GetFloat());
            
            component._rotation = glm::quat(Vector3(glm::radians(component._euler_angles.x),
                                                    glm::radians(component._euler_angles.y),
                                                    glm::radians(component._euler_angles.z)));
            
            rapidjson::Value& scale = json["scale"];
            
            component._scale.x = scale["x"].GetFloat();
            component._scale.y = scale["y"].GetFloat();
            component._scale.z = scale["z"].GetFloat();
            
            if(!json["parent_entity"].IsNull())
            {
                component._parent_entity_name = std::string(json["parent_entity"].GetString());
            }
        }
        
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            TransformComponent& component = scene->attach_transform_component(entity);
            create_internal(component, json);
		}
	}
}
