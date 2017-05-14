#pragma once

#include <gameplay/entity.h>
#include <container/packed_array.h>
#include <gameplay/entity_data_map.h>

namespace terminus
{
	struct PhysicsScene;
	struct FramePacket;
	class  Scene;
	class  PhysicsEngine;
	
	struct PhysicsEntity
	{
		ID rigid_body;
		ID transform;
	};

	class PhysicsSystem
	{
		friend class PhysicsEngine;

	public:
		PhysicsSystem();
		~PhysicsSystem();
		void initialize(Scene* scene);
		void simulate(double dt);
		void shutdown();
		void on_entity_created(Entity entity);
		void on_entity_destroyed(Entity entity);

	private:
		EntityDataMap<PhysicsEntity, MAX_ENTITIES> m_physics_entities;
		PhysicsScene*							   m_physics_scene;
		Scene*									   m_scene;
		Vector3									   m_gravity;
		bool									   m_active;
	};
}