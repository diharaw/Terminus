#pragma once

#include <core/types.h>

#include <container/packed_array.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace terminus
{
    enum class CollisionShapeType
    {
        SPHERE = 0,
        BOX,
        CYLINDER,
        CAPSULE,
        TRIANGLE_MESH,
        CONVEX_HULL,
        STATIC_MESH,
        HEIGHT_FIELD,
        STATIC_PLANE,
        NONE
    };
    
    struct RigidBody
    {
        btRigidBody*   _rigid_body;
        btMotionState* _motion_state;

		inline void set_rigid_body_user_data(void* user_data)
		{
			_rigid_body->setUserPointer(user_data);
		}

		inline Matrix4 get_world_transform()
		{
			btTransform bullet_transform;
			_motion_state->getWorldTransform(bullet_transform);

			return physics::convert(bullet_transform);
		}

		inline Vector3 get_position()
		{
			btTransform bullet_transform;
			_motion_state->getWorldTransform(bullet_transform);

			return physics::convert(bullet_transform.getOrigin());
		}

		inline Quaternion get_rotation()
		{
			btTransform bullet_transform;
			_motion_state->getWorldTransform(bullet_transform);

			return physics::convert(bullet_transform.getRotation());
		}
    };
    
    struct CollisionShape
    {
        CollisionShapeType _type;
        btCollisionShape*  _shape;
        ID                 _shape_id;
        
        CollisionShape()
        {
            _type = CollisionShapeType::NONE;
        }
    };
    
    struct SphereShape : public CollisionShape
    {
        float          _radius;
        
        SphereShape()
        {
            _type = CollisionShapeType::SPHERE;
        }
    };
    
    struct BoxShape : public CollisionShape
    {
        Vector3     _half_extents;
        
        BoxShape()
        {
            _type = CollisionShapeType::BOX;
        }
    };
    
    struct CylinderShape : public CollisionShape
    {
        Vector3          _half_extents;
        
        CylinderShape()
        {
            _type = CollisionShapeType::CYLINDER;
        }
    };
    
    struct CapsuleShape : public CollisionShape
    {
        float           _radius;
        float           _height;
        
        CapsuleShape()
        {
            _type = CollisionShapeType::CAPSULE;
        }
    };
    
    // TODO(dihara) : Bvh Triangle Mesh Shape
    struct TriangleMeshShape : public CollisionShape
    {
        TriangleMeshShape()
        {
            _type = CollisionShapeType::TRIANGLE_MESH;
        }
    };
    
    // TODO(dihara) : Convex Hull Shape
    struct ConvexHullShape : public CollisionShape
    {
        ConvexHullShape()
        {
            _type = CollisionShapeType::CONVEX_HULL;
        }
    };
    
    struct StaticPlaneShape : public CollisionShape
    {
        Vector3             _normal;
        float               _constant;
        
        StaticPlaneShape()
        {
            _type = CollisionShapeType::STATIC_PLANE;
        }
    };
    
    struct HeightFieldShape : public CollisionShape
    {
        int                        _height;
        int                        _width;
        
        HeightFieldShape()
        {
            _type = CollisionShapeType::HEIGHT_FIELD;
        }
    };

	struct PhysicsScene
	{
		btDynamicsWorld* _world;
		Vector3 _gravity;

		PhysicsScene()
		{
			_world = nullptr;
		}

		~PhysicsScene()
		{

		}

		inline void simulate(double dt)
		{
			assert(_world);
			_world->stepSimulation(dt);
		}

		inline bool active()
		{
			return (_world != nullptr);
		}

		inline void add_rigid_body(RigidBody& rigid_body)
		{
			assert(_world);
			if (rigid_body._rigid_body)
				_world->addRigidBody(rigid_body._rigid_body);
		}

		inline void remove_rigid_body(RigidBody& rigid_body)
		{
			assert(_world);
			if (rigid_body._rigid_body)
				_world->removeRigidBody(rigid_body._rigid_body);
		}

		inline void set_gravity(Vector3 gravity)
		{
			assert(_world);
			_world->setGravity(physics::convert(gravity));
			_gravity = gravity;
		}
	};
}
