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
            
            if(scene->has_collider_component(entity))
            {
                ColliderComponent& collider = scene->get_collider_component(entity);
                
                switch (collider._type)
                {
                    case CollisionShapeType::SPHERE:
                        shape = &collider._sphere;
                        break;
                        
                    case CollisionShapeType::BOX:
                        shape = &collider._sphere;
                        break;
                        
                    case CollisionShapeType::CYLINDER:
                        shape = &collider._cylinder;
                        break;
                        
                    case CollisionShapeType::CAPSULE:
                        shape = &collider._capsule;
                        break;
                        
                    case CollisionShapeType::STATIC_PLANE:
                        shape = &collider._plane;
                        break;
 
                    default:
                        break;
                }
            }
           
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
