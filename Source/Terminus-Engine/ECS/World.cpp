#include "World.h"
#include "../Types.h"

namespace Terminus { namespace ECS {

	World::World()
	{
		m_last_entity_id = 0;
	}

	World::~World()
	{		
		for (auto it : m_component_pools)
		{
			T_SAFE_DELETE(it.second);
		}
	}

	Entity World::CreateEntity()
	{
		Entity entity = m_last_entity_id++;
		m_entities.push_back(entity);
		return entity;
	}

	void World::Initialize()
	{
		for (auto entity : m_entities)
		{
			for (auto system : m_systems)
				system->OnEntityCreated(entity);
		}
	}

	void World::Update(double delta)
	{
		for (auto system : m_systems)
			system->Update(delta);
	}

	void World::RegisterSystem(ISystem* system)
	{
		system->SetWorld(this);
		m_systems.push_back(system);
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

	bool World::HasComponent(Entity entity, ComponentID id)
	{
		IComponent* component = GetComponent(entity, id);
		if (component)
			return true;
		else
			return false;
	}

	void World::RemoveComponent(Entity entity, ComponentID id)
	{

		m_component_pools[id]->RemoveComponent(entity);;
	}

} }
