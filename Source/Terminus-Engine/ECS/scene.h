#pragma once

#include <ECS/component_types.h>
#include <Graphics/render_device.h>
#include <types.h>
#include <container/packed_array.h>
#include <ECS/component_pool.h>
#include <ECS/entity.h>
#include <ECS/transform_system.h>
#include <ECS/render_system.h>
#include <ECS/camera_system.h>

#include <vector>
#include <iostream>
#include <string>
#include <array>

namespace terminus
{
	class Scene
	{
	public:
		PackedArray<Entity, MAX_ENTITIES> _entities;

		// component pools

		ComponentPool<TransformComponent> _transform_pool;
		ComponentPool<MeshComponent>	  _mesh_pool;
		ComponentPool<ColliderComponent>  _collider_pool;
		ComponentPool<CameraComponent>	  _camera_pool;
		ComponentPool<LuaScriptComponent> _lua_script_pool;
        ComponentPool<CppScriptComponent> _cpp_script_pool;

		// systems

		CameraSystem	_camera_system;
		TransformSystem _transform_system;
		RenderSystem    _render_system;
        
        String          _name;

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
        }
        
        inline void shutdown()
        {
            _camera_system.shutdown();
            _transform_system.shutdown();
            _render_system.shutdown();
        }

        inline void update(double dt)
        {
            _transform_system.update(dt);
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

		// has methods

		inline bool has_transform_component(Entity& entity)
		{
			return _transform_pool.has(entity);
		}

		inline bool has_mesh_component(Entity& entity)
		{
			return _mesh_pool.has(entity);
		}

		inline bool has_collider_component(Entity& entity)
		{
			return _collider_pool.has(entity);
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
				_collider_pool.remove(entity);

				_versions[INDEX_FROM_ID(entity._id)]++;
				_entities.remove(entity._id);
			}
		}

		inline bool is_alive(Entity& entity)
		{
			return (entity._version == _versions[INDEX_FROM_ID(entity._id)] - 1);
		}

	};
}