#pragma once

#include <ECS/component_pool.h>
#include <script/lua_script.h>
#include <script/cpp_script.h>
#include <Core/Event/event.h>
#include <types.h>

namespace terminus
{
	struct LuaScriptListener
	{
		LuaFunction _callback;
		LuaObject   _object;
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
        
        void execute_file_lua(LuaScriptFile* script_file);
		void execute_string_lua(String script);
        
        void reload_lua_script(LuaScript* script);
        void reload_cpp_script(CppScript* script);
        
        // events
        void on_lua_script_updated(Event* event);
        void on_cpp_script_updated(Event* event);

    private:
        void register_lua_api();
        
	private:
		sol::state _lua_state;
        uint32_t   _last_object_id;
	};
}
