#include <gameplay/script_system.h>
#include <gameplay/scene.h>
#include <script/lua_script.h>
#include <script/cpp_script.h>

namespace terminus
{
    ScriptSystem::ScriptSystem()
    {
        
    }
    
    ScriptSystem::~ScriptSystem()
    {
        
    }
    
    void ScriptSystem::initialize(Scene* scene)
    {
        _scene = scene;
        
        // for all cpp components
        CppScriptComponent* cpp_scripts = _scene->_cpp_script_pool.get_array();
        int num_cpp_scripts = _scene->_cpp_script_pool.get_num_objects();
        
        for (int i = 0; i < num_cpp_scripts; i++)
        {
            cpp_scripts[i]._script->initialize();
        }
        
        // for all lua components
        LuaScriptComponent* lua_scripts = _scene->_lua_script_pool.get_array();
        int num_lua_scripts = _scene->_lua_script_pool.get_num_objects();
        
        for (int i = 0; i < num_lua_scripts; i++)
        {
            lua_scripts[i]._script->initialize(lua_scripts[i]._scene, lua_scripts[i]._entity);
        }
    }
    
    void ScriptSystem::simulate(double delta)
    {
        CppScriptComponent* cpp_scripts = _scene->_cpp_script_pool.get_array();
        int num_cpp_scripts = _scene->_cpp_script_pool.get_num_objects();
        
        // for all cpp components
        for (int i = 0; i < num_cpp_scripts; i++)
        {
            cpp_scripts[i]._script->update(delta);
        }
        
        // for all lua components
        LuaScriptComponent* lua_scripts = _scene->_lua_script_pool.get_array();
        int num_lua_scripts = _scene->_lua_script_pool.get_num_objects();
        
        for (int i = 0; i < num_lua_scripts; i++)
        {
            lua_scripts[i]._script->update(delta);
        }
    }
    
    void ScriptSystem::shutdown()
    {
        CppScriptComponent* cpp_scripts = _scene->_cpp_script_pool.get_array();
        int num_cpp_scripts = _scene->_cpp_script_pool.get_num_objects();
        
        // for all cpp components
        for (int i = 0; i < num_cpp_scripts; i++)
        {
            cpp_scripts[i]._script->shutdown();
        }
        
        // for all lua components
        LuaScriptComponent* lua_scripts = _scene->_lua_script_pool.get_array();
        int num_lua_scripts = _scene->_lua_script_pool.get_num_objects();
        
        for (int i = 0; i < num_lua_scripts; i++)
        {
            lua_scripts[i]._script->shutdown();
        }
    }
    
    void ScriptSystem::on_entity_created(Entity entity)
    {
        
    }
    
    void ScriptSystem::on_entity_destroyed(Entity entity)
    {
        
    }
}
