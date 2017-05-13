#pragma once

#include <physics/physics_types.h>
#include <core/types.h>

#define MAX_RIGID_BODIES 1024
#define MAX_COLLISION_SHAPES 1024

namespace terminus
{
	class PoolAllocator;

	struct RigidBodyCreateDesc
	{
		Matrix4 transform;
		Matrix4 cof_offset;
		float mass;
		float restituation;
		float friction;
		bool kinematic;
		CollisionShape* shape;
	};

	namespace physics
	{
		extern btVector3 convert(Vector3 vec);
		extern Vector3 convert(btVector3 vec);
		extern Matrix4 convert(btTransform bt_mat);
		extern btTransform convert(Matrix4 mat);
		extern Quaternion convert(btQuaternion bt_quat);
	}

	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();
		void initialize();
		void shutdown();
		void create_scene(PhysicsScene& scene, Vector3 gravity);
		void destroy_scene(PhysicsScene& scene);
		void create_sphere_shape(SphereShape& shape, float radius);
		void create_box_shape(BoxShape& shape, Vector3 half_extents);
		void create_cylinder_shape(CylinderShape& shape, Vector3 half_extents);
		void create_capsule_shape(CapsuleShape& shape, float radius, float height);
		void create_triangle_mesh_shape(TriangleMeshShape& shape);
		void create_height_field_shape(HeightFieldShape& shape);
		void create_convex_hull_shape(ConvexHullShape& shape);
		void create_staic_plane_shape(StaticPlaneShape& shape, Vector3 normal, float constant);
		void destroy_collision_shape(CollisionShape* shape);
		void create_rigid_body(RigidBody& rigid_body, RigidBodyCreateDesc& desc);
		void destroy_rigid_body(RigidBody& rigid_body);

	private:
		PoolAllocator* _world_allocator;
		btDispatcher* _dispatcher;
		btBroadphaseInterface* _broadphase;
		btConstraintSolver* _constraint_solver;
		btCollisionConfiguration* _collision_config;
	};
}
