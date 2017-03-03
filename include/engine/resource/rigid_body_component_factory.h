#pragma once

#include <types.h>
#include <ECS/scene.h>

namespace terminus
{
    namespace rigid_body_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
}

