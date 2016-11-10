#include "System.h"

namespace Terminus { namespace ECS {

	ISystem::ISystem(World* world)
	{
		m_world = world;
	}

	ISystem::~ISystem()
	{

	}

	void ISystem::OnEntityCreated(Entity entity)
	{

	}

	void ISystem::OnEntityDestroyed(Entity entity)
	{

	}

	void ISystem::OnComponentAttached(Entity entity, ComponentID id)
	{

	}

	void ISystem::OnComponentRemoved(Entity entity, ComponentID id)
	{

	}

} }
