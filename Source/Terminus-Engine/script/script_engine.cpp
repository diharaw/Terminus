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
        
        _lua_state.script(construct_script);
        
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
    
    void ScriptEngine::reload_lua_script(LuaScript* script)
    {
        String instance_name = "obj";
        instance_name += std::to_string(_last_object_id++);
        
        String construct_script = instance_name;
        construct_script += " = ";
        construct_script += script->_class_name;
        construct_script += ":new()";
        
        _lua_state.script(construct_script);
        LuaObject new_object = _lua_state[instance_name];
        
        LuaObject new_properties = new_object["property"];
        LuaObject old_properties = script->_object["property"];
        
        new_object["_entity"] = script->_object["_entity"];
        new_object["_scene"] = script->_object["_scene"];
        
        for(auto member : new_properties)
        {
            const std::string member_name = member.first.as<std::string>();
            auto lua_member = old_properties[member_name];
            new_properties[member_name] = old_properties[member_name];
        }
        
        script->_initialize = script->_object["initialize"];
        script->_update = script->_object["update"];
        script->_shutdown = script->_object["shutdown"];
        
        _lua_state[script->_instance_name] = sol::lua_nil;
        _lua_state.collect_garbage();
        
        script->_object = new_object;
    }
    
    void ScriptEngine::reload_cpp_script(CppScript* script)
    {
        
    }
    
    void ScriptEngine::on_lua_script_updated(Event* event)
    {
        LuaScriptUpdatedEvent* event_data = (LuaScriptUpdatedEvent*)event;
    
        LuaScriptFileCache& cache = context::get_lua_script_file_cache();
        cache.unload(HASH(event_data->get_script_name().c_str()));
        LuaScriptFile* script_file = cache.load(event_data->get_script_name());
        
        LuaScriptInstanceList* instance_list = _script_instance_map.get_ref(HASH(event_data->get_script_name().c_str()));
        bool first = true;
        
        if(instance_list)
        {
            for(int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(instance_list->_instances[i])
                {
                    if(first)
                    {
                        _lua_state[instance_list->_instances[i]->_class_name] = sol::nil;
                        _lua_state.collect_garbage();
                        execute_file_lua(script_file);
                        
                        first = false;
                    }
                    
                    LuaScript* script = instance_list->_instances[i];
                    reload_lua_script(script);
                }
            }
        }
    }
    
    void ScriptEngine::on_cpp_script_updated(Event* event)
    {
        
    }
    
    void ScriptEngine::execute_file_lua(LuaScriptFile* script_file)
    {
        try
        {
            _lua_state.script(script_file->c_str());
        }
        catch(sol::error error)
        {
            std::cout << error.what() << std::endl;
        }
    }
    
    void ScriptEngine::execute_string_lua(String script)
    {
        _lua_state.script(script);
    }
}
