#include <Resource/collider_component_factory.h>
#include <physics/physics_engine.h>

namespace terminus
{
    namespace collider_component_factory
    {
        void create_sphere_collider(JsonValue& json, Entity& entity, Scene* scene, ColliderComponent& cmp)
        {
            float radius = json["radius"].GetFloat();
            bool set_from_mesh = json["set_from_mesh"].GetBool();
            
            if(set_from_mesh && scene->has_mesh_component(entity))
            {
                MeshComponent& mesh = scene->get_mesh_component(entity);
                radius = glm::distance(mesh.mesh->m_MaxExtents, mesh.mesh->m_MinExtents) / 2.0f;
            }
            
            physics::create_sphere_shape(cmp._sphere, radius);
        }
        
        void create_box_collider(JsonValue& json, Entity& entity, Scene* scene, ColliderComponent& cmp)
        {
            rapidjson::Value& extents = json["half_extents"];
            Vector3 half_extents;
            
            half_extents.x = extents["x"].GetFloat();
            half_extents.y = extents["y"].GetFloat();
            half_extents.z = extents["z"].GetFloat();
            
            bool set_from_mesh = json["set_from_mesh"].GetBool();
            
            if(set_from_mesh && scene->has_mesh_component(entity))
            {
                MeshComponent& mesh = scene->get_mesh_component(entity);
                half_extents = (mesh.mesh->m_MaxExtents - mesh.mesh->m_MinExtents) / 2.0f;
            }
            
            physics::create_box_shape(cmp._box, half_extents);
        }
        
        void create_cylinder_collider(JsonValue& json, Entity& entity, Scene* scene, ColliderComponent& cmp)
        {
            rapidjson::Value& extents = json["half_extents"];
            Vector3 half_extents;
            
            half_extents.x = extents["x"].GetFloat();
            half_extents.y = extents["y"].GetFloat();
            half_extents.z = extents["z"].GetFloat();
            
            bool set_from_mesh = json["set_from_mesh"].GetBool();
            
            if(set_from_mesh && scene->has_mesh_component(entity))
            {
                MeshComponent& mesh = scene->get_mesh_component(entity);
                half_extents = (mesh.mesh->m_MaxExtents - mesh.mesh->m_MinExtents) / 2.0f;
            }
            
            physics::create_cylinder_shape(cmp._cylinder, half_extents);
        }
        
        void create_capsule_collider(JsonValue& json, Entity& entity, Scene* scene, ColliderComponent& cmp)
        {
            float radius = json["radius"].GetFloat();
            float height = json["height"].GetFloat();
            bool set_from_mesh = json["set_from_mesh"].GetBool();
            
            if(set_from_mesh && scene->has_mesh_component(entity))
            {
                MeshComponent& mesh = scene->get_mesh_component(entity);
                radius = glm::distance(Vector2(mesh.mesh->m_MaxExtents.x, mesh.mesh->m_MaxExtents.z),
                                       Vector2(mesh.mesh->m_MinExtents.x, mesh.mesh->m_MinExtents.z)) / 2.0f;
                
                height = mesh.mesh->m_MaxExtents.y - mesh.mesh->m_MinExtents.y;
            }
            
            physics::create_capsule_shape(cmp._capsule, radius, height);
        }

        
        void create_plane_collider(JsonValue& json, Entity& entity, Scene* scene, ColliderComponent& cmp)
        {
            rapidjson::Value& plane_normal = json["normal"];
            Vector3 normal;
            
            normal.x = plane_normal["x"].GetFloat();
            normal.y = plane_normal["y"].GetFloat();
            normal.z = plane_normal["z"].GetFloat();
            
            float constant = json["constant"].GetFloat();
            
            physics::create_staic_plane_shape(cmp._plane, normal, constant);
        }
        
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            String collider_type = String(json["collider_type"].GetString());
            ColliderComponent& cmp = scene->attach_collider_component(entity);
            
            if(collider_type == "SPHERE")
            {
                cmp._type = CollisionShapeType::SPHERE;
                create_sphere_collider(json, entity, scene, cmp);
            }
            else if(collider_type == "BOX")
            {
                cmp._type = CollisionShapeType::BOX;
                create_box_collider(json, entity, scene, cmp);
            }
            else if(collider_type == "CYLINDER")
            {
                cmp._type = CollisionShapeType::CYLINDER;
                create_cylinder_collider(json, entity, scene, cmp);
            }
            else if(collider_type == "CAPSULE")
            {
                cmp._type = CollisionShapeType::CAPSULE;
                create_capsule_collider(json, entity, scene, cmp);
            }
            else if(collider_type == "PLANE")
            {
                cmp._type = CollisionShapeType::STATIC_PLANE;
                create_plane_collider(json, entity, scene, cmp);
            }
        }
    }
}
