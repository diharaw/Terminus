#include <Resource/scene_factory.h>
#include <Resource/transform_component_factory.h>
#include <Resource/mesh_component_factory.h>
#include <Resource/camera_component_factory.h>
#include <Resource/lua_script_component_factory.h>
#include <Resource/cpp_script_component_factory.h>
#include <Resource/collider_component_factory.h>
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
                        else if(component_type == "SPHERE_COLLIDER")
                        {
                            sphere_collider_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "BOX_COLLIDER")
                        {
                            box_collider_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "CYLINDER_COLLIDER")
                        {
                            cylinder_collider_component_factory::create(components[j], new_entity, scene);
                        }
                        else if(component_type == "CAPSULE_COLLIDER")
                        {
                            capsule_collider_component_factory::create(components[j], new_entity, scene);
                        }
                    }
                }
                
                scene->initialize();
                
                return scene;
            }
            
            // Temp
            return nullptr;
        }
        
#if defined(TERMINUS_WITH_EDITOR)
        Scene* create(EditorScene* editor_scene)
        {
            Scene* scene = new Scene();
            
            scene->_name = editor_scene->_name.c_str();
            
            for(int i = 0; i < editor_scene->_entities.size(); i++)
            {
                EditorEntity& entity = editor_scene->_entities._objects[i];
                Entity& new_entity = scene->create_entity(entity._name.c_str());
                
                for(int j = 0; j < MAX_COMPONENT_DESC; j++)
                {
                    switch (j)
                    {
                        case EditorComponentType::TRANSFORM:
                            transform_component_factory::create_from_desc((TransformDesc*)entity._components[j], new_entity, scene);
                            break;
                            
                        case EditorComponentType::MESH:
                            mesh_component_factory::create_from_desc((MeshDesc*)entity._components[j], new_entity, scene);
                            break;
                            
                        case EditorComponentType::LUA_SCRIPT:
                            lua_script_component_factory::create_from_desc((LuaScriptDesc*)entity._components[j], new_entity, scene);
                            break;
                            
                        case EditorComponentType::CPP_SCRIPT:
                            cpp_script_component_factory::create_from_desc((CppScriptDesc*)entity._components[j], new_entity, scene);
                            break;
                            
                        case EditorComponentType::CAMERA:
                            camera_component_factory::create_from_desc((CameraDesc*)entity._components[j], new_entity, scene);
                            break;
                            
                        case EditorComponentType::BOX_COLLIDER:
                            break;
                            
                        case EditorComponentType::SPHERE_COLLIDER:
                            break;
                            
                        case EditorComponentType::HEIGHT_FIELD_COLLIDER:
                            break;
                            
                        case EditorComponentType::TRIANGLE_MESH_COLLIDER:
                            break;
                            
                        default:
                            break;
                    }
                }
            }
            
            scene->initialize();
            
            return scene;
        }
        
        EditorScene* create_editor_scene(asset_common::TextLoadData* _data)
        {
            JsonDocument doc;
            doc.Parse(_data->buffer);
            
            EditorScene* scene = new EditorScene();
            
            if (doc.HasMember("scene_name"))
            {
                scene->_name = doc["scene_name"].GetString();
            }
            if (doc.HasMember("entities"))
            {
                rapidjson::Value& entities = doc["entities"];
                
                for (rapidjson::SizeType i = 0; i < entities.Size(); i++)
                {
                    EditorEntity& new_entity = scene->create_entity(entities[i]["entity_name"].GetString());
                    
                    rapidjson::Value& components = entities[i]["components"];
                    
                    for (rapidjson::SizeType j = 0; j < components.Size(); j++)
                    {
                        String component_type = std::string(components[j]["component_type"].GetString());
                        
                        if(component_type == "TRANSFORM")
                        {
                            transform_component_factory::create(components[j], new_entity);
                        }
                        else if(component_type == "CAMERA")
                        {
                            camera_component_factory::create(components[j], new_entity);
                        }
                        else if(component_type == "MESH")
                        {
                            mesh_component_factory::create(components[j], new_entity);
                        }
                        else if(component_type == "LUA_SCRIPT")
                        {
                            lua_script_component_factory::create(components[j], new_entity);
                        }
                        else if(component_type == "CPP_SCRIPT")
                        {
                            cpp_script_component_factory::create(components[j], new_entity);
                        }
                    }
                }
            }
            
            return scene;
        }
#endif
    }
}
