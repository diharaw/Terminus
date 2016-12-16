#pragma once

#ifndef  SCENE_H
#define SCENE_H

#include "System.h"
#include "ComponentPool.h"
#include <vector>
#include <iostream>

namespace Terminus { namespace ECS {

	using SystemList				   = std::vector<ISystem*>;
	using EntityList					   = std::vector<Entity>;
	using ComponentPoolMap = std::unordered_map<ComponentID, IComponentPool*>;

	class Scene
	{
	private:
		Entity							 m_last_entity_id;
		SystemList					 m_systems;
		EntityList						 m_entities;
		ComponentPoolMap m_component_pools;

	public:
		Scene();
		~Scene();
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void Initialize();
		void Update(double delta);
		IComponent* AttachComponent(Entity entity, ComponentID id);
		IComponent* GetComponent(Entity entity, ComponentID id);
		bool HasComponent(Entity entity, ComponentID id);
		void RemoveComponent(Entity entity, ComponentID id);
      	void RegisterSystem(ISystem* system);

		template<typename T>
		void RegisterComponentPool()
		{
			m_component_pools[T::_id] = new ComponentPool<T>();
		}

		template<typename T>
		SlotMap<T, MAX_COMPONENTS>& GetComponentArray()
		{
			assert(m_component_pools.find(T::_id) != m_component_pools.end());
			ComponentPool<T>* pool = (ComponentPool<T>*)m_component_pools[T::_id];
			return pool->m_components;
		} 

	};
    
    template <typename T>
    void ISystem::RegisterComponentType()
    {
        if(m_scene)
            m_scene->RegisterComponentPool<T>();
    }
    
} }

#endif
