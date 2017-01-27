#include <ECS/camera_system.h>
#include <ECS/scene.h>

namespace terminus
{
	CameraSystem::CameraSystem()
	{
        _num_entities = 0;
        
        for(int i = 0; i < 16; i++)
            _entities[i]._id = INVALID_ID;
	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::initialize(Scene* scene)
	{
        _scene = scene;
        Entity* entity_array = _scene->get_entity_array();
        
        for (int i = 0; i < _scene->get_num_entities(); i++)
        {
            Entity& entity = entity_array[i];
            on_entity_created(entity);
        }
	}

	void CameraSystem::update(double delta)
	{
        for(int i = 0; i < 16; i++)
        {
            if(_entities[i]._id != INVALID_ID)
            {
                TransformComponent& trn_cmp = _scene->get_transform_component(_entities[i]);
                CameraComponent& cmr_cmp = _scene->get_camera_component(_entities[i]);
                
                cmr_cmp.camera.SetPosition(trn_cmp._position);
                cmr_cmp.camera.Update();
            }
        }
	}

	void CameraSystem::shutdown()
	{

	}

	void CameraSystem::on_entity_created(Entity entity)
	{
        if(_scene->has_camera_component(entity) && _scene->has_transform_component(entity))
            _entities[_num_entities++] = entity;
	}

	void CameraSystem::on_entity_destroyed(Entity entity)
	{
        if(_scene->has_camera_component(entity) && _scene->has_transform_component(entity))
        {
            for (int i = 0; i < 16; i++)
            {
                if(_entities[i]._id == entity._id)
                    _entities[i]._id = INVALID_ID;
            }
        }
	}
}
