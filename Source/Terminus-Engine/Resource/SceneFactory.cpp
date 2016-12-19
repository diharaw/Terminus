#include "SceneFactory.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/CameraComponent.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/ScriptComponent.h"

namespace Terminus { namespace Resource {

	SceneFactory::SceneFactory()
	{

	}

	SceneFactory::~SceneFactory()
	{

	}
    
    void SceneFactory::Initialize(MeshCache* meshCache)
    {
        m_mesh_cache = meshCache;
    }

	ECS::Scene* SceneFactory::Create(AssetCommon::TextLoadData* _data)
	{
		JsonDocument doc;
		doc.Parse(_data->buffer);
        
        ECS::Scene* scene = new ECS::Scene();
        
        if (doc.HasMember("scene_name"))
        {
            scene->m_name = String(doc["scene_name"].GetString());
        }
        if (doc.HasMember("entities"))
        {
            rapidjson::Value& entities = doc["entities"];
            
            for (rapidjson::SizeType i = 0; i < entities.Size(); i++)
            {
                String entity_name = std::string(entities[i]["entity_name"].GetString());
                ECS::Entity new_entity = scene->CreateEntity(entity_name);
                
                rapidjson::Value& components = doc["componentss"];
                
                for (rapidjson::SizeType j = 0; i < components.Size(); j++)
                {
                    String component_type = std::string(components[i]["component_type"].GetString());
                    
                    if(component_type == "TRANSFORM")
                    {
                        CreateTransformComponent(components[i], new_entity,scene);
                    }
                    else if(component_type == "CAMERA")
                    {
                        CreateCameraComponent(components[i], new_entity, scene);
                    }
                    else if(component_type == "MESH")
                    {
                        CreateMeshComponent(components[i], new_entity, scene);
                    }
                }
                
            }
            
            return scene;
        }

		// Temp
		return nullptr;
	}
    
    void SceneFactory::CreateTransformComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene)
    {
        ECS::TransformComponent* component = (ECS::TransformComponent*)scene->AttachComponent(entity, ECS::TransformComponent::_id);
        rapidjson::Value& position = value["position"];
        
        component->position.x = position["x"].GetFloat();
        component->position.y = position["y"].GetFloat();
        component->position.z = position["z"].GetFloat();
        
        rapidjson::Value& rotation = value["rotation"];
        
        component->rotation.x = rotation["x"].GetFloat();
        component->rotation.y = rotation["y"].GetFloat();
        component->rotation.z = rotation["z"].GetFloat();
        
        rapidjson::Value& scale = value["scale"];
        
        component->scale.x = scale["x"].GetFloat();
        component->scale.y = scale["y"].GetFloat();
        component->scale.z = scale["z"].GetFloat();
        
        if(!value["parent_entity"].IsNull())
        {
            component->parent_entity_name = std::string(value["parent_entity"].GetString());
        }
    }
    
    void SceneFactory::CreateCameraComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene)
    {
        ECS::CameraComponent* component = (ECS::CameraComponent*)scene->AttachComponent(entity, ECS::CameraComponent::_id);
        
        rapidjson::Value& projection_info = value["projection_info"];
    }
    
    void SceneFactory::CreateMeshComponent(rapidjson::Value& value, ECS::Entity entity, ECS::Scene* scene)
    {
        ECS::MeshComponent* component = (ECS::MeshComponent*)scene->AttachComponent(entity, ECS::MeshComponent::_id);
    }

} }
