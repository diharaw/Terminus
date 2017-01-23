#pragma once

#include <ECS/entity.h>

namespace terminus
{
    class Scene;
    
	class CameraSystem
	{
    private:
        Scene* _scene;
        
	public:
		CameraSystem();
		~CameraSystem();
		virtual void Initialize(Scene* scene);
		virtual void Update(double delta);
		virtual void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};
} // namespace terminus
