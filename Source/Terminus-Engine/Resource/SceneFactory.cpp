#include "SceneFactory.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/CameraComponent.h"
#include "../ECS/MeshComponent.h"
#include "../ECS/ScriptComponent.h"
#include <Core/context.h>

namespace terminus
{
	SceneFactory::SceneFactory()
	{

	}

	SceneFactory::~SceneFactory()
	{

	}
    
    void SceneFactory::Initialize()
    {
        
    }

	Scene* SceneFactory::Create(AssetCommon::TextLoadData* _data)
	{
		JsonDocument doc;
		doc.Parse(_data->buffer);
        
        Scene* scene = new Scene();
        
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
                Entity new_entity = scene->CreateEntity(entity_name);
                
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
    
    void SceneFactory::CreateTransformComponent(rapidjson::Value& value, Entity entity, Scene* scene)
    {
        TransformComponent* component = (TransformComponent*)scene->AttachComponent(entity, TransformComponent::_id);
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
    
    void SceneFactory::CreateCameraComponent(rapidjson::Value& value, Entity entity, Scene* scene)
    {
        CameraComponent* component = (CameraComponent*)scene->AttachComponent(entity, CameraComponent::_id);
        
        rapidjson::Value& projection_info = value["projection_info"];
        
        {
            component->camera.SetFarPlane(projection_info["far_plane"].GetFloat());
            component->camera.SetNearPlane(projection_info["near_plane"].GetFloat());
            
            String proj_type = String(projection_info["projection_type"].GetString());
            
            if(proj_type == "PERSPECTIVE")
                component->camera.SetProjectionType(ProjectionType::PERSPECTIVE);
            else
                component->camera.SetProjectionType(ProjectionType::ORTHOGRAPHIC);
            
            component->camera.SetFoV(projection_info["field_of_view"].GetFloat());
            
            float aspect_x = projection_info["aspect_ratio_x"].GetFloat();
            float aspect_y = projection_info["aspect_ratio_y"].GetFloat();
            
            component->camera.SetAspectRatio(aspect_x / aspect_y);
        }
        
        component->is_active = value["is_active"].GetBool();
        component->is_offscreen = value["is_offscreen"].GetBool();
        
        rapidjson::Value& viewport_rect = value["viewport_rect"];
        
        {
            component->screen_rect.x = viewport_rect["x"].GetFloat();
            component->screen_rect.y = viewport_rect["y"].GetFloat();
            component->screen_rect.z = viewport_rect["w"].GetFloat();
            component->screen_rect.w = viewport_rect["h"].GetFloat();
        }
    }
    
    void SceneFactory::CreateMeshComponent(rapidjson::Value& value, Entity entity, Scene* scene)
    {
        MeshComponent* component = (MeshComponent*)scene->AttachComponent(entity, MeshComponent::_id);
        
        component->mesh = context::get_mesh_cache().Load(std::string(value["mesh"].GetString()));
        
        // TODO: Material Overrides
    }
}
