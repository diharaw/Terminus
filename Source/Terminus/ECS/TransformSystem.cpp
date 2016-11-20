#include "TransformSystem.h"
#include "World.h"

namespace Terminus { namespace ECS {

	TransformSystem::TransformSystem(World* world) : ISystem(world)
	{
		RegisterComponentType<TransformComponent>();
	}

	TransformSystem::~TransformSystem()
	{

	}

	void TransformSystem::Initialize()
	{

	}

	void TransformSystem::Update(double delta)
	{

	}

	void TransformSystem::Shutdown()
	{

	}

	void TransformSystem::OnEntityCreated(Entity entity)
	{
		if (m_world->HasComponent(entity, TransformComponent::_id))
		{

		}
	}

	void TransformSystem::OnEntityDestroyed(Entity entity)
	{

	}

} }