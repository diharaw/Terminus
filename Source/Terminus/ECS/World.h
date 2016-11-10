#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "System.h"
#include "ComponentPool.h"

namespace Terminus { namespace ECS {

	using SystemList = std::vector<ISystem*>;
	using ComponentPoolMap = std::unordered_map<ComponentID, IComponentPool*>;

	class World
	{
	private:
		Entity m_last_entity_id;
		SystemList m_systems;
		ComponentPoolMap m_component_pools;

	public:
		World();
		~World();
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		IComponent* AttachComponent(Entity entity, ComponentID id);
		IComponent* GetComponent(Entity entity, ComponentID id);
		void RemoveComponent(Entity entity, ComponentID id);

		template<typename T>
		void RegisterSystem()
		{
			m_systems.push_back(new T(this));
		}

		template<typename T>
		void RegisterComponentPool()
		{
			m_component_pools[T::_id] = new ComponentPool<T>();
		}
	};


} }

#endif