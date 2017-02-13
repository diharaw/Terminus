#include <script/script_engine.h>
#include <Core/context.h>
#include <ECS/entity.h>
#include <ECS/scene.h>
#include <ECS/component_types.h>
#include <types.h>
#include <Math/math_utility.h>
#include <Input/input_handler.h>

namespace terminus
{
	// temp global
	LuaListenerMap _listener_map;

	void on_lua_script_event(Event* event)
	{
		EventType type = event->GetType();

		if (_listener_map.find(type) != _listener_map.end())
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

	ListenerID register_script_listener(EventType type, LuaFunction lua_callback, LuaObject object)
	{
		if (_listener_map.find(type) == _listener_map.end())
			_listener_map[type] = LuaListenerArray();

		EventCallback callback;
		callback.Bind<&on_lua_script_event>();
		ListenerID listener_id = EventHandler::register_listener(type, callback);

		ListenerID lua_listener_id = _listener_map[type].add();
		LuaScriptListener& script_listener = _listener_map[type].lookup(lua_listener_id);
		script_listener._listener_id = listener_id;
		script_listener._callback = lua_callback;
		script_listener._object = object;

		return lua_listener_id;
	}

	void unregister_script_listener(EventType type, ListenerID listener)
	{
		if (_listener_map[type].has(listener))
		{
			LuaScriptListener& script_listener = _listener_map[type].lookup(listener);
			EventHandler::unregister_listener(type, script_listener._listener_id);
			_listener_map[type].remove(listener);
		}
	}

	// platform script api
	void set_cursor_visibility(bool visible)
	{
		context::get_platform().set_cursor_visibility(visible);
	}

	void set_relative_mouse(bool relative)
	{
		context::get_platform().set_relative_mouse(relative);
	}

	void grab_mouse(bool grab)
	{
		context::get_platform().grab_mouse(grab);
	}

	double get_delta_time()
	{
		return context::get_platform().get_delta_time();
	}

	float dot_product(Vector3& a, Vector3& b)
	{
		return glm::dot(a, b);
	}

	Vector3 cross_product(Vector3& a, Vector3& b)
	{
		return glm::cross(a, b);
	}
    
    Quaternion axis_angle(float angle, Vector3 axis)
    {
        return glm::angleAxis(angle, axis);
    }
    
    HashResult string_hash(std::string text)
    {
        HashResult result;
        result.hash = HASH(text.c_str());
        return result;
    }
    
    bool hash_equals(std::string text, HashResult hash_result)
    {
        uint64_t result = HASH(text.c_str());
        return (result == hash_result.hash);
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
    
        register_misc_api();
		register_math_api();
		register_component_api();
		register_scene_api();
		register_scene_manager_api();
        register_input_api();
		register_platform_api();
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
		sol::table math_root = _lua_state.create_table("Math");

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
        
        _lua_state.new_usertype<Quaternion>("Quaternion",
                                         sol::constructors<sol::types<>, sol::types<Vector3>>(),
                                         sol::meta_function::multiplication, sol::resolve<glm::quat(const glm::quat&, const glm::quat&)>(glm::operator*),
                                         sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::quat&, const glm::vec3&)>(glm::operator*)
                                         );

		math_root.set_function("dot", &dot_product);
		math_root.set_function("cross", &cross_product);
        math_root.set_function("axis_angle", &axis_angle);
	}
    
    void ScriptEngine::register_misc_api()
    {
        LuaObject utility_root = _lua_state.create_table("Utility");
        _lua_state.new_usertype<HashResult>("Hash",
                                            sol::constructors<sol::types<>>()
                                            );
        
        utility_root.set_function("string_hash", &string_hash);
        utility_root.set_function("hash_equals", &hash_equals);
        
        _lua_state.new_usertype<StringBuffer32>("StringBuffer32",
                                            sol::constructors<sol::types<>, sol::types<std::string>>()
                                            );
    }

	void ScriptEngine::register_event_api()
	{
		sol::table event_root = _lua_state.create_table("event_handler");

		event_root.set_function("register_listener", &register_script_listener);
		event_root.set_function("unregister_listener", &unregister_script_listener);
		event_root.set_function("queue_event", &EventHandler::queue_event);
		event_root.set_function("fire_event", &EventHandler::fire_event);

		_lua_state.new_usertype<InputActionEvent>("InputActionEvent",
                                                  sol::constructors<sol::types<HashResult>>(),
                                                  "get_action_hash", &InputActionEvent::get_action_hash
                                                  );

		_lua_state.new_usertype<InputStateEvent>("InputStateEvent",
                                                 sol::constructors<sol::types<HashResult, int>>(),
												 "value", &InputStateEvent::m_value,
                                                 "get_state_hash", &InputStateEvent::get_state_hash
                                                 );

		_lua_state.new_usertype<InputAxisEvent>("InputAxisEvent",
                                                sol::constructors<sol::types<HashResult, double, double>>(),
												"value", &InputAxisEvent::m_value,
												"delta", &InputAxisEvent::m_delta,
                                                "get_axis_hash", &InputAxisEvent::get_axis_hash
                                                );
        
        sol::table event_types = _lua_state.create_table("EventType");
        
        event_types.set("INPUT_ACTION", InputActionEvent::sk_Type);
        event_types.set("INPUT_AXIS", InputAxisEvent::sk_Type);
        event_types.set("INPUT_STATE", InputStateEvent::sk_Type);
        event_types.set("SCENE_LOAD", SceneLoadEvent::sk_Type);
        event_types.set("SCENE_PRELOAD", ScenePreloadEvent::sk_Type);
	}

