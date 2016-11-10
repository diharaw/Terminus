#include "World.h"

namespace Terminus { namespace ECS {

	World::World()
	{
		m_last_entity_id = 0;
	}

	World::~World()
	{
		for (auto it : m_systems)
			delete it;

		for (auto it : m_component_pools)
			delete it.second;
	}

	Entity World::CreateEntity()
	{
		return m_last_entity_id++;
	}

	void World::DestroyEntity(Entity entity)
	{
		//return 0;
	}

	IComponent* World::AttachComponent(Entity entity, ComponentID id)
	{
		// Check if entity already exists. Only one instance allowed for now.
		return m_component_pools[id]->AttachComponent(entity);
	}

	IComponent* World::GetComponent(Entity entity, ComponentID id)
	{
		// Check if entity exists;
		return m_component_pools[id]->GetComponent(entity);;
	}

	void World::RemoveComponent(Entity entity, ComponentID id)
	{

		m_component_pools[id]->RemoveComponent(entity);;
	}

} }
