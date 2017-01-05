#include "Scene.h"
#include "TransformComponent.h"
#include "../Types.h"

namespace terminus
{
	Scene::Scene()
	{
		m_last_entity_id = 0;
        
        // Register Component Pools
        RegisterComponentPool<TransformComponent>(&m_transform_component_pool);
        RegisterComponentPool<CameraComponent>(&m_camera_component_pool);
        RegisterComponentPool<MeshComponent>(&m_mesh_component_pool);
        
        // Register Systems
        RegisterSystem(&m_transform_system);
        RegisterSystem(&m_render_system);
	}

	Scene::~Scene()
	{		

	}

	Entity Scene::CreateEntity(String name)
	{
		Entity entity = m_last_entity_id++;
		m_entities.push_back(entity);
        m_entity_id_name_map[entity] = name;
        m_entity_name_id_map[name] = entity;
		return entity;
	}
    
    Entity Scene::GetEntityByName(String name)
    {
        if (m_entity_name_id_map.find(name) == m_entity_name_id_map.end())
            return 0;
        else
            return m_entity_name_id_map[name];
    }
    
    String Scene::GetNameByEntity(Entity entity)
    {
        if (m_entity_id_name_map.find(entity) == m_entity_id_name_map.end())
            return 0;
        else
            return m_entity_id_name_map[entity];
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
} // namespace terminus
