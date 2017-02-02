#pragma once

#include <types.h>
#include <ECS/scene.h>

namespace terminus
{
    namespace cpp_script_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
}

