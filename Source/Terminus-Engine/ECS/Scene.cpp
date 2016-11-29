#include "Scene.h"
#include "../Types.h"

namespace Terminus { namespace ECS {

	Scene::Scene()
	{
		m_last_entity_id = 0;
	}

	Scene::~Scene()
	{		
		for (auto it : m_component_pools)
		{
			T_SAFE_DELETE(it.second);
		}
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = m_last_entity_id++;
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::Initialize()
	{
		for (auto entity : m_entities)
		{
			for (auto system : m_systems)
				system->OnEntityCreated(entity);
		}
	}

	void Scene::Update(double delta)
	{
		for (auto system : m_systems)
			system->Update(delta);
	}

	void Scene::RegisterSystem(ISystem* system)
	{
		system->SetScene(this);
		m_systems.push_back(system);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		//return 0;
	}

	IComponent* Scene::AttachComponent(Entity entity, ComponentID id)
	{
		// Check if entity already exists. Only one instance allowed for now.
		return m_component_pools[id]->AttachComponent(entity);
	}

	IComponent* Scene::GetComponent(Entity entity, ComponentID id)
	{
		// Check if entity exists;
		return m_component_pools[id]->GetComponent(entity);;
	}

	bool Scene::HasComponent(Entity entity, ComponentID id)
	{
		IComponent* component = GetComponent(entity, id);
		if (component)
			return true;
		else
			return false;
	}

	void Scene::RemoveComponent(Entity entity, ComponentID id)
	{

		m_component_pools[id]->RemoveComponent(entity);;
	}

} }
