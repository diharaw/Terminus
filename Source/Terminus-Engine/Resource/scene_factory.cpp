#include <Resource/scene_factory.h>
#include <Resource/transform_component_factory.h>
#include <Resource/mesh_component_factory.h>
#include <Resource/camera_component_factory.h>
#include <Resource/lua_script_component_factory.h>
#include <Resource/cpp_script_component_factory.h>
#include <Core/context.h>

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
                    }
                }
                
                return scene;
            }
            
            // Temp
            return nullptr;
        }

    }
}
