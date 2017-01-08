#pragma once

#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include <ECS/system.h>
#include <container/packed_array.h>
#include <unordered_map>

namespace terminus
{
#define MAX_COMPONENTS 1024

	using EntityMap = std::unordered_map<Entity, ID>;

	class IComponentPool
	{
	protected:
		EntityMap m_entity_map;

	public:
		IComponentPool() {}
		virtual ~IComponentPool() {}
		virtual IComponent* AttachComponent(Entity entity) = 0;
		virtual IComponent* GetComponent(Entity entity) = 0;
		virtual void RemoveComponent(Entity entity) = 0;
	};

	template <typename T>
	class ComponentPool : public IComponentPool
	{
    public:
		SlotMap<T, MAX_COMPONENTS> m_components;

	public:
		ComponentPool() {}
		~ComponentPool() {}

		inline IComponent* AttachComponent(Entity entity)
		{
			ID component_id = m_components.add();
			IComponent* component = &m_components.lookup(component_id);
			m_entity_map[entity] = component_id;

			return component;
		}

		inline IComponent* GetComponent(Entity entity)
		{
			if (m_entity_map.find(entity) == m_entity_map.end())
				return nullptr;
			else
				return &m_components.lookup(m_entity_map[entity]);
		}

		inline void RemoveComponent(Entity entity)
		{
			m_components.remove(m_entity_map[entity]);
		}
	};

}

#endif
