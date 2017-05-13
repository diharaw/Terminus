#include <gameplay/physics_system.h>
#include <core/context.h>
#include <gameplay/scene.h>
#include <core/frame_packet.h>

namespace terminus
{
	PhysicsSystem::PhysicsSystem() : _active(false)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::initialize(Scene* scene)
	{
		_scene = scene;
		_physics_scene = &_scene->physics_scene();
	}

	void PhysicsSystem::simulate(double dt)
	{
		for (int i = 0; i < _entries.size(); i++)
		{
			TransformComponent& transform = _scene->_transform_pool._pool.lookup(_entries._objects[i].transform);
			RigidBodyComponent& rigid_body = _scene->_rigid_body_pool._pool.lookup(_entries._objects[i].rigid_body);

			transform._global_transform = rigid_body._rigid_body.get_world_transform();
			transform::set_position(transform, rigid_body._rigid_body.get_position());
			transform::set_rotation_quat(transform, rigid_body._rigid_body.get_rotation());
		}
	}

	void PhysicsSystem::shutdown()
	{

	}

	void PhysicsSystem::on_entity_created(Entity entity)
	{
		ID transform = _scene->get_transform_id(entity);
		ID rigid_body = _scene->get_rigid_body_id(entity);
		ID collision_shape = _scene->get_collision_shape_id(entity);

		if (transform != INVALID_ID && rigid_body != INVALID_ID && collision_shape != INVALID_ID)
		{
			ID entry_id = _entries.add();
			_entities[INDEX_FROM_ID(entity._id)] = entry_id;
			PhysicsEntity entry =  _entries.lookup(entry_id);
			entry.rigid_body = rigid_body;
			entry.transform = transform;

			_physics_scene->add_rigid_body(_scene->_rigid_body_pool.lookup(entity)._rigid_body);
		}
		else
		{
			_entities[INDEX_FROM_ID(entity._id)] = INVALID_ID;
		}
	}

	void PhysicsSystem::on_entity_destroyed(Entity entity)
	{
		ID entry_id = _entities[INDEX_FROM_ID(entity._id)];

		if (entry_id != INVALID_ID)
		{
			_entities[INDEX_FROM_ID(entity._id)] = INVALID_ID;

			// Find RigidBody from ComponentPool.
			RigidBody& body = _scene->_rigid_body_pool.lookup(entity)._rigid_body;

			// Find Collider from ComponentPool.
			ColliderComponent& collider = _scene->_collider_pool.lookup(entity);
			CollisionShape* shape = collider.shape();

			// Remove PhysicsEntity entry related to the current Entity.
			_entries.remove(entry_id);

			// Remove RigidBody from DynamicsWorld.
			_physics_scene->remove_rigid_body(body);

			// Destroy RigidBody.
			context::get_physics_engine().destroy_rigid_body(body);

			// Destroy CollisionShape.
			context::get_physics_engine().destroy_collision_shape(shape);

			// Finally, remove RigidBodyComponent and ColliderComponent 
			// belonging to the current Entity.
			_scene->_rigid_body_pool.remove(entity);
			_scene->_collider_pool.remove(entity);
		}
	}
}