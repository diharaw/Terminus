#pragma once

#include <gameplay/entity.h>
#include <gameplay/component_types.h>

namespace terminus
{
    class Scene;
    
	class TransformSystem
	{
    private:
        Scene* _scene;
        TransformComponent* _component_array;
        
	public:
		TransformSystem();
		~TransformSystem();
		void initialize(Scene* scene);
		void update(double delta);
		void shutdown();
		void on_entity_created(Entity entity);
		void on_entity_destroyed(Entity entity);
	};
} // namespace terminus


