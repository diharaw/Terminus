#pragma once

#include <types.h>

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
    };
    
    struct CollisionShape
    {
        CollisionShapeType _type;
        btCollisionShape* _shape;
        
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
}
