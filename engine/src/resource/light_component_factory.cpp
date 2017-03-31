#include <resource/light_component_factory.h>

namespace terminus
{
    void create_point_light(JsonValue& json, Scene* scene, Entity& entity)
    {
        PointLightComponent& component = scene->attach_point_light_component(entity);
        
        rapidjson::Value& color = json["color"];
        
        component.color.x = color["x"].GetFloat();
        component.color.y = color["y"].GetFloat();
        component.color.z = color["z"].GetFloat();
        
        component.constant_attenuation = json["constant_attenuation"].GetFloat();
        component.linear_attenuation = json["linear_attenuation"].GetFloat();
        component.quadratic_attenuation = json["quadratic_attenuation"].GetFloat();
    }
    
    void create_spot_light(JsonValue& json, Scene* scene, Entity& entity)
	{
        SpotLightComponent& component = scene->attach_spot_light_component(entity);
        
        rapidjson::Value& direction = json["direction"];

		component.direction.x = direction["x"].GetFloat();
		component.direction.y = direction["y"].GetFloat();
		component.direction.z = direction["z"].GetFloat();
        
        component.constant_attenuation = json["constant_attenuation"].GetFloat();
        component.linear_attenuation = json["linear_attenuation"].GetFloat();
        component.quadratic_attenuation = json["quadratic_attenuation"].GetFloat();
        
        component.inner_cutoff = json["inner_cutoff"].GetFloat();
        component.outer_cutoff = json["outer_cutoff"].GetFloat();
	}

	void create_directional_light(JsonValue& json, Scene* scene, Entity& entity)
	{
        DirectionalLightComponent& component = scene->attach_directional_light_component(entity);
        
		rapidjson::Value& direction = json["direction"];

		component.direction.x = direction["x"].GetFloat();
		component.direction.y = direction["y"].GetFloat();
		component.direction.z = direction["z"].GetFloat();
	}

	void create(JsonValue& json, Entity& entity, Scene* scene)
	{
		String light_type = String(json["light_type"].GetString());

		if (light_type == "POINT")
            create_point_light(json, scene, entity);
		else if (light_type == "SPOT")
            create_spot_light(json, scene, entity);
		else if (light_type == "DIRECTIONAL")
            create_directional_light(json, scene, entity);
			
	}
}
