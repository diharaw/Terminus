#include <ECS/camera_system.h>
#include <ECS/component_types.h>
#include <ECS/scene.h>

namespace terminus
{
	CameraSystem::CameraSystem()
	{

	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::Initialize(Scene* scene)
	{
        _scene = scene;
	}

	void CameraSystem::Update(double delta)
	{

	}

	void CameraSystem::Shutdown()
	{

	}

	void CameraSystem::OnEntityCreated(Entity entity)
	{

	}

	void CameraSystem::OnEntityDestroyed(Entity entity)
	{

	}
}
