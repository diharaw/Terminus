#pragma once

#include <ECS/component_types.h>

namespace terminus
{
    enum EditorComponentType
    {
        TRANSFORM = 0,
        MESH,
        LUA_SCRIPT,
        CPP_SCRIPT,
        CAMERA,
        BOX_COLLIDER,
        SPHERE_COLLIDER,
        HEIGHT_FIELD_COLLIDER,
        TRIANGLE_MESH_COLLIDER
    };
    
    struct ComponentDesc
    {
        
    };
    
    struct TransformDesc : public ComponentDesc
    {
        TransformComponent cmp;
    };
    
    struct MeshDesc : public ComponentDesc
    {
        MeshComponent cmp;
    };
    
    struct CameraDesc : public ComponentDesc
    {
        CameraComponent cmp;
    };
    
    struct LuaScriptDesc : public ComponentDesc
    {
        LuaScriptComponent cmp;
    };
    
    struct CppScriptDesc : public ComponentDesc
    {
        CppScriptComponent cmp;
    };
    
    struct SphereColliderDesc : public ComponentDesc
    {
        SphereColliderComponent cmp;
    };
    
    struct BoxColliderDesc : public ComponentDesc
    {
        BoxColliderComponent cmp;
    };

    struct HeightFieldColliderDesc : public ComponentDesc
    {
        HeightFieldColliderComponent cmp;
    };

    struct TriangleMeshColliderDesc : public ComponentDesc
    {
        TriangleMeshColliderComponent cmp;
    };

    struct ConvexHullColliderDesc : public ComponentDesc
    {
        ConvexHullColliderComponent cmp;
    };
  
}
