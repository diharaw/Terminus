#include <Resource/rigid_body_component_factory.h>
#include <ECS/component_types.h>
#include <physics/physics_engine.h>

namespace terminus
{
    namespace rigid_body_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            CollisionShape* shape = nullptr;
            
            if(scene->has_sphere_collider_component(entity))
                shape = &scene->get_sphere_collider_component(entity)._collision_shape;
            else if(scene->has_box_collider_component(entity))
                shape = &scene->get_box_collider_component(entity)._collision_shape;
            else if(scene->has_cylinder_collider_component(entity))
                shape = &scene->get_cylinder_collider_component(entity)._collision_shape;
            else if(scene->has_capsule_collider_component(entity))
                shape = &scene->get_capsule_collider_component(entity)._collision_shape;
            
            if(shape)
            {
                float mass = json["mass"].GetFloat();
                float restitution = json["restitution"].GetFloat();
                float friction = json["friction"].GetFloat();
                bool kinematic = json["kinematic"].GetBool();
                
                TransformComponent& transform = scene->get_transform_component(entity);
                
                RigidBodyComponent& component = scene->attach_rigid_body_component(entity);
                component._rigid_body = physics::create_rigid_body(transform._global_transform,
                                                                   Matrix4(),
                                                                   mass,
                                                                   restitution,
                                                                   friction,
                                                                   kinematic,
                                                                   shape);
            }
        }
    }
}
