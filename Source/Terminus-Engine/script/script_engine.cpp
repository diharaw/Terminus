#include <script/script_engine.h>
#include <Core/context.h>
#include <ECS/entity.h>
#include <ECS/scene.h>

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
        
        if(!cache.is_loaded(file_name))
        {
            LuaScriptFile* script_file = cache.load(file_name);
            execute_file_lua(script_file);
        }
        
        String instance_name = "obj";
        instance_name += std::to_string(_last_object_id++);
        
        String construct_script = instance_name;
        construct_script += " = ";
        construct_script += class_name;
        construct_script += ".new()";
        
        _lua_state.script(construct_script);
        
        // TODO: Use pool allocator.
        LuaScript* lua_script = new LuaScript();
        lua_script->_instance_name = instance_name;
        lua_script->_object = _lua_state[instance_name];
        
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
        
    }
    
    void ScriptEngine::reload_cpp_script(CppScript* script)
    {
        
    }
    
    void ScriptEngine::on_lua_script_updated(Event* event)
    {
        
    }
    
    void ScriptEngine::on_cpp_script_updated(Event* event)
    {
        
    }
    
    void ScriptEngine::execute_file_lua(LuaScriptFile* script_file)
    {
        _lua_state.script(script_file->c_str());
    }
    
    void ScriptEngine::execute_string_lua(String script)
    {
        _lua_state.script(script);
    }
}
