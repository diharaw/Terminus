#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Component.h"
#include <vector>

namespace Terminus { namespace ECS {

	using Entity = uint32_t;

	class World;

	class ISystem
	{
	public:
		ISystem();
		virtual ~ISystem();
		virtual void Initialize() = 0;
		virtual void Update(double delta) = 0;
		virtual void Shutdown() = 0;
		void SetWorld(World* world);
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
		void OnComponentAttached(Entity entity, ComponentID id);
		void OnComponentRemoved(Entity entity, ComponentID id);

	protected:
		World* m_world;

		template <typename T>
        void RegisterComponentType();
	};

} }

#endif
