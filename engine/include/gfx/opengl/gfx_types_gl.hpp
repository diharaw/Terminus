#pragma once

#include <vector>
#include <unordered_map>
#include <stdint.h>

#if defined(TE_GFX_BACKEND_GL) || defined(TE_GFX_BACKEND_GLES)

#if defined(TE_GFX_BACKEND_GLES)
    #ifdef __APPLE__
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #else
        #include <GLES3/gl3.h>
        #include <GLES2/gl2platform.h>
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
        #include <EGL/egl.h>
        #include <EGL/eglext.h>
    #endif
#else
    #include <gfx/gl/glad.h>
#endif

struct Shader;

using ShaderMap = std::unordered_map<uint32_t, Shader*>;
using BindingMap = std::unordered_map<uint32_t, uint32_t>;

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
	uint32_t mipLevels;
	uint32_t arraySize;
	GLenum internalFormat;
	GLenum format;
	GLenum type;
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
	uint16_t width;
	uint16_t height;
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
    GLuint id;
    uint16_t resource_id;
};

struct BlendState
{
	bool enable;
	uint32_t src_func;
	uint32_t dst_func;
	uint32_t blend_op;
	uint32_t src_func_alpha;
	uint32_t dst_func_alpha;
	uint32_t blend_op_alpha;
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

#endif
