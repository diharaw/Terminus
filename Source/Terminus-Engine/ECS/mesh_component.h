#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include <ECS/component.h>
#include <Resource/mesh.h>

namespace terminus
{
    struct MeshComponent : IComponent
    {
        static const ComponentID _id;
        Mesh* mesh;
        bool cull_submeshes;
        bool casts_shadow;
        bool is_visible;
    };
}

#endif
