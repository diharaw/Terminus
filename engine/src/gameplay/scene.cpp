#include <gameplay/scene.h>
#include <core/frame_packet.h>
#include <core/context.h>

namespace terminus
{
	Scene::Scene()
	{
		for (int& version : _versions)
		{
			version = 0;
		}
	}

	Scene::~Scene()
	{

	}

	void Scene::initialize()
	{
		_camera_system.initialize(this);
		_transform_system.initialize(this);
		_script_system.initialize(this);
		_physics_system.initialize(this);
		_render_system.initialize(this);
	}

	void Scene::shutdown()
	{
		_script_system.shutdown();
		_camera_system.shutdown();
		_transform_system.shutdown();
		_physics_system.shutdown();
		_render_system.shutdown();

		if (_physics_scene.active())
			context::get_physics_engine().destroy_scene(_physics_scene);
	}

	void Scene::simulate(FramePacket* pkt, double dt)
	{


		if (_physics_scene.active())
			_physics_scene.simulate(dt);

		_script_system.simulate(dt);
		_transform_system.simulate(dt);
		_camera_system.simulate(dt);
		_physics_system.simulate(dt);
		

		_render_system.simulate(pkt, dt);
	}

	PhysicsScene& Scene::physics_scene()
	{
		return _physics_scene;
	}

	Entity* Scene::get_entity_array()
	{
		return &_entities._objects[0];
	}

	uint32_t Scene::get_num_entities()
	{
		return _entities._num_objects;
	}

	// attach methods

	TransformComponent& Scene::attach_transform_component(Entity& entity)
	{
		return _transform_pool.create(entity);
	}

	MeshComponent& Scene::attach_mesh_component(Entity& entity)
	{
		return _mesh_pool.create(entity);
	}

	ColliderComponent& Scene::attach_collider_component(Entity& entity)
	{
		return _collider_pool.create(entity);
	}

	CameraComponent& Scene::attach_camera_component(Entity& entity)
	{
		return _camera_pool.create(entity);
	}

	LuaScriptComponent& Scene::attach_lua_script_component(Entity& entity)
	{
		return _lua_script_pool.create(entity);
	}

	CppScriptComponent& Scene::attach_cpp_script_component(Entity& entity)
	{
		return _cpp_script_pool.create(entity);
	}

	RigidBodyComponent& Scene::attach_rigid_body_component(Entity& entity)
	{
		return _rigid_body_pool.create(entity);
	}

	PointLightComponent& Scene::attach_point_light_component(Entity& entity)
	{
		return _point_light_pool.create(entity);
	}

	SpotLightComponent& Scene::attach_spot_light_component(Entity& entity)
	{
		return _spot_light_pool.create(entity);
	}

	DirectionalLightComponent& Scene::attach_directional_light_component(Entity& entity)
	{
		return _directional_light_pool.create(entity);
	}

	SkyComponent& Scene::attach_sky_component(Entity& entity)
	{
		return _sky_pool.create(entity);
	}

	// get id methods

	ID Scene::get_transform_id(Entity& entity)
	{
		return _transform_pool.get_id(entity);
	}

	ID Scene::get_rigid_body_id(Entity& entity)
	{
		return _rigid_body_pool.get_id(entity);
	}

	ID Scene::get_collision_shape_id(Entity& entity)
	{
		return _collider_pool.get_id(entity);
	}

	// get methods

	TransformComponent& Scene::get_transform_component(Entity& entity)
	{
		return _transform_pool.lookup(entity);
	}

	MeshComponent& Scene::get_mesh_component(Entity& entity)
	{
		return _mesh_pool.lookup(entity);
	}

	ColliderComponent& Scene::get_collider_component(Entity& entity)
	{
		return _collider_pool.lookup(entity);
	}

	CameraComponent& Scene::get_camera_component(Entity& entity)
	{
		return _camera_pool.lookup(entity);
	}

	LuaScriptComponent& Scene::get_lua_script_component(Entity& entity)
	{
		return _lua_script_pool.lookup(entity);
	}

	CppScriptComponent& Scene::get_cpp_script_component(Entity& entity)
	{
		return _cpp_script_pool.lookup(entity);
	}

	RigidBodyComponent& Scene::get_rigid_body_component(Entity& entity)
	{
		return _rigid_body_pool.lookup(entity);
	}

	PointLightComponent& Scene::get_point_light_component(Entity& entity)
	{
		return _point_light_pool.lookup(entity);
	}

	SpotLightComponent& Scene::get_spot_light_component(Entity& entity)
	{
		return _spot_light_pool.lookup(entity);
	}

	DirectionalLightComponent& Scene::get_directional_light_component(Entity& entity)
	{
		return _directional_light_pool.lookup(entity);
	}

	SkyComponent& Scene::get_sky_component(Entity& entity)
	{
		return _sky_pool.lookup(entity);
	}

	// has methods

	bool Scene::has_transform_component(Entity& entity)
	{
		return _transform_pool.has(entity);
	}

	bool Scene::has_mesh_component(Entity& entity)
	{
		return _mesh_pool.has(entity);
	}

	bool Scene::has_camera_component(Entity& entity)
	{
		return _camera_pool.has(entity);
	}

	bool Scene::has_lua_script_component(Entity& entity)
	{
		return _lua_script_pool.has(entity);
	}

	bool Scene::has_cpp_script_component(Entity& entity)
	{
		return _cpp_script_pool.has(entity);
	}

	bool Scene::has_collider_component(Entity& entity)
	{
		return _collider_pool.has(entity);
	}

	bool Scene::has_rigid_body_component(Entity& entity)
	{
		return _rigid_body_pool.has(entity);
	}

	bool Scene::has_point_light_component(Entity& entity)
	{
		return _point_light_pool.has(entity);
	}

	bool Scene::has_spot_light_component(Entity& entity)
	{
		return _spot_light_pool.has(entity);
	}

	bool Scene::has_directional_light_component(Entity& entity)
	{
		return _directional_light_pool.has(entity);
	}

	bool Scene::has_sky_component(Entity& entity)
	{
		return _sky_pool.has(entity);
	}

	Entity& Scene::create_entity(std::string name = "")
	{
		assert(_entities._num_objects != MAX_ENTITIES);

		ID id = _entities.add();
		Entity& entity = _entities.lookup(id);
		entity._name = name;
		entity._id = id;
		entity._version = _versions[INDEX_FROM_ID(id)]++;

		return entity;
	}

	Entity& Scene::create_entity_from_prefab(std::string prefab)
	{
		Entity& entity = create_entity();
		return entity;
	}

	void Scene::destroy_entity(Entity& entity)
	{
		_removal_queue.add(entity);

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
			_sky_pool.remove(entity);

			_versions[INDEX_FROM_ID(entity._id)]++;
			_entities.remove(entity._id);
		}
	}

	bool Scene::is_alive(Entity& entity)
	{
		return (entity._version == _versions[INDEX_FROM_ID(entity._id)] - 1);
	}

	void Scene::set_name(const char* name)
	{
		_name = name;
	}
}