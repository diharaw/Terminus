#pragma once

#include <container/hash_map.h>
#include <gameplay/component_pool.h>
#include <gameplay/component_types.h>
#include <gameplay/entity.h>
#include <gameplay/transform_system.h>
#include <gameplay/render_system.h>
#include <gameplay/camera_system.h>
#include <gameplay/script_system.h>
#include <gameplay/physics_system.h>
#include <gameplay/light_system.h>
#include <gameplay/render_world.h>
#include <physics/physics_types.h>

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
#define MAX_SKY_COMPONENTS 1

namespace terminus
{
	struct FramePacket;
	class  PhysicsWorld;

	class Scene
	{
		friend class WorldManager;

	public:
		Scene();
		~Scene();
		void initialize();
		void simulate(FramePacket* pkt, double dt);
		void shutdown();
		Entity* get_entity_array();
		uint32_t get_num_entities();
		PhysicsScene& physics_scene();
		TransformComponent& attach_transform_component(Entity& entity);
		MeshComponent& attach_mesh_component(Entity& entity);
		ColliderComponent& attach_collider_component(Entity& entity);
		CameraComponent& attach_camera_component(Entity& entity);
		LuaScriptComponent& attach_lua_script_component(Entity& entity);
		CppScriptComponent& attach_cpp_script_component(Entity& entity);
		RigidBodyComponent& attach_rigid_body_component(Entity& entity);
		PointLightComponent& attach_point_light_component(Entity& entity);
		SpotLightComponent& attach_spot_light_component(Entity& entity);
		DirectionalLightComponent& attach_directional_light_component(Entity& entity);
		SkyComponent& attach_sky_component(Entity& entity);
		ID get_transform_id(Entity& entity);
		ID get_rigid_body_id(Entity& entity);
		ID get_collision_shape_id(Entity& entity);
		TransformComponent& get_transform_component(Entity& entity);
		MeshComponent& get_mesh_component(Entity& entity);
		ColliderComponent& get_collider_component(Entity& entity);
		CameraComponent& get_camera_component(Entity& entity);
		LuaScriptComponent& get_lua_script_component(Entity& entity);
		CppScriptComponent& get_cpp_script_component(Entity& entity);
		RigidBodyComponent& get_rigid_body_component(Entity& entity);
		PointLightComponent& get_point_light_component(Entity& entity);
		SpotLightComponent& get_spot_light_component(Entity& entity);
		DirectionalLightComponent& get_directional_light_component(Entity& entity);
		SkyComponent& get_sky_component(Entity& entity);
		bool has_transform_component(Entity& entity);
		bool has_mesh_component(Entity& entity);
		bool has_camera_component(Entity& entity);
		bool has_lua_script_component(Entity& entity);
		bool has_cpp_script_component(Entity& entity);
		bool has_collider_component(Entity& entity);
		bool has_rigid_body_component(Entity& entity);
		bool has_point_light_component(Entity& entity);
		bool has_spot_light_component(Entity& entity);
		bool has_directional_light_component(Entity& entity);
		bool has_sky_component(Entity& entity);
		Entity& create_entity(std::string name = "");
		Entity& create_entity_from_prefab(std::string prefab);
		void destroy_entity(Entity& entity);
		bool is_alive(Entity& entity);
		void set_name(const char* name);

	public:
		PackedArray<Entity, MAX_ENTITIES> _entities;
		std::array<int, MAX_ENTITIES>	  _versions;

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
		ComponentPool<SkyComponent, MAX_SKY_COMPONENTS>                     _sky_pool;

		// systems

		CameraSystem	_camera_system;
		TransformSystem _transform_system;
		RenderSystem    _render_system;
		ScriptSystem    _script_system;
		PhysicsSystem   _physics_system;
		LightSystem		_light_system;
		StringBuffer32  _name;
		PhysicsScene	_physics_scene;
	};
}
