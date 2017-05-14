#include <gameplay/physics_system.h>
#include <core/context.h>
#include <gameplay/scene.h>
#include <core/frame_packet.h>

namespace terminus
{
	PhysicsSystem::PhysicsSystem() : m_active(false)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::initialize(Scene* scene)
	{
		m_scene = scene;
		m_physics_scene = &m_scene->physics_scene();
	}

	void PhysicsSystem::simulate(double dt)
	{
		PhysicsEntity* entities = m_physics_entities.array();

		for (int i = 0; i < m_physics_entities.size(); i++)
		{
			TransformComponent& transform = m_scene->_transform_pool._pool.lookup(entities[i].transform);
			RigidBodyComponent& rigid_body = m_scene->_rigid_body_pool._pool.lookup(entities[i].rigid_body);

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
		ID transform = m_scene->get_transform_id(entity);
		ID rigid_body = m_scene->get_rigid_body_id(entity);
		ID collision_shape = m_scene->get_collision_shape_id(entity);

		if (transform != INVALID_ID && rigid_body != INVALID_ID && collision_shape != INVALID_ID)
		{
			PhysicsEntity* physics_entity = m_physics_entities.add(entity);
			
			physics_entity->rigid_body = rigid_body;
			physics_entity->transform = transform;
		}
	}

	void PhysicsSystem::on_entity_destroyed(Entity entity)
	{
		m_physics_entities.remove(entity);
	}
}