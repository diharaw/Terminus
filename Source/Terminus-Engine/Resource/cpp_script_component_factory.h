#pragma once

#include <types.h>
#include <ECS/scene.h>
#include <Resource/script_library_cache.h>

namespace terminus
{
    namespace cpp_script_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
}

