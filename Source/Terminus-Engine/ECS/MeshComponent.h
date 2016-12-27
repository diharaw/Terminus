#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"
#include "../Resource/Mesh.h"

namespace Terminus {  namespace ECS {
    
    struct MeshComponent : IComponent
    {
        static const ComponentID _id;
        Mesh* mesh;
        bool cull_submeshes;
        bool casts_shadow;
        bool is_visible;
    };
    
} }

#endif
