#pragma once

#include <core/macro.h>

TERMINUS_BEGIN_NAMESPACE

// in a 64-bit key
// 5-bits | 4-bits | 10-bits | 10-bits | 1-bit | 1-bit   | 2-bits | 24-bits
// Pass	  |	Subpass|  Mat Id | VAO	   | Z-Buf | Stencil | Cull   | Depth

struct VertexArray;
struct Material;
struct PerDrawUniforms;

struct SortKey
{
    uint64 key;
    
    inline void set_render_pass(int _renderPass) { uint64 temp = _renderPass; key |= temp << 59; }
    inline void set_render_subpass(int _renderSubPass) { uint64 temp = _renderSubPass; key |= temp << 55; }
    inline void set_material(int _material) { uint64 temp = _material; key |= temp << 45; }
    inline void set_vertex_array(int _vertexArray) { uint64 temp = _vertexArray; key |= temp << 35; }
    inline void set_depth_test(int _depthTest) { uint64 temp = _depthTest; key |= temp << 34; }
    inline void set_stencil_test(int _stencilTest) { uint64 temp = _stencilTest; key |= temp << 33; }
    inline void set_face_culling(int _faceCull) { uint64 temp = _faceCull; key |= temp << 31; }
    inline void set_depth(float _depth) { key |= (static_cast<uint64>(_depth) & 0xffff); }
};

enum class RenderableType
{
    StaticMesh = 0,
    SkeletalMesh,
    Terrain,
    Ocean,
    Particle,
    Skybox,
    Quad
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
    int            renderable_id;
};

TERMINUS_END_NAMESPACE
