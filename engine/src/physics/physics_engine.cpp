#include <physics/physics_engine.h>
#include <core/context.h>

#define MAX_PHYSICS_WORLDS 10

namespace terminus
{
	namespace physics
	{
		btVector3 convert(Vector3 vec)
		{
			return btVector3(vec.x, vec.y, vec.z);
		}

		Vector3 convert(btVector3 vec)
		{
			return Vector3(vec.x(), vec.y(), vec.z());
		}

		Matrix4 convert(btTransform bt_mat)
		{
			Matrix4 mat;
			bt_mat.getOpenGLMatrix(glm::value_ptr(mat));
			return mat;
		}

		btTransform convert(Matrix4 mat)
		{
			btTransform bt_mat;
			bt_mat.setFromOpenGLMatrix(glm::value_ptr(mat));
			return bt_mat;
		}

		Quaternion convert(btQuaternion bt_quat)
		{
			Quaternion quat;
			quat.x = bt_quat.getX();
			quat.y = bt_quat.getY();
			quat.z = bt_quat.getZ();
			quat.w = bt_quat.getW();

			return quat;
		}
	}

	PhysicsEngine::PhysicsEngine()
	{
		void* memory = global::get_default_allocator()->Allocate(sizeof(PhysicsWorld) * MAX_PHYSICS_WORLDS, 8);
		_world_allocator = TE_NEW(global::get_default_allocator()) PoolAllocator(sizeof(PhysicsWorld),
																				 alignof(PhysicsWorld),
																				 sizeof(PhysicsWorld) * MAX_PHYSICS_WORLDS,
																				 memory);
	}

	PhysicsEngine::~PhysicsEngine()
	{

	}

	void PhysicsEngine::create_scene(PhysicsScene& scene, Vector3 gravity)
	{
		scene._world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _constraint_solver, _collision_config);
	}

	void PhysicsEngine::destroy_scene(PhysicsScene& scene)
	{
		T_SAFE_DELETE(scene._world);
	}

	void PhysicsEngine::initialize()
	{
		_collision_config = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_collision_config);
		_broadphase = new btDbvtBroadphase();
		_constraint_solver = new btSequentialImpulseConstraintSolver();
	}

	void PhysicsEngine::shutdown()
	{
		T_SAFE_DELETE(_constraint_solver);
		T_SAFE_DELETE(_broadphase);
		T_SAFE_DELETE(_dispatcher);
		T_SAFE_DELETE(_collision_config);
	}

	void PhysicsEngine::create_sphere_shape(SphereShape& shape, float radius)
	{
		shape._radius = radius;
		shape._shape = new btSphereShape(radius);
	}

	void PhysicsEngine::create_box_shape(BoxShape& shape, Vector3 half_extents)
	{
		shape._half_extents = half_extents;
		shape._shape = new btBoxShape(physics::convert(half_extents));
	}

	void PhysicsEngine::create_cylinder_shape(CylinderShape& shape, Vector3 half_extents)
	{
		shape._half_extents = half_extents;
		shape._shape = new btCylinderShape(physics::convert(half_extents));
	}

	void PhysicsEngine::create_capsule_shape(CapsuleShape& shape, float radius, float height)
	{
		shape._radius = radius;
		shape._height = height;
		shape._shape = new btCapsuleShape(radius, height);
	}

	void PhysicsEngine::create_staic_plane_shape(StaticPlaneShape& shape, Vector3 normal, float constant)
	{
		shape._normal = normal;
		shape._constant = constant;
		shape._shape = new btStaticPlaneShape(physics::convert(normal), constant);
	}

	void PhysicsEngine::create_triangle_mesh_shape(TriangleMeshShape& shape)
	{

	}

	void PhysicsEngine::create_height_field_shape(HeightFieldShape& shape)
	{

	}

	void PhysicsEngine::create_convex_hull_shape(ConvexHullShape& shape)
	{

	}

	void PhysicsEngine::destroy_collision_shape(CollisionShape* shape)
	{
		T_SAFE_DELETE(shape->_shape);
	}

	void PhysicsEngine::create_rigid_body(RigidBody& rigid_body, RigidBodyCreateDesc& desc)
	{
		btVector3 inertia(0.0f, 0.0f, 0.0f);

		rigid_body._motion_state = new btDefaultMotionState(physics::convert(desc.transform), 
															physics::convert(desc.cof_offset));

		if (desc.mass != 0.0f)
			desc.shape->_shape->calculateLocalInertia(desc.mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo info(desc.mass, rigid_body._motion_state, desc.shape->_shape, inertia);
		info.m_friction = desc.friction;
		info.m_restitution = desc.restituation;

		rigid_body._rigid_body = new btRigidBody(info);
	}

	void PhysicsEngine::destroy_rigid_body(RigidBody& rigid_body)
	{
		T_SAFE_DELETE(rigid_body._rigid_body);
		T_SAFE_DELETE(rigid_body._motion_state);
	}
}
