#pragma once

#include <gameplay/entity.h>
#include <container/packed_array.h>

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

		PhysicsEntity()
		{

		}
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
		PackedArray<PhysicsEntity, MAX_ENTITIES> _entries;
		ID										 _entities[MAX_ENTITIES];
		PhysicsScene*							 _physics_scene;
		Scene*									 _scene;
		Vector3									 _gravity;
		bool									 _active;
	};
}