#include <script/script_engine.h>
#include <Core/context.h>
#include <ECS/entity.h>
#include <ECS/scene.h>
#include <Core/Event/event.h>

namespace terminus
{
    ScriptEngine::ScriptEngine()
    {
        _last_object_id = 0;
    }
    
    ScriptEngine::~ScriptEngine()
    {
        
    }
    
    void ScriptEngine::initialize()
    {
        _lua_state.open_libraries(sol::lib::base, sol::lib::package);
        register_lua_api();
    }
    
    void ScriptEngine::register_lua_api()
    {
        _lua_state.new_usertype<Scene>("Scene",
                                      sol::constructors<sol::types<>>(),
                                      "get_transform_component", &Scene::get_transform_component,
                                      "get_mesh_component", &Scene::get_mesh_component,
                                      "get_collider_component", &Scene::get_collider_component,
                                      "get_camera_component", &Scene::get_camera_component,
                                      "get_lua_script_component", &Scene::get_lua_script_component,
                                      "has_transform_component", &Scene::has_transform_component,
                                      "has_mesh_component", &Scene::has_mesh_component,
                                      "has_collider_component", &Scene::has_collider_component,
                                      "has_camera_component", &Scene::has_camera_component,
                                      "has_lua_script_component", &Scene::has_lua_script_component,
                                      "is_entity_alive", &Scene::is_alive);
        
        _lua_state.new_usertype<Entity>("Entity",
                                        sol::constructors<sol::types<>>(),
                                        "_id", &Entity::_id,
                                        "_name", &Entity::_name
                                        );
        // register logger
        _lua_state.set_function("T_LOG_INFO", &logger::log_info);
        _lua_state.set_function("T_LOG_WARNING", &logger::log_warning);
        _lua_state.set_function("T_LOG_ERROR", &logger::log_error);
        _lua_state.set_function("T_LOG_FATAL", &logger::log_fatal);
    }
    
    void ScriptEngine::shutdown()
    {
        
    }
    
    LuaScript* ScriptEngine::create_lua_script(String file_name, String class_name)
    {
        LuaScriptFileCache& cache = context::get_lua_script_file_cache();
        
        String instance_name = "obj";
        instance_name += std::to_string(_last_object_id++);
        
        // development only
        if(!_script_instance_map.has(HASH(file_name.c_str())))
        {
            LuaScriptInstanceList new_list;
            new_list.initialize();
            new_list._class_name = class_name;
            _script_instance_map.set(HASH(file_name.c_str()), new_list);
        }
    
        if(!cache.is_loaded(file_name))
        {
            LuaScriptFile* script_file = cache.load(file_name);
            execute_file_lua(script_file);
        }
        
        String construct_script = instance_name;
        construct_script += " = ";
        construct_script += class_name;
        construct_script += ":new()";
        
        if(!execute_string_lua(construct_script))
        {
            T_LOG_ERROR("Failed to execute Lua String");
            return nullptr;
        }
        
        // TODO: Use pool allocator.
        LuaScript* lua_script = new LuaScript();
        
        lua_script->_instance_name = instance_name;
        lua_script->_class_name = class_name;
        lua_script->_id = _last_object_id - 1;
        lua_script->_object = _lua_state[instance_name];
        
        _script_instance_map.get_ref(HASH(file_name.c_str()))->add_instance(lua_script);
        
        return lua_script;
    }
    
    CppScript* ScriptEngine::create_cpp_script(String file_name, String class_name)
    {
        DynamicLibraryCache& cache = context::get_dynamic_library_cache();
        dynamic_library::LibHandle* handle = cache.load(file_name);
        
        String factory_name = "Create";
        factory_name += class_name;
        
        CppScript* cpp_script = dynamic_library::create_instance_from_factory<CppScript>(factory_name.c_str(), *handle);
        return cpp_script;
    }
    
    void ScriptEngine::destroy_lua_script(LuaScript* script)
    {
        if(script)
        {
            _lua_state[script->_instance_name] = sol::lua_nil;
            _lua_state.collect_garbage();
            
            // TODO: remove from instance list
            // temp
            T_SAFE_DELETE(script);
        }
    }
    
    void ScriptEngine::destroy_cpp_script(CppScript* script)
    {
        if(script)
        {
            // temp
            T_SAFE_DELETE(script);
        }
    }
    
