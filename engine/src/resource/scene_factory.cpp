#include <resource/scene_factory.h>
#include <resource/transform_component_factory.h>
#include <resource/mesh_component_factory.h>
#include <resource/camera_component_factory.h>
#include <resource/lua_script_component_factory.h>
#include <resource/cpp_script_component_factory.h>
#include <resource/collider_component_factory.h>
#include <resource/rigid_body_component_factory.h>
#include <resource/sky_component_factory.h>
#include <core/context.h>

namespace terminus
{
	namespace scene_factory
    {
        Scene* create(asset_common::TextLoadData* _data)
        {
            JsonDocument doc;
            doc.Parse(_data->buffer);
            
            Scene* scene = new Scene();
            
            if (doc.HasMember("scene_name"))
            {
                scene->_name = String(doc["scene_name"].GetString());
            }
            if (doc.HasMember("entities"))
            {
                rapidjson::Value& entities = doc["entities"];
                
                for (rapidjson::SizeType i = 0; i < entities.Size(); i++)
                {
                    String entity_name = std::string(entities[i]["entity_name"].GetString());
                    Entity new_entity = scene->create_entity(entity_name);
                    
                    rapidjson::Value& components = entities[i]["components"];
                    
                    for (rapidjson::SizeType j = 0; j < components.Size(); j++)
                    {
                        String component_type = std::string(components[j]["component_type"].GetString());
                        
                        if(component_type == "TRANSFORM")
                        {
                            transform_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "CAMERA")
                        {
                            camera_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "MESH")
                        {
                            mesh_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "LUA_SCRIPT")
                        {
                            lua_script_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "CPP_SCRIPT")
                        {
                            cpp_script_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "COLLIDER")
                        {
                            collider_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "RIGID_BODY")
                        {
                            rigid_body_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "SKY")
                        {
                            sky_component_factory::create(components[j], new_entity, scene);
                        }
                    }
                }
                
                scene->initialize();
                
                return scene;
            }
            
            // Temp
            return nullptr;
        }
    }
}
