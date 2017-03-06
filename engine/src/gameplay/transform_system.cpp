#include <gameplay/transform_system.h>
#include <gameplay/component_types.h>
#include <gameplay/scene.h>

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
            
            Matrix4 translation = glm::translate(cmp._position);
            Matrix4 rotation = glm::toMat4(cmp._rotation);
            Matrix4 scale = glm::scale(cmp._scale);
            
            cmp._global_transform = translation * rotation * scale;
            
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
                Matrix4 translation = glm::translate(cmp._position);
                Matrix4 rotation = glm::toMat4(cmp._rotation);
                Matrix4 scale = glm::scale(cmp._scale);
                
                cmp._global_transform = translation * rotation * scale;

                cmp._is_dirty = false;
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
