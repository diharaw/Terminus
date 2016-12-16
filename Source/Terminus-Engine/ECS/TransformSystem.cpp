#include "TransformSystem.h"
#include "TransformComponent.h"
#include "Scene.h"

namespace Terminus { namespace ECS {

	TransformSystem::TransformSystem() : ISystem()
	{
		RegisterComponentType<TransformComponent>();
	}

	TransformSystem::~TransformSystem()
	{

	}

	void TransformSystem::Initialize()
	{
        SlotMap<TransformComponent, MAX_COMPONENTS>& component_list = m_scene->GetComponentArray<TransformComponent>();
        
        for (int i = 0; i < component_list._num_objects; i++)
        {
            // Generate World Matrix
        }
	}

	void TransformSystem::Update(double delta)
	{
        
	}

	void TransformSystem::Shutdown()
	{

	}

	void TransformSystem::OnEntityCreated(Entity entity)
	{
		
	}

	void TransformSystem::OnEntityDestroyed(Entity entity)
	{

	}

} }
