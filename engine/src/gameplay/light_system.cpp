#include <gameplay/light_system.h>
#include <gameplay/scene.h>

namespace terminus
{
	LightSystem::LightSystem()
	{
        _num_point_lights = 0;
        _num_spot_lights = 0;
	}

	LightSystem::~LightSystem()
	{

	}
    
	void LightSystem::initialize(Scene* scene)
	{
		_scene = scene;
        Entity* entities = _scene->get_entity_array();
        
        for(int i = 0; i < _scene->get_num_entities(); i++)
        {
            if(_scene->has_transform_component(entities[i]))
            {
                if(_scene->has_point_light_component(entities[i]))
                    _point_light_list[_num_point_lights++] = entities[i];
                else if(_scene->has_spot_light_component(entities[i]))
                    _spot_light_list[_num_spot_lights++] = entities[i];
            }
        }
	}

	void LightSystem::update(double delta)
	{
        for(int i = 0; i < _num_point_lights; i++)
        {
            TransformComponent& cmp = _scene->get_transform_component(_point_light_list[i]);
            
            if(cmp._is_dirty)
            {
                PointLightComponent& light = _scene->get_point_light_component(_point_light_list[i]);
                light.position = cmp._position;
            }
        }
        
        for(int i = 0; i < _num_spot_lights; i++)
        {
            TransformComponent& cmp = _scene->get_transform_component(_spot_light_list[i]);
            
            if(cmp._is_dirty)
            {
                SpotLightComponent& light = _scene->get_spot_light_component(_spot_light_list[i]);
                light.position = cmp._position;
            }
        }
    }

	void LightSystem::shutdown()
	{

	}

	void LightSystem::on_entity_created(Entity entity)
	{

	}

	void LightSystem::on_entity_destroyed(Entity entity)
	{

	}
}