    void ScriptEngine::reload_lua_script(String file_name)
    {
        LuaScriptFileCache& cache = context::get_lua_script_file_cache();
        cache.unload(HASH(file_name.c_str()));
        LuaScriptFile* script_file = cache.load(file_name);
        
        LuaScriptInstanceList* instance_list = _script_instance_map.get_ref(HASH(file_name.c_str()));
        
        int last_temp_obj_id = 0;
        
        if(instance_list)
        {
            // first pass to copy state into temporary tables
            for(int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(instance_list->_instances[i])
                {
                    // copy state into temporary table
                    std::string temp_obj_name = "temp_obj_";
                    temp_obj_name += std::to_string(last_temp_obj_id++);
                    
                    sol::table temp_obj = _lua_state.create_table(temp_obj_name);
                    sol::table old_properties = instance_list->_instances[i]->_object["property"];
                    
                    temp_obj["_entity"] = instance_list->_instances[i]->_object["_entity"];
                    temp_obj["_scene"] = instance_list->_instances[i]->_object["_scene"];
                    
                    for(auto member : old_properties)
                    {
                        std::string new_member = member.first.as<std::string>();
                        temp_obj[new_member] = member.second;
                    }
                }
            }
            
            // initial run to check for errors
            if(!execute_file_lua(script_file))
            {
                T_LOG_ERROR("Failed to Hot Reload Lua Script : Failed to execute Lua Script File");
                
                // delete temp objects since loading failed
                for(int i = 0; i < last_temp_obj_id; i++)
                {
                    std::string temp_obj_name = "temp_obj_";
                    temp_obj_name += std::to_string(i);
                    
                    _lua_state[temp_obj_name] = sol::lua_nil;
                    _lua_state.collect_garbage();
                }
                
                return;
            }
            
            // second pass to delete instances if script is valid
            for(int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(instance_list->_instances[i])
                {
                    _lua_state[instance_list->_instances[i]->_instance_name] = sol::lua_nil;
                    _lua_state.collect_garbage();
                }
            }
            
            // destroy initial class
            _lua_state[instance_list->_class_name] = sol::lua_nil;
            _lua_state.collect_garbage();
            
            // run script again after old one is deleted
            execute_file_lua(script_file);

            // second pass to instantiate new instances and copy data from temporary tables
            for(int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(instance_list->_instances[i])
                {
                    std::string temp_obj_name = "temp_obj_";
                    temp_obj_name += std::to_string(i);
                    
                    sol::table temp_obj = _lua_state[temp_obj_name];
                    
                    std::string command = "obj";
                    command += std::to_string(_last_object_id++);
                    
                    std::string instance_name = command;
                    
                    command += " = ";
                    command += instance_list->_class_name;
                    command += ":new()";
                    
                    if(!execute_string_lua(command))
                    {
                        T_LOG_ERROR("Failed to Hot Reload Lua Script : Failed to execute Lua String");
                        return;
                    }
                    
                    sol::table obj = _lua_state[instance_name];
                    sol::table new_properties = obj["property"];
                    
                    obj["_entity"] = temp_obj["_entity"];
                    obj["_scene"] = temp_obj["_scene"];
                    
                    for (auto member : new_properties)
                    {
                        std::string new_member = member.first.as<std::string>();
                        sol::object temp = temp_obj[new_member];
                        
                        if(temp != sol::nil)
                            new_properties[new_member] = temp;
                    }
                    
                    instance_list->_instances[i]->_id = _last_object_id - 1;
                    instance_list->_instances[i]->_object = obj;
                    instance_list->_instances[i]->_initialize = obj["initialize"];
                    instance_list->_instances[i]->_update = obj["update"];
                    instance_list->_instances[i]->_shutdown = obj["shutdown"];
                    
                    _lua_state[temp_obj_name] = sol::lua_nil;
                    _lua_state.collect_garbage();
                }
            }
        }
    }
    
    void ScriptEngine::reload_cpp_script(String file_name)
    {
        
    }
    
    void ScriptEngine::on_lua_script_updated(Event* event)
    {
        LuaScriptUpdatedEvent* event_data = (LuaScriptUpdatedEvent*)event;
        reload_lua_script(event_data->get_script_name());
    }
    
    void ScriptEngine::on_cpp_script_updated(Event* event)
    {
        
    }
    
    bool ScriptEngine::execute_file_lua(LuaScriptFile* script_file)
    {
        try
        {
            _lua_state.script(script_file->c_str());
        }
        catch(sol::error error)
        {
            T_LOG_ERROR(error.what());
            return false;
        }
        return true;
    }
    
    bool ScriptEngine::execute_string_lua(String script)
    {
        try
        {
            _lua_state.script(script);
        }
        catch(sol::error error)
        {
            T_LOG_ERROR(error.what());
            return false;
        }
        return true;
    }
}
