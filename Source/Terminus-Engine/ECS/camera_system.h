#pragma once

#include <ECS/system.h>

namespace terminus
{
	class CameraSystem : public ISystem
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
