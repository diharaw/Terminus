#pragma once

#include <types.h>
#include <ECS/entity.h>
#include <ECS/component_types.h>
#include <Core/context.h>

namespace terminus
{
	namespace camera_component_factory
	{
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            CameraComponent& component = scene->attach_camera_component(entity);
            
            rapidjson::Value& projection_info = json["projection_info"];
            
            {
                component.camera.SetFarPlane(projection_info["far_plane"].GetFloat());
                component.camera.SetNearPlane(projection_info["near_plane"].GetFloat());
                
                String proj_type = String(projection_info["projection_type"].GetString());
                
                if(proj_type == "PERSPECTIVE")
                    component.camera.SetProjectionType(ProjectionType::PERSPECTIVE);
                else
                    component.camera.SetProjectionType(ProjectionType::ORTHOGRAPHIC);
                
                component.camera.SetFoV(projection_info["field_of_view"].GetFloat());
                
                float aspect_x = projection_info["aspect_ratio_x"].GetFloat();
                float aspect_y = projection_info["aspect_ratio_y"].GetFloat();
                
                if(aspect_x == 0 && aspect_y == 0)
                {
                    aspect_x = context::get_platform().get_width();
                    aspect_y = context::get_platform().get_height();
                }
                
                component.camera.SetAspectRatio(aspect_x / aspect_y);
            }
            
            component.is_active = json["is_active"].GetBool();
            component.is_offscreen = json["is_offscreen"].GetBool();
            
            rapidjson::Value& viewport_rect = json["viewport_rect"];
            
            {
                component.screen_rect.x = viewport_rect["x"].GetFloat();
                component.screen_rect.y = viewport_rect["y"].GetFloat();
                component.screen_rect.z = viewport_rect["w"].GetFloat();
                component.screen_rect.w = viewport_rect["h"].GetFloat();
            }
            
            RenderingPathCache& rp_cache = context::get_rendering_path_cache();
            component.rendering_path = rp_cache.load(String(json["rendering_path"].GetString()));
		}
	}
}
