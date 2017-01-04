#ifndef DRAWITEM_H
#define DRAWITEM_H

#include "render_device.h"
#include "Material.h"
#include "Renderer.h"

namespace terminus
{
    // in a 64-bit key
    // 5-bits | 4-bits | 10-bits | 10-bits | 1-bit | 1-bit   | 2-bits | 24-bits
    // Pass	  |	Subpass|  Mat Id | VAO	   | Z-Buf | Stencil | Cull   | Depth

    struct SortKey
    {
        uint64 key;
        
        inline void SetRenderPass(int _renderPass) { uint64 temp = _renderPass; key |= temp << 59; }
        inline void SetRenderSubpass(int _renderSubPass) { uint64 temp = _renderSubPass; key |= temp << 55; }
        inline void SetMaterial(int _material) { uint64 temp = _material; key |= temp << 45; }
        inline void SetVertexArray(int _vertexArray) { uint64 temp = _vertexArray; key |= temp << 35; }
        inline void SetDepthTest(int _depthTest) { uint64 temp = _depthTest; key |= temp << 34; }
        inline void SetStencilTest(int _stencilTest) { uint64 temp = _stencilTest; key |= temp << 33; }
        inline void SetFaceCulling(int _faceCull) { uint64 temp = _faceCull; key |= temp << 31; }
        inline void SetDepth(float _depth) { key |= (static_cast<uint64>(_depth) & 0xffff); }
    };
    
    enum class RenderableType
    {
        StaticMesh = 0,
        SkeletalMesh,
        Terrain,
        Ocean,
        Particle,
        Skybox
    };
    
    struct DrawItem
    {
        SortKey        sort_key;
        RenderableType type;
        Material*      material;
        VertexArray*   vertex_array;
        PerDrawUniforms* uniforms;
        int            index_count;
        unsigned int   base_index;
        unsigned int   base_vertex;
    };
} // namespace terminus

#endif
