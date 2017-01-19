#pragma once

#include <ECS/entity.h>

namespace terminus
{
	class TransformSystem
	{
	public:
		TransformSystem();
		~TransformSystem();
		void Initialize();
		void Update(double delta);
		void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};
} // namespace terminus


