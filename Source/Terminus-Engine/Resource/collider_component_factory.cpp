#include <Resource/collider_component_factory.h>
#include <physics/physics_engine.h>

namespace terminus
{
    namespace sphere_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            SphereColliderComponent& component = scene->attach_shpere_collider_component(entity);
            float radius = json["radius"].GetFloat();
            bool set_from_mesh = json["set_from_mesh"].GetBool();
            
            if(set_from_mesh && scene->has_mesh_component(entity))
            {
                MeshComponent& mesh = scene->get_mesh_component(entity);
                radius = glm::distance(mesh.mesh->m_MaxExtents, mesh.mesh->m_MinExtents) / 2.0f;
            }
            
            physics::create_sphere_shape(component._collision_shape, radius);
        }
    }
    
    namespace box_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            BoxColliderComponent& component = scene->attach_box_collider_component(entity);
            
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
            
            physics::create_box_shape(component._collision_shape, half_extents);
        }
    }
    
    namespace capsule_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            CapsuleColliderComponent& component = scene->attach_capsule_collider_component(entity);
            
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
            
            physics::create_capsule_shape(component._collision_shape, radius, height);
        }
    }
    
    namespace cylinder_collider_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            CylinderColliderComponent& component = scene->attach_cylinder_collider_component(entity);
            
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
            
            physics::create_cylinder_shape(component._collision_shape, half_extents);
        }
    }
}
