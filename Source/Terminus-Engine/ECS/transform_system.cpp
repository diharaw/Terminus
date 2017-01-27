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

	void TransformSystem::initialize(Scene* scene)
	{
        _scene = scene;
        _component_array = _scene->_transform_pool.get_array();
        
        int num_component = _scene->_transform_pool.get_num_objects();
        
        for (int i = 0; i < num_component; i++)
        {
            // Generate World Matrix
            TransformComponent& cmp = _component_array[i];
            
            cmp._forward = Vector3(0.0f, 1.0f, 0.0f);
            cmp._global_transform = Matrix4();
            cmp._global_transform = glm::translate(cmp._global_transform, cmp._position);
            //cmp._global_transform = glm::rotate(cmp._global_transform, glm);
            cmp._global_transform = glm::scale(cmp._global_transform, cmp._scale);
            
            cmp._is_dirty = false;
        }
	}

	void TransformSystem::update(double delta)
	{
        int num_component = _scene->_transform_pool.get_num_objects();
        
        for (int i = 0; i < num_component; i++)
		{
			// Generate World Matrix
            TransformComponent& cmp = _component_array[i];
            
            if(cmp._is_dirty)
            {
                cmp._global_transform = Matrix4();
                cmp._global_transform = glm::translate(cmp._global_transform, cmp._position);
                //cmp._global_transform = glm::rotate(cmp._global_transform, glm);
                cmp._global_transform = glm::scale(cmp._global_transform, cmp._scale);
            }
		}
	}

	void TransformSystem::shutdown()
	{

	}

	void TransformSystem::on_entity_created(Entity entity)
	{
		
	}

	void TransformSystem::on_entity_destroyed(Entity entity)
	{

	}
} // namespace terminus
