#pragma once

#include <types.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace terminus
{
    struct RigidBody
    {
        
    };
    
    struct SphereShape
    {
        btSphereShape* _shape;
        float          _radius;
    };
    
    struct BoxShape
    {
        btBoxShape* _shape;
        Vector3     _half_extents;
    };
    
    struct CylinderShape
    {
        btCylinderShape* _shape;
        Vector3          _half_extents;
    };
    
    struct CapsuleShape
    {
        btCapsuleShape* _shape;
        float           _radius;
        float           _height;
    };
    
    // TODO(dihara) : Bvh Triangle Mesh Shape
    struct TriangleMeshShape
    {
        btBvhTriangleMeshShape* _shape;
    };
    
    // TODO(dihara) : Convex Hull Shape
    struct ConvexHullShape
    {
        btConvexHullShape* _shape;
    };
    
    struct StaticPlaneShape
    {
        btStaticPlaneShape* _shape;
        Vector3             _normal;
        float               _constant;
    };
    
    struct HeightFieldShape
    {
        btHeightfieldTerrainShape* _shape;
        int                        _height;
        int                        _width;
    };
}
