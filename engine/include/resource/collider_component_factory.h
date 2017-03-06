#pragma once

#include <core/types.h>
#include <gameplay/scene.h>

namespace terminus
{
    namespace collider_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
}
