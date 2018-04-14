#pragma once

#include "macros.hpp"
#include <vector>
#include <stdint.h>
#include <unordered_map>

#if defined(GFX_BACKEND_GL4)

#include "gl_core_4_5.h"

GFX_BEGIN_NAMESPACE

struct Shader;

using ShaderMap = std::unordered_map<uint32_t, Shader*>;
using BindingMap = std::unordered_map<uint32_t, uint32_t>;

struct InputElement
{
    uint32_t	num_sub_elements;
    uint32_t    type;
    bool		normalized;
    uint32_t	offset;
    const char* semantic_name;
};

struct InputLayout
{
    InputElement elements[10];
    uint32_t	 vertex_size;
    uint32_t	 num_elements;
};

struct Texture
{
    GLuint   id;
    uint16_t resource_id;
    GLenum   gl_texture_target;
};

struct Texture1D : Texture
{
    
};

struct Texture2D : Texture
{
    uint16_t width;
    uint16_t height;
};

struct Texture3D : Texture
{
    
};

struct TextureCube : Texture
{
    
};

struct Buffer
{
    GLuint   id;
    uint16_t resource_id;
    void*	 data;
    uint32_t size;
    uint32_t usage_type;
    GLenum   buffer_type;
};

struct VertexBuffer : Buffer
{
    
};

struct IndexBuffer : Buffer
{
    GLenum type;
};

struct UniformBuffer : Buffer
{
    
};

struct VertexArray
{
    GLuint      id;
    uint16_t	  resource_id;
    VertexBuffer* vb;
    IndexBuffer*  ib;
};

struct Shader
{
    GLuint      id;
    BindingMap  sampler_bindings;
    uint32_t    type;
    std::string source;
};

struct ShaderProgram
{
    GLuint    id;
    uint16_t  resource_id;
    ShaderMap shader_map;
    int       shader_count;
};

struct RasterizerState
{
    GLenum cull_face;
    GLenum polygon_mode;
    bool   enable_cull_face;
    bool   enable_multisample;
    bool   enable_front_face_ccw;
    bool   enable_scissor;
};

struct DepthStencilState
{
    bool   enable_depth;
    bool   enable_stencil;
    
    GLenum depth_func;
    bool   depth_mask;
    
    GLenum front_stencil_comparison;
    GLenum back_stencil_comparison;
    GLuint stencil_mask;
    
    GLenum front_stencil_fail;
    GLenum front_stencil_pass_depth_fail;
    GLenum front_stencil_pass_depth_pass;
    
    GLenum back_stencil_fail;
    GLenum back_stencil_pass_depth_fail;
    GLenum back_stencil_pass_depth_pass;
};

struct SamplerState
{
    GLuint	 id;
    uint16_t resource_id;
};

struct BlendState
{
    
};

struct Framebuffer
{
    GLuint   id;
    uint16_t resource_id;
    uint16_t num_render_targets;
    Texture* render_targets[MAX_RENDER_TARGETS];
    Texture* depth_target;
};

struct PipelineStateObject
{
    DepthStencilState* depth_stencil_state;
    RasterizerState*   rasterizer_state;
    BlendState*        blend_state;
    uint32_t           primitive;
};

struct DeviceData
{
    void*          window;
    uint16_t       width;
    uint16_t       height;
    GLenum		   primitive_type;
    ShaderProgram* current_program;
    GLuint		   last_sampler_location;
    IndexBuffer*   current_index_buffer = nullptr;
};

GFX_END_NAMESPACE

#endif
