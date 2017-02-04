#include <script/script_engine.h>
#include <Core/context.h>
#include <ECS/entity.h>
#include <ECS/scene.h>
#include <types.h>
#include <Math/math_utility.h>

namespace terminus
{
	float dot_product(Vector3& a, Vector3& b)
	{
		return glm::dot(a, b);
	}

	Vector3 cross_product(Vector3& a, Vector3& b)
	{
		return glm::cross(a, b);
	}

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
        
		register_math_api();
		register_component_api();
		register_scene_api();
		register_scene_manager_api();
		register_logger_api();
		register_event_api();
    }
    
	void ScriptEngine::register_logger_api()
	{
		// register logger
		_lua_state.set_function("log_info", &logger::log_info);
		_lua_state.set_function("log_warning", &logger::log_warning);
		_lua_state.set_function("log_error", &logger::log_error);
		_lua_state.set_function("log_fatal", &logger::log_fatal);
	}

	void ScriptEngine::register_math_api()
	{
		sol::table math_root = _lua_state.create_table("math");

		_lua_state.new_usertype<Vector2>("Vector2",
			sol::constructors<sol::types<>, sol::types<float, float>>(),
			"x", &Vector2::x,
			"y", &Vector2::y,
			sol::meta_function::addition, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+),
			sol::meta_function::subtraction, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-),
			sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, float)>(glm::operator*),
			sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, float)>(glm::operator/)
			);

		_lua_state.new_usertype<Vector3>("Vector3",
			sol::constructors<sol::types<>, sol::types<float, float, float>>(),
			"x", &Vector3::x,
			"y", &Vector3::y,
			"z", &Vector3::z,
			sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
			sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
			sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, float)>(glm::operator*),
			sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, float)>(glm::operator/)
			);

		_lua_state.new_usertype<Vector4>("Vector4",
			sol::constructors<sol::types<>, sol::types<float, float, float, float>>(),
			"x", &Vector4::x,
			"y", &Vector4::y,
			"z", &Vector4::z,
			"w", &Vector4::w,
			sol::meta_function::addition, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator+),
			sol::meta_function::subtraction, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator-),
			sol::meta_function::multiplication, sol::resolve<glm::vec4(const glm::vec4&, float)>(glm::operator*),
			sol::meta_function::division, sol::resolve<glm::vec4(const glm::vec4&, float)>(glm::operator/)
			);

		_lua_state.new_usertype<Matrix3>("Matrix3",
			sol::constructors<sol::types<>>(),
			sol::meta_function::multiplication, sol::resolve<glm::mat3(const glm::mat3&, const glm::mat3&)>(glm::operator*),
			sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::mat3&, const glm::vec3&)>(glm::operator*)
			);

		_lua_state.new_usertype<Matrix4>("Matrix4",
			sol::constructors<sol::types<>>(),
			sol::meta_function::multiplication, sol::resolve<glm::mat4(const glm::mat4&, const glm::mat4&)>(glm::operator*),
			sol::meta_function::multiplication, sol::resolve<glm::vec4(const glm::mat4&, const glm::vec4&)>(glm::operator*)
			);

		math_root.set_function("dot", &dot_product);
		math_root.set_function("cross", &cross_product);
	}

	void ScriptEngine::register_event_api()
	{
		sol::table event_root = _lua_state.create_table("event_handler");

		event_root.set_function("register_listener", &ScriptEngine::register_script_listener);
		event_root.set_function("unregister_listener", &ScriptEngine::unregister_script_listener);
		event_root.set_function("queue_event", &EventHandler::queue_event);
		event_root.set_function("fire_event", &EventHandler::fire_event);

		_lua_state.new_usertype<InputActionEvent>("InputActionEvent",
			sol::constructors<sol::types<std::string>>()
			);

		_lua_state.new_usertype<InputStateEvent>("InputStateEvent",
			sol::constructors<sol::types<std::string, int>>()
			);

		_lua_state.new_usertype<InputAxisEvent>("InputAxisEvent",
			sol::constructors<sol::types<std::string, double, double>>()
			);
	}

	void ScriptEngine::register_component_api()
	{

	}

	void ScriptEngine::register_scene_api()
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

	void ScriptEngine::register_scene_manager_api()
	{
		_lua_state.new_usertype<SceneManager>("SceneManager",
			sol::constructors<sol::types<>>(),
			"load_scene", &SceneManager::load,
			"preload_scene", &SceneManager::preload,
			"unload_scene", &SceneManager::unload,
			"set_active_scene", &SceneManager::set_active_scene);
	}

	void ScriptEngine::on_lua_script_event(Event* event)
	{
		EventType type = event->GetType();

		if (_listener_map.find(type) == _listener_map.end())
		{
			LuaListenerArray& listener_array = _listener_map[type];

			for (int i = 0; i < listener_array._num_objects; i++)
			{
				LuaScriptListener& listener = listener_array._objects[i];
				// TODO: make event types scoped enums
				switch (type)
				{
				case 0x791dc446: // InputStateEvent
				{
					listener._callback(listener._object, (InputStateEvent*)event);
					break;
				}
				case 0x63fa800: // InputAxisEvent
				{
					listener._callback(listener._object, (InputAxisEvent*)event);
					break;
				}
				case 0x235cd18d: // InputActionEvent
				{
					listener._callback(listener._object, (InputActionEvent*)event);
					break;
				}
				case 0x8bd7b76e: // SceneLoadEvent
				{
					listener._callback(listener._object, (SceneLoadEvent*)event);
					break;
				}
				case 0x8bd7b36e: // ScenePreloadEvent
				{
					listener._callback(listener._object, (ScenePreloadEvent*)event);
					break;
				}
				default:
					break;
				}
			}
		}
	}

	ListenerID ScriptEngine::register_script_listener(EventType type, LuaFunction lua_callback, LuaObject object)
	{
		if (_listener_map.find(type) == _listener_map.end())
			_listener_map[type] = LuaListenerArray();

		EventCallback callback;
		callback.Bind<ScriptEngine, &ScriptEngine::on_lua_script_event>(this);
		ListenerID listener_id = EventHandler::register_listener(type, callback);

		ListenerID lua_listener_id = _listener_map[type].add();
		LuaScriptListener& script_listener = _listener_map[type].lookup(lua_listener_id);
		script_listener._listener_id = listener_id;
		script_listener._callback = lua_callback;
		script_listener._object = object;
		
		return lua_listener_id;
	}

	void ScriptEngine::unregister_script_listener(EventType type, ListenerID listener)
	{
		if (_listener_map[type].has(listener))
		{
			LuaScriptListener& script_listener = _listener_map[type].lookup(listener);
			EventHandler::unregister_listener(type, script_listener._listener_id);
			_listener_map[type].remove(listener);
		}
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
