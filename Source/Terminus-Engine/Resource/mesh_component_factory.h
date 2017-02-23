#pragma once

#include <types.h>
#include <ECS/entity.h>
#include <ECS/scene.h>
#include <ECS/component_types.h>
#include <Core/context.h>
#include <editor/editor_scene.h>
#include <editor/editor_entity.h>

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
        
#if defined(TERMINUS_WITH_EDITOR)
        extern void create_from_desc(MeshDesc* desc, Entity& entity, Scene* scene)
        {
            MeshComponent& component = scene->attach_mesh_component(entity);
            component.mesh_name = desc->_mesh_name.c_str();
            component.mesh = context::get_mesh_cache().load(String(desc->_mesh_name.c_str()));
        }
        
        void create(JsonValue& json, EditorEntity& entity)
        {
            MeshDesc* desc = new MeshDesc();
            desc->_mesh_name = json["mesh"].GetString();
            context::get_mesh_cache().load(std::string(json["mesh"].GetString()));
            
            entity.add_component_desc(EditorComponentType::MESH, desc);
        }
#endif
	}
}
