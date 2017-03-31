#pragma once

#include <gameplay/entity.h>
#include <gameplay/component_types.h>

#define MAX_LIGHT_ENTITIES 100

namespace terminus
{
	class Scene;

	class LightSystem
	{
	private:
		Scene* _scene;
        int    _num_point_lights;
        int    _num_spot_lights;
        Entity _point_light_list[MAX_LIGHT_ENTITIES];
        Entity _spot_light_list[MAX_LIGHT_ENTITIES];

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
