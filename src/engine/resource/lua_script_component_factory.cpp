#include <Resource/lua_script_component_factory.h>
#include <ECS/component_types.h>
#include <Core/context.h>

namespace terminus
{
    namespace lua_script_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            ScriptEngine& script_engine = context::get_script_engine();
            
            LuaScriptComponent& component = scene->attach_lua_script_component(entity);
            
            String class_name = String(json["class_name"].GetString());
            String file_name = String(json["script_file"].GetString());
            component._scene = scene;
            component._entity = entity;
            
            LuaScript* script = script_engine.create_lua_script(file_name, class_name);
            
            if(!script)
            {
                scene->_lua_script_pool.remove(entity);
                T_LOG_ERROR("Lua Script Component not attached");
                return;
            }
            else
                component._script = script;
        }
        
#if defined(TERMINUS_WITH_EDITOR)
        void create_from_desc(LuaScriptDesc* desc, Entity& entity, Scene* scene)
        {
            ScriptEngine& script_engine = context::get_script_engine();
            
            LuaScriptComponent& component = scene->attach_lua_script_component(entity);
            
            String class_name = String(desc->_class_name.c_str());
            String file_name = String(desc->_script_name.c_str());
            component._scene = scene;
            component._entity = entity;
            
            LuaScript* script = script_engine.create_lua_script(file_name, class_name);
            
            if(!script)
            {
                scene->_lua_script_pool.remove(entity);
                T_LOG_ERROR("Lua Script Component not attached");
                return;
            }
            else
                component._script = script;
        }
        
        void create(JsonValue& json, EditorEntity& entity)
        {
            LuaScriptDesc* desc = new LuaScriptDesc();
            desc->_class_name = json["class_name"].GetString();
            desc->_script_name = json["script_file"].GetString();
            
            entity.add_component_desc(EditorComponentType::LUA_SCRIPT, desc);
        }
#endif

    }
}
