#include <resource/rigid_body_component_factory.h>
#include <gameplay/component_types.h>
#include <physics/physics_engine.h>
#include <core/context.h>

namespace terminus
{
    namespace rigid_body_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
			// @NOTE: Come up with a more elegant way to set gravity.
			if(!scene->m_physics_scene.active())
				context::get_physics_engine().create_scene(scene->m_physics_scene, Vector3(0.0f, -10.0f, 0.0f));

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
				RigidBodyCreateDesc desc;

				desc.transform = transform._global_transform;
				desc.cof_offset = Matrix4();
				desc.mass = mass;
				desc.restituation = restitution;
				desc.friction = friction;
				desc.kinematic;
				desc.shape = shape;

				context::get_physics_engine().create_rigid_body(component._rigid_body, desc);
            }
        }
    }
}
