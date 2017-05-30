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

#define MAX_DEFERRED_ITEMS 1024u
#define DEFERRED_QUEUE_MASK MAX_DEFERRED_ITEMS - 1u

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;
class  PhysicsWorld;

class Scene
{
	friend class SceneManager;

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
	void update_deferred_removal_list();

public:
	struct RemovedEntity
	{
		Entity	 entity;
		uint16_t counter;
	};

	struct DeferredRemovalQueue
	{
		uint16_t count;
		uint32_t front;
		uint32_t back;
		RemovedEntity entities[MAX_ENTITIES];

		DeferredRemovalQueue()
		{
			front = 0;
			back = 0;
			count = 0;
		}

		void push_back(Entity entity)
		{
			RemovedEntity& result = entities[count];

			result.entity = entity;
			result.counter = 0;

			entities[back & DEFERRED_QUEUE_MASK] = result;
			++back;
		}

		void pop_front()
		{
			++front;
		}

		uint32_t front()
		{
			return front;
		}

		uint32_t back()
		{
			return back;
		}
	};

	DeferredRemovalQueue			  m_removal_queue;
	PackedArray<Entity, MAX_ENTITIES> m_entities;
	std::array<int, MAX_ENTITIES>	  m_versions;

	// component pools

	ComponentPool<TransformComponent, MAX_TRANSFORM_COMPONENTS>         m_transform_pool;
	ComponentPool<MeshComponent, MAX_MESH_COMPONENTS>                   m_mesh_pool;
	ComponentPool<CameraComponent, MAX_CAMERA_COMPONENTS>               m_camera_pool;
	ComponentPool<LuaScriptComponent, MAX_LUA_COMPONENTS>               m_lua_script_pool;
	ComponentPool<CppScriptComponent, MAX_CPP_COMPONENTS>               m_cpp_script_pool;
	ComponentPool<RigidBodyComponent, MAX_RIGIDBODY_COMPONENTS>         m_rigid_body_pool;
	ComponentPool<ColliderComponent, MAX_COLLIDER_COMPONENTS>           m_collider_pool;
	ComponentPool<PointLightComponent, MAX_POINT_LIGHT_COMPONENTS>	    m_point_light_pool;
	ComponentPool<DirectionalLightComponent, MAX_SPOT_LIGHT_COMPONENTS> m_directional_light_pool;
	ComponentPool<SpotLightComponent, MAX_DIRECTIONAL_LIGHT_COMPONENTS> m_spot_light_pool;
	ComponentPool<SkyComponent, MAX_SKY_COMPONENTS>                     m_sky_pool;

	// systems

	CameraSystem	m_camera_system;
	TransformSystem m_transform_system;
	RenderSystem    m_render_system;
	ScriptSystem    m_script_system;
	PhysicsSystem   m_physics_system;
	LightSystem		m_light_system;
	StringBuffer32  m_name;
	uint32_t		m_scene_id;
	PhysicsScene	m_physics_scene;
};

TERMINUS_END_NAMESPACE
