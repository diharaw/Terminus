#include "System.h"

namespace terminus
{
	ISystem::ISystem()
	{
		
	}

	ISystem::~ISystem()
	{

	}

	void ISystem::SetScene(Scene* scene)
	{
		m_scene = scene;
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
} // namespace terminus
