#pragma once

#include <ECS/entity.h>

namespace terminus
{
    class Scene;
    
	class TransformSystem
	{
    private:
        Scene* _scene;
        
	public:
		TransformSystem();
		~TransformSystem();
		void Initialize(Scene* scene);
		void Update(double delta);
		void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};
} // namespace terminus


