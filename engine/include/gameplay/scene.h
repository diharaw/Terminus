#pragma once

#include <gameplay/component_types.h>
#include <graphics/render_device.h>
#include <core/types.h>
#include <container/packed_array.h>
#include <gameplay/component_pool.h>
#include <gameplay/entity.h>
#include <gameplay/transform_system.h>
#include <gameplay/render_system.h>
#include <gameplay/camera_system.h>
#include <gameplay/script_system.h>
#include <gameplay/physics_system.h>
#include <gameplay/light_system.h>

#include <vector>
#include <iostream>
#include <string>
#include <array>

#define MAX_TRANSFORM_COMPONENTS MAX_ENTITIES
#define MAX_MESH_COMPONENTS 1024
#define MAX_CAMERA_COMPONENTS 10
#define MAX_LUA_COMPONENTS 100
#define MAX_CPP_COMPONENTS 100
#define MAX_RIGIDBODY_COMPONENTS 1024
#define MAX_COLLIDER_COMPONENTS 1024
#define MAX_POINT_LIGHT_COMPONENTS 100
#define MAX_SPOT_LIGHT_COMPONENTS 100
#define MAX_DIRECTIONAL_LIGHT_COMPONENTS 100

namespace terminus
{
	class Scene
	{
	public:
		PackedArray<Entity, MAX_ENTITIES> _entities;

		// component pools

		ComponentPool<TransformComponent, MAX_TRANSFORM_COMPONENTS>         _transform_pool;
		ComponentPool<MeshComponent, MAX_MESH_COMPONENTS>                   _mesh_pool;
		ComponentPool<CameraComponent, MAX_CAMERA_COMPONENTS>               _camera_pool;
		ComponentPool<LuaScriptComponent, MAX_LUA_COMPONENTS>               _lua_script_pool;
        ComponentPool<CppScriptComponent, MAX_CPP_COMPONENTS>               _cpp_script_pool;
        ComponentPool<RigidBodyComponent, MAX_RIGIDBODY_COMPONENTS>         _rigid_body_pool;
        ComponentPool<ColliderComponent, MAX_COLLIDER_COMPONENTS>           _collider_pool;
		ComponentPool<PointLightComponent, MAX_POINT_LIGHT_COMPONENTS>	    _point_light_pool;
        ComponentPool<DirectionalLightComponent, MAX_SPOT_LIGHT_COMPONENTS> _directional_light_pool;
        ComponentPool<SpotLightComponent, MAX_DIRECTIONAL_LIGHT_COMPONENTS> _spot_light_pool;

		// systems

		CameraSystem	_camera_system;
		TransformSystem _transform_system;
		RenderSystem    _render_system;
        ScriptSystem    _script_system;
        PhysicsSystem   _physics_system;
		LightSystem		_light_system;

        StringBuffer32  _name;

	private:
		std::array<int, MAX_ENTITIES> _versions;

	public:
		Scene()
		{
			for (int& version : _versions)
			{
				version = 0;
			}
		}

		~Scene()
		{

		}
        
        inline void initialize()
        {
            _camera_system.initialize(this);
            _transform_system.initialize(this);
            _render_system.initialize(this);
            _script_system.initialize(this);
            _physics_system.initialize(this);
            _light_system.initialize(this);
        }
        
        inline void shutdown()
        {
            _script_system.shutdown();
            _camera_system.shutdown();
            _transform_system.shutdown();
            _render_system.shutdown();
            _physics_system.shutdown();
            _light_system.shutdown();
        }

        inline void update(double dt)
        {
            _script_system.update(dt);
            _light_system.update(dt);
            _transform_system.update(dt);
            _physics_system.update(dt);
            _camera_system.update(dt);
            _render_system.update(dt);
        }
        
        inline Entity* get_entity_array()
        {
            return &_entities._objects[0];
        }
        
        inline uint32_t get_num_entities()
        {
            return _entities._num_objects;
        }
        
		// attach methods

		inline TransformComponent& attach_transform_component(Entity& entity)
		{
			return _transform_pool.create(entity);
		}

		inline MeshComponent& attach_mesh_component(Entity& entity)
		{
			return _mesh_pool.create(entity);
		}
        
        inline ColliderComponent& attach_collider_component(Entity& entity)
        {
            return _collider_pool.create(entity);
        }

		inline CameraComponent& attach_camera_component(Entity& entity)
		{
			return _camera_pool.create(entity);
		}

		inline LuaScriptComponent& attach_lua_script_component(Entity& entity)
		{
			return _lua_script_pool.create(entity);
		}
        
        inline CppScriptComponent& attach_cpp_script_component(Entity& entity)
        {
            return _cpp_script_pool.create(entity);
        }
        
        inline RigidBodyComponent& attach_rigid_body_component(Entity& entity)
        {
            return _rigid_body_pool.create(entity);
        }

		inline PointLightComponent& attach_point_light_component(Entity& entity)
		{
			return _point_light_pool.create(entity);
		}
        
        inline SpotLightComponent& attach_spot_light_component(Entity& entity)
        {
            return _spot_light_pool.create(entity);
        }
        
        inline DirectionalLightComponent& attach_directional_light_component(Entity& entity)
        {
            return _directional_light_pool.create(entity);
        }
        
        // get id methods
        
        inline ID get_transform_id(Entity& entity)
        {
            return _transform_pool.get_id(entity);
        }
        
        inline ID get_rigid_body_id(Entity& entity)
        {
            return _rigid_body_pool.get_id(entity);
        }

		// get methods

		inline TransformComponent& get_transform_component(Entity& entity)
		{
			return _transform_pool.lookup(entity);
		}

		inline MeshComponent& get_mesh_component(Entity& entity)
		{
			return _mesh_pool.lookup(entity);
		}

