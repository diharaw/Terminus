#pragma once

#include <ECS/component_pool.h>
#include <script/lua_script.h>
#include <script/cpp_script.h>
#include <Core/Event/event_handler.h>
#include <Core/Event/event.h>
#include <types.h>

#include <container/hash_map.h>

#define MAX_SCRIPT_INSTANCES 10
#define MAX_SCRIPTS 10

namespace terminus
{
	struct LuaScriptListener
	{
		LuaFunction _callback;
		LuaObject   _object;
		ListenerID  _listener_id;
	};

	using LuaListenerArray = PackedArray<LuaScriptListener, MAX_EVENT_LISTENERS>;
	using LuaListenerMap = std::map<EventType, LuaListenerArray>;
    
    // development only
    
    struct LuaScriptInstanceList
    {
        LuaScript* _instances[MAX_SCRIPT_INSTANCES];
        String     _class_name;
        
        void initialize()
        {
            for (int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
                    _instances[i] = nullptr;
        }
        
        void add_instance(LuaScript* script)
        {
            for (int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(!_instances[i])
                {
                    _instances[i] = script;
                    return;
                }
            }
        }
        
        void remove_instance(LuaScript* script)
        {
            for (int i = 0; i < MAX_SCRIPT_INSTANCES; i++)
            {
                if(_instances[i]->_id == script->_id)
                {
                    _instances[i] = nullptr;
                    return;
                }
            }
        }
    };

	class ScriptEngine
	{
	public:
		ScriptEngine();
		~ScriptEngine();
		void initialize();
		void shutdown();
        LuaScript* create_lua_script(String file_name, String class_name);
        CppScript* create_cpp_script(String file_name, String class_name);
        
        void destroy_lua_script(LuaScript* script);
        void destroy_cpp_script(CppScript* script);
        
        bool execute_file_lua(LuaScriptFile* script_file);
		bool execute_string_lua(String script);
        
        void reload_lua_script(String file_name);
        void reload_cpp_script(String file_name);
        
        // events
        void on_lua_script_updated(Event* event);
        void on_cpp_script_updated(Event* event);
		//void on_lua_script_event(Event* event);
		//ListenerID register_script_listener(EventType type, LuaFunction lua_callback, LuaObject object);
		//void unregister_script_listener(EventType type, ListenerID listener);

    private:
		void register_math_api();
		void register_component_api();
		void register_scene_api();
		void register_scene_manager_api();
		void register_logger_api();
		void register_event_api();
        void register_input_api();
		void register_platform_api();
        void register_misc_api();
        
	private:
		sol::state     _lua_state;
        uint32_t	   _last_object_id;
        
        // development only
        HashMap<LuaScriptInstanceList, MAX_SCRIPTS> _script_instance_map;
	};
}
