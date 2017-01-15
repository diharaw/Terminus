#pragma once

#include <ECS/entity.h>
#include <ECS/component_types.h>

namespace terminus
{
	class CameraSystem
	{
	public:
		CameraSystem();
		~CameraSystem();
		virtual void Initialize();
		virtual void Update(double delta);
		virtual void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};
} // namespace terminus
