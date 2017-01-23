#include <ECS/transform_system.h>
#include <ECS/component_types.h>
#include <ECS/scene.h>

namespace terminus
{
	TransformSystem::TransformSystem()
	{
		
	}

	TransformSystem::~TransformSystem()
	{

	}

	void TransformSystem::Initialize(Scene* scene)
	{
        _scene = scene;
        //SlotMap<TransformComponent, MAX_COMPONENTS>& component_list = m_scene->GetComponentArray<TransformComponent>();
        
        //for (int i = 0; i < component_list._num_objects; i++)
        {
            // Generate World Matrix
        }
	}

	void TransformSystem::Update(double delta)
	{
		//SlotMap<TransformComponent, MAX_COMPONENTS>& component_list = m_scene->GetComponentArray<TransformComponent>();

		//for (int i = 0; i < component_list._num_objects; i++)
		{
			// Generate World Matrix

		}
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
} // namespace terminus
