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
        StringBuffer32 _mesh_name;
    };
    
    struct CameraDesc : public ComponentDesc
    {
        CameraComponent cmp;
    };
    
    struct LuaScriptDesc : public ComponentDesc
    {
        StringBuffer32 _script_name;
        StringBuffer32 _class_name;
    };
    
    struct CppScriptDesc : public ComponentDesc
    {
        StringBuffer32 _script_name;
        StringBuffer32 _class_name;
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
