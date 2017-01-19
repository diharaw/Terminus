#pragma once

#include <ECS/component_pool.h>
#include <types.h>

namespace terminus
{
	struct ScriptListener
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
        void execute_file(String script);
		void execute_string(String script);

	private:
		sol::state _lua_state;
	};
}
