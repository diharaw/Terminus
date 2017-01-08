#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include <ECS/component.h>
#include <vector>

namespace terminus
{
	using Entity = uint32_t;

	struct Scene;

	class ISystem
	{
	public:
		ISystem();
		virtual ~ISystem();
		virtual void Initialize() = 0;
		virtual void Update(double delta) = 0;
		virtual void Shutdown() = 0;
		void SetScene(Scene* scene);
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
		void OnComponentAttached(Entity entity, ComponentID id);
		void OnComponentRemoved(Entity entity, ComponentID id);

	protected:
		Scene* m_scene;
	};
}

#endif
