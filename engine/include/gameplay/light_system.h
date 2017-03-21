#pragma once

#include <gameplay/entity.h>
#include <gameplay/component_types.h>

namespace terminus
{
	class Scene;

	class LightSystem
	{
	private:
		Scene*   _scene;

	public:
		LightSystem();
		~LightSystem();
		void initialize(Scene* scene);
		void update(double delta);
		void shutdown();
		void on_entity_created(Entity entity);
		void on_entity_destroyed(Entity entity);
	};
}