#include <resource/light_component_factory.h>

namespace terminus
{
	void create_point_light(JsonValue& json, LightComponent& cmp)
	{
		rapidjson::Value& position = json["position"];

		cmp.position.x = position["x"].GetFloat();
		cmp.position.y = position["y"].GetFloat();
		cmp.position.z = position["z"].GetFloat();
	}

	void create_spot_light(JsonValue& json, LightComponent& cmp)
	{
		rapidjson::Value& position = json["position"];

		cmp.position.x = position["x"].GetFloat();
		cmp.position.y = position["y"].GetFloat();
		cmp.position.z = position["z"].GetFloat();

		rapidjson::Value& direction = json["direction"];

		cmp.direction.x = direction["x"].GetFloat();
		cmp.direction.y = direction["y"].GetFloat();
		cmp.direction.z = direction["z"].GetFloat();
	}

	void create_directional_light(JsonValue& json, LightComponent& cmp)
	{
		rapidjson::Value& direction = json["direction"];

		cmp.direction.x = direction["x"].GetFloat();
		cmp.direction.y = direction["y"].GetFloat();
		cmp.direction.z = direction["z"].GetFloat();
	}

	void create(JsonValue& json, Entity& entity, Scene* scene)
	{
		LightComponent& component = scene->attach_light_component(entity);
		String light_type = String(json["light_type"].GetString());

		rapidjson::Value& color = json["color"];

		component.color.x = color["x"].GetFloat();
		component.color.y = color["y"].GetFloat();
		component.color.z = color["z"].GetFloat();

		if (light_type == "POINT")
		{
			component.type = LightType::POINT;
			create_point_light(json, component);
		}
		else if (light_type == "SPOT")
		{
			component.type = LightType::SPOT;
			create_spot_light(json, component);
		}
		else if (light_type == "DIRECTIONAL")
		{
			component.type = LightType::DIRECTIONAL;
			create_directional_light(json, component);
		}
			
	}
}