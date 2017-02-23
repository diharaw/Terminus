#pragma once

#include <types.h>
#include <ECS/scene.h>
#include <editor/editor_scene.h>
#include <editor/editor_entity.h>

namespace terminus
{
    namespace cpp_script_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
        
#if defined(TERMINUS_WITH_EDITOR)
        extern void create_from_desc(CppScriptDesc* desc, Entity& entity, Scene* scene);
        extern void create(JsonValue& json, EditorEntity& entity);
#endif
    }
}

