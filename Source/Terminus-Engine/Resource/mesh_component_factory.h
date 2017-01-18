#pragma once

#include <types.h>
#include <ECS/entity.h>
#include <ECS/component_types.h>
#include <Core/context.h>

namespace terminus
{
	namespace mesh_component_factory
	{
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            MeshComponent& component = scene->attach_mesh_component(entity);
            component.mesh = context::get_mesh_cache().load(std::string(json["mesh"].GetString()));
            
            // TODO: Material Overrides
		}
	}
}
