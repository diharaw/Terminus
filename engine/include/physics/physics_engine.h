#pragma once

#include <physics/physics_types.h>
#include <container/packed_array.h>

#include <core/types.h>

#define MAX_RIGID_BODIES 1024
#define MAX_COLLISION_SHAPES 1024

namespace terminus
{
    struct PhysicsEngineState
    {
        btDynamicsWorld* _dynamics_world;
        btDispatcher* _dispatcher;
        btBroadphaseInterface* _broadphase;
        btConstraintSolver* _constraint_solver;
        btCollisionConfiguration* _collision_config;
        PackedArray<RigidBody, MAX_RIGID_BODIES> _rigid_bodies;
        PackedArray<btCollisionShape*, MAX_COLLISION_SHAPES> _collision_shapes;
        Vector3 _gravity;
    };
    
    namespace physics
    {
        extern void initialize();
        extern void update(double dt);
        extern void shutdown();
        extern void set_gravity(Vector3 gravity);
        extern void create_sphere_shape(SphereShape& shape, float radius);
        extern void create_box_shape(BoxShape& shape, Vector3 half_extents);
        extern void create_cylinder_shape(CylinderShape& shape, Vector3 half_extents);
        extern void create_capsule_shape(CapsuleShape& shape, float radius, float height);
        extern void create_triangle_mesh_shape(TriangleMeshShape& shape);
        extern void create_height_field_shape(HeightFieldShape& shape);
        extern void create_convex_hull_shape(ConvexHullShape& shape);
        extern void create_staic_plane_shape(StaticPlaneShape& shape, Vector3 normal, float constant);
        extern void destroy_collision_shape(CollisionShape* shape);
        extern ID   create_rigid_body(Matrix4 transform, Matrix4 cof_offset, float mass, float restituation, float friction, bool kinematic, CollisionShape* shape);
        extern Matrix4 get_world_transform(ID rigid_body, PhysicsEngineState& state);
        extern Vector3 get_position(ID rigid_body, PhysicsEngineState& state);
        extern Quaternion get_rotation(ID rigid_body, PhysicsEngineState& state);
        extern void set_rigid_body_user_data(ID rigid_body, void* user_data);
        extern void destroy_rigid_body(ID rigid_body);
        
    }
}
