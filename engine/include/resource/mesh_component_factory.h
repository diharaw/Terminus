#pragma once

#include <core/types.h>
#include <gameplay/entity.h>
#include <gameplay/scene.h>
#include <gameplay/component_types.h>
#include <core/context.h>

namespace terminus
{
	namespace mesh_component_factory
	{
		void create(JsonValue& json, Entity& entity, Scene* scene)
		{
            MeshComponent& component = scene->attach_mesh_component(entity);

#if defined(TERMINUS_WITH_EDITOR)
			component.mesh_name = String(json["mesh"].GetString());
#endif

            component.mesh = context::get_mesh_cache().load(std::string(json["mesh"].GetString()));
            
            // TODO: Material Overrides
		}
	}
}
