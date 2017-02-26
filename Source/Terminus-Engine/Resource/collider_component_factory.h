#pragma once

#include <types.h>
#include <ECS/scene.h>

namespace terminus
{
    namespace sphere_collider_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
    
    namespace box_collider_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
    
    namespace capsule_collider_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
    
    namespace cylinder_collider_component_factory
    {
        extern void create(JsonValue& json, Entity& entity, Scene* scene);
    }
}
