#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "RenderDevice.h"
#include "Material.h"

namespace Terminus { namespace Graphics {
    
    enum class RenderableType
    {
        StaticMesh = 0,
        SkeletalMesh,
        Terrain,
        Ocean,
        Particle,
        Skybox
    };
    
    struct RenderCommand
    {
        RenderableType type;
        Material*      material;
        VertexArray*   vertex_array;
        ShaderProgram* shader_program;
        Framebuffer*   framebuffer;
        int            index_count;
        unsigned int   base_index;
        unsigned int   base_vertex;
    };
    
} }

#endif