	void ScriptEngine::register_component_api()
	{
		_lua_state.new_usertype<TransformComponent>("TransformComponent",
                                                    sol::constructors<sol::types<>>(),
                                                    "_global_transform", &TransformComponent::_global_transform,
                                                    "_position", &TransformComponent::_position,
                                                    "_scale", &TransformComponent::_scale,
                                                    "_rotation", &TransformComponent::_rotation,
                                                    "_euler_angles", &TransformComponent::_euler_angles,
                                                    "_forward", &TransformComponent::_forward,
                                                    "_is_dirty", &TransformComponent::_is_dirty,
                                                    "_parent_entity_name", &TransformComponent::_parent_entity_name);

//		_lua_state.new_usertype<CameraComponent>("CameraComponent",
//                                                 sol::constructors<sol::types<>>(),
//                                                 "offset", &CameraComponent::offset,
//                                                 "screen_rect", &CameraComponent::screen_rect);

        _lua_state.new_usertype<CameraComponent>("CameraComponent",
                                                 sol::constructors<sol::types<>>(),
                                                 "transform", &CameraComponent::transform,
                                                 "screen_rect", &CameraComponent::screen_rect,
                                                 "projection_matrix", &CameraComponent::projection_matrix,
                                                 "near_plane", &CameraComponent::near_plane,
                                                 "far_plane", &CameraComponent::far_plane,
                                                 "is_active", &CameraComponent::is_active,
                                                 "inherit_pitch", &CameraComponent::inherit_pitch,
                                                 "inherit_yaw", &CameraComponent::inherit_yaw,
                                                 "inherit_roll", &CameraComponent::inherit_roll);

        LuaObject transform_root = _lua_state.create_table("Transform");
        
		transform_root.set_function("set_position", &transform::set_position);
		transform_root.set_function("offset_position", &transform::offset_position);
        transform_root.set_function("set_scale", &transform::set_scale);
		transform_root.set_function("set_rotation_euler", &transform::set_rotation_euler);
        transform_root.set_function("set_rotation_quat", &transform::set_rotation_quat);
		transform_root.set_function("look_at", &transform::look_at);
        
        LuaObject camera_root = _lua_state.create_table("Camera");
        
        camera_root.set_function("set_position", &camera::set_position);
        camera_root.set_function("set_yaw", &camera::set_yaw);
        camera_root.set_function("set_pitch", &camera::set_pitch);
        camera_root.set_function("set_roll", &camera::set_roll);
        camera_root.set_function("offset_position", &camera::offset_position);
        camera_root.set_function("offset_euler", &camera::offset_euler);
        camera_root.set_function("offset_yaw", &camera::offset_yaw);
        camera_root.set_function("offset_pitch", &camera::offset_pitch);
        camera_root.set_function("offset_roll", &camera::offset_roll);
	}
    
    void ScriptEngine::register_input_api()
    {
        _lua_state.new_usertype<PlayerContextHandle>("PlayerContextHandle",
                                                     sol::constructors<sol::types<>>()
                                                     );
        
        _lua_state.new_usertype<InputMapHandle>("InputMapHandle",
                                                 sol::constructors<sol::types<>>()
                                                );
        
        LuaObject input_root = _lua_state.create_table("Input");
        
        input_root.set_function("create_player", input_handler::create_player);
        input_root.set_function("get_default_player_context", input_handler::get_default_player_context);
        input_root.set_function("load_input_map", input_handler::load_input_map);
        input_root.set_function("set_active_input_map", input_handler::set_active_input_map);
    }

	void ScriptEngine::register_platform_api()
	{
		LuaObject platform_root = _lua_state.create_table("Platform");

		platform_root.set_function("set_cursor_visibility", &set_cursor_visibility);
		platform_root.set_function("set_relative_mouse", &set_relative_mouse);
		platform_root.set_function("grab_mouse", &grab_mouse);
		platform_root.set_function("get_delta_time", &get_delta_time);
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
                                       "is_entity_alive", &Scene::is_alive,
                                       "_name", &Scene::_name);

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
		lua_script->_script_name = file_name;
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
		cpp_script->setup_environment(context::get_script_interface_impl_ptr());

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
