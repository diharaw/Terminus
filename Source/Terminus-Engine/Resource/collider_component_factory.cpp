#include <Resource/collider_component_factory.h>

namespace terminus
{
    namespace sphere_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            SphereColliderComponent& component = scene->attach_transform_component(entity);
        }
    }
    
    namespace box_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            
        }
    }
    
    namespace capsule_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            
        }
    }
    
    namespace cylinder_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            
        }
    }
}