		inline ColliderComponent& get_collider_component(Entity& entity)
		{
			return _collider_pool.lookup(entity);
		}

		inline CameraComponent& get_camera_component(Entity& entity)
		{
			return _camera_pool.lookup(entity);
		}

		inline LuaScriptComponent& get_lua_script_component(Entity& entity)
		{
			return _lua_script_pool.lookup(entity);
		}
        
        inline CppScriptComponent& get_cpp_script_component(Entity& entity)
        {
            return _cpp_script_pool.lookup(entity);
        }
        
        inline RigidBodyComponent& get_rigid_body_component(Entity& entity)
        {
            return _rigid_body_pool.lookup(entity);
        }

		inline PointLightComponent& get_point_light_component(Entity& entity)
		{
			return _point_light_pool.lookup(entity);
		}
        
        inline SpotLightComponent& get_spot_light_component(Entity& entity)
        {
            return _spot_light_pool.lookup(entity);
        }
        
        inline DirectionalLightComponent& get_directional_light_component(Entity& entity)
        {
            return _directional_light_pool.lookup(entity);
        }

		// has methods

		inline bool has_transform_component(Entity& entity)
		{
			return _transform_pool.has(entity);
		}

		inline bool has_mesh_component(Entity& entity)
		{
			return _mesh_pool.has(entity);
		}

		inline bool has_camera_component(Entity& entity)
		{
			return _camera_pool.has(entity);
		}

		inline bool has_lua_script_component(Entity& entity)
		{
			return _lua_script_pool.has(entity);
		}
        
        inline bool has_cpp_script_component(Entity& entity)
        {
            return _cpp_script_pool.has(entity);
        }
        
        inline bool has_collider_component(Entity& entity)
        {
            return _collider_pool.has(entity);
        }
        
        inline bool has_rigid_body_component(Entity& entity)
        {
            return _rigid_body_pool.has(entity);
        }

		inline bool has_point_light_component(Entity& entity)
		{
			return _point_light_pool.has(entity);
		}
        
        inline bool has_spot_light_component(Entity& entity)
        {
            return _spot_light_pool.has(entity);
        }
        
        inline bool has_directional_light_component(Entity& entity)
        {
            return _directional_light_pool.has(entity);
        }

		inline Entity& create_entity(std::string name = "")
		{
			assert(_entities._num_objects != MAX_ENTITIES);

			ID id = _entities.add();
			Entity& entity = _entities.lookup(id);
			entity._name = name;
			entity._id = id;
			entity._version = _versions[INDEX_FROM_ID(id)]++;

			return entity;
		}

		inline Entity& create_entity_from_prefab(std::string prefab)
		{
			Entity& entity = create_entity();
			return entity;
		}

		inline void destroy_entity(Entity& entity)
		{
			if (entity._id != INVALID_ID && is_alive(entity))
			{
				// remove all components belonging to entity
				_transform_pool.remove(entity);
				_mesh_pool.remove(entity);
                _cpp_script_pool.remove(entity);
                _lua_script_pool.remove(entity);
                _rigid_body_pool.remove(entity);
                _collider_pool.remove(entity);
				_point_light_pool.remove(entity);
                _spot_light_pool.remove(entity);
                _directional_light_pool.remove(entity);

				_versions[INDEX_FROM_ID(entity._id)]++;
				_entities.remove(entity._id);
			}
		}

		inline bool is_alive(Entity& entity)
		{
			return (entity._version == _versions[INDEX_FROM_ID(entity._id)] - 1);
		}

#if defined(TERMINUS_WITH_EDITOR)

		inline void serialize(JsonDocument& doc)
		{
			JsonValue scene(_name.c_str(), doc.GetAllocator());
			doc.AddMember("scene_name", scene, doc.GetAllocator());
			
			JsonValue entities(rapidjson::kArrayType);

			for (int i = 0; i < _entities.size(); i++)
			{
				Entity& current_entity = _entities._objects[i];

				JsonValue entity(rapidjson::kObjectType);

				JsonValue entity_name(current_entity._name.c_str(), doc.GetAllocator());
				entity.AddMember("entity_name", entity_name, doc.GetAllocator());

				// add components

				JsonValue components(rapidjson::kArrayType);

				{
					// transform
					if (has_transform_component(current_entity))
					{
						TransformComponent& cmp = get_transform_component(current_entity);
						JsonValue json_cmp = component_serialize(cmp, doc);
						components.PushBack(json_cmp, doc.GetAllocator());
					}

					// mesh
					if (has_mesh_component(current_entity))
					{
						MeshComponent& cmp = get_mesh_component(current_entity);
						JsonValue json_cmp = component_serialize(cmp, doc);
						components.PushBack(json_cmp, doc.GetAllocator());
					}

					// lua script
					if (has_lua_script_component(current_entity))
					{
						LuaScriptComponent& cmp = get_lua_script_component(current_entity);
						JsonValue json_cmp = component_serialize(cmp, doc);
						components.PushBack(json_cmp, doc.GetAllocator());
					}

					// cpp script
					if (has_cpp_script_component(current_entity))
					{
						CppScriptComponent& cmp = get_cpp_script_component(current_entity);
						JsonValue json_cmp = component_serialize(cmp, doc);
						components.PushBack(json_cmp, doc.GetAllocator());
					}

					// camera
					if (has_camera_component(current_entity))
					{
						CameraComponent& cmp = get_camera_component(current_entity);
						JsonValue json_cmp = component_serialize(cmp, doc);
						components.PushBack(json_cmp, doc.GetAllocator());
					}
				}

				entity.AddMember("components", components, doc.GetAllocator());

				entities.PushBack(entity, doc.GetAllocator());
			}

			doc.AddMember("entities", entities, doc.GetAllocator());
		}

#endif

	};
}
