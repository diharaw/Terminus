#include <script/script_engine.h>
#include <ECS/entity.h>
#include <ECS/scene.h>

namespace terminus
{
	ScriptEngine::ScriptEngine()
	{

	}

	ScriptEngine::~ScriptEngine()
	{

	}

	void ScriptEngine::initialize()
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
    
    void ScriptEngine::execute_file(String script)
    {
        
    }

	void ScriptEngine::execute_string(String script)
	{
		_lua_state.script(script);
	}
}
