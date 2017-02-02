#pragma once

#include <ECS/entity.h>
#include <ECS/component_types.h>

namespace terminus
{
    class Scene;
    
	class CameraSystem
	{
    private:
        Scene*   _scene;
        Entity   _entities[16];
        uint32_t _num_entities;
        
	public:
		CameraSystem();
		~CameraSystem();
		void initialize(Scene* scene);
		void update(double delta);
		void shutdown();
		void on_entity_created(Entity entity);
		void on_entity_destroyed(Entity entity);
	};
} // namespace terminus
