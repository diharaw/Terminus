#if defined(TE_GFX_BACKEND_GL) || defined(TE_GFX_BACKEND_GLES)

#include <gfx/opengl/gfx_device_gl.hpp>

#include "utility.h"
#include "logger.h"

#define GFX_ENABLE_ERROR_CHECK

#ifdef GFX_ENABLE_ERROR_CHECK
#define GL_CHECK_ERROR(x)																		  \
x; {                                                                                              \
GLenum err(glGetError());																		  \
																								  \
while (err != GL_NO_ERROR)																		  \
{																								  \
std::string error;																				  \
																								  \
switch (err)																					  \
{																								  \
case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;						  \
case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;						  \
case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;						  \
case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;						  \
case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;		  \
}																								  \
																								  \
std::string formatted_error = "OPENGL: ";														  \
formatted_error = formatted_error + error;														  \
LOG_ERROR(formatted_error);																		  \
err = glGetError();																				  \
}																								  \
}
#else
#define GL_CHECK_ERROR(x)	x
#endif

const GLenum kTextureFormatTable[][3] =
{
	// Internal Format    // Format			  // Type
	{ GL_RGB32F,		  GL_RGB,			  GL_FLOAT } ,
	{ GL_RGBA32F,		  GL_RGBA,			  GL_FLOAT } ,
	{ GL_RGB32UI,		  GL_RGB,			  GL_UNSIGNED_INT } ,
	{ GL_RGBA32UI,		  GL_RGBA,			  GL_UNSIGNED_INT } ,
	{ GL_RGB32I,		  GL_RGB,			  GL_INT } ,
	{ GL_RGBA32I,		  GL_RGBA,			  GL_INT } ,
	{ GL_RG16F,			  GL_RG,			  GL_HALF_FLOAT } ,
	{ GL_RGB16F,		  GL_RGB,			  GL_HALF_FLOAT } ,
    { GL_RGBA16F,		  GL_RGBA,			  GL_HALF_FLOAT } ,
    { GL_RGB16UI,		  GL_RGB,			  GL_UNSIGNED_SHORT } ,
	{ GL_RGBA16UI,		  GL_RGBA,			  GL_UNSIGNED_SHORT } ,
    { GL_RGB16I,		  GL_RGB,			  GL_SHORT } ,
	{ GL_RGBA16I,		  GL_RGBA,			  GL_SHORT } ,
    { GL_RGB8,			  GL_RGB,			  GL_UNSIGNED_BYTE } ,
	{ GL_RGBA8,			  GL_RGBA,			  GL_UNSIGNED_BYTE } ,
    { GL_SRGB8,			  GL_RGB,			  GL_UNSIGNED_BYTE } ,
    { GL_SRGB8_ALPHA8,	  GL_RGBA,			  GL_UNSIGNED_BYTE } ,
    { GL_RGB8_SNORM,	  GL_RGB,			  GL_BYTE } ,
    { GL_RGBA8_SNORM,	  GL_RGBA,			  GL_BYTE } ,
    { GL_RGB8I,			  GL_RGB,			  GL_INT } ,
    { GL_RGBA8I,		  GL_RGBA,			  GL_INT } ,
    { GL_RGB8I,			  GL_RGB,			  GL_UNSIGNED_INT } ,
	{ GL_RGBA8UI,		  GL_RGBA,			  GL_UNSIGNED_INT } ,
	{ GL_R8,			  GL_RED,			  GL_UNSIGNED_BYTE } ,
	{ GL_R8_SNORM,		  GL_RED,			  GL_BYTE } ,
	{ GL_DEPTH_STENCIL,	  GL_DEPTH_STENCIL,	  GL_FLOAT_32_UNSIGNED_INT_24_8_REV } ,
	{ GL_DEPTH_STENCIL,	  GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8 } ,
	{ GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT },
	{ GL_RG32F,			  GL_RG,			  GL_FLOAT },
	{ GL_R16F,			  GL_RED,			  GL_UNSIGNED_SHORT }
};

const GLenum kShaderTypeTable[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
#ifndef __EMSCRIPTEN__
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER
#else
	0,
	0,
	0,
	0
#endif
};

const GLenum kBufferUsageTable[] =
{
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW
};

const GLenum kMapUsageTable[] =
{
#ifndef __EMSCRIPTEN__
	GL_READ_ONLY,
	GL_WRITE_ONLY,
	GL_READ_WRITE
#else
	0,
	0,
	0
#endif
};

const GLenum kBufferDataTypeTable[] =
{
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_INT,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT,
	GL_FLOAT
};

const GLenum kCullModeTable[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

const GLenum kFillModeTable[] =
{
#ifndef __EMSCRIPTEN__
	GL_FILL,
	GL_LINE
#else
	0,
	0
#endif
};

const GLenum kComparisonFunctionTable[] =
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

const GLenum kStencilOperationTable[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP
};

const GLenum kTextureWrapModeTable[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
#ifndef __EMSCRIPTEN__
	GL_CLAMP_TO_BORDER
#else
	0
#endif
};

const GLenum kTextureMagFilteringModeTable[] =
{
    GL_LINEAR,
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST,
    GL_TEXTURE_MAX_ANISOTROPY_EXT,
    GL_LINEAR,
    GL_NEAREST
};

const GLenum kTextureMinFilteringModeTable[] =
{
    GL_LINEAR,
    GL_NEAREST,
	GL_LINEAR_MIPMAP_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_TEXTURE_MAX_ANISOTROPY_EXT,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR
};

const GLenum kDrawPrimitiveTypeTable[] =
{
	GL_POINTS,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_LINES,
	GL_LINE_STRIP,
	GL_PATCHES
};

const GLenum kTextureTargetTable[] = 
{
#ifndef __EMSCRIPTEN__
	GL_TEXTURE_1D,
#else
	0,
#endif
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

const GLenum kBlendFuncTable[] = 
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE,
#ifndef __EMSCRIPTEN__
	GL_SRC1_COLOR,
	GL_ONE_MINUS_SRC1_COLOR,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA
#else
	0,
	0,
	0,
	0
#endif
};

const GLenum kBlendEquationTable[] =
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
};
 
GfxDevice::GfxDevice()
{
    
}

GfxDevice::~GfxDevice()
{
    
}

bool GfxDevice::init(ExtensionLoaderFunc func)
{
#if defined(__EMSCRIPTEN__)
    return true;
#else
#if defined(TE_GFX_BACKEND_GLES)
    if (gladLoadGLES2Loader(func) == 0)
#else
	if (gladLoadGLLoader(func) == 0)
#endif
	{
        LOG_ERROR("Failed to load OpenGL extensions");
		return false;
	}
	else
	{
        glEnable(GL_MULTISAMPLE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &m_max_work_group_count[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &m_max_work_group_count[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &m_max_work_group_count[2]);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &m_max_work_group_size[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &m_max_work_group_size[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &m_max_work_group_size[2]);

		return true;
	}
#endif
}

Shader* GfxDevice::create_shader(const char* source, uint32_t type)
{
	Shader* shader = new Shader();
	shader->type = type;

	GL_CHECK_ERROR(shader->id = glCreateShader(kShaderTypeTable[type]));

	GLint success;
	GLchar infoLog[512];

#if defined(__APPLE__)
    shader->source = "#version 410 core\n" + std::string(source);
#elif defined(__EMSCRIPTEN__)
    shader->source = "#version 300 es\n" + std::string(source);
#else
    shader->source = "#version 430 core\n" + std::string(source);
#endif

	const GLchar* src = shader->source.c_str();

	GL_CHECK_ERROR(glShaderSource(shader->id, 1, &src, NULL));
	GL_CHECK_ERROR(glCompileShader(shader->id));
	GL_CHECK_ERROR(glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success));

	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader->id, 512, NULL, infoLog);

        std::string log_error = "Shader compilation failed: ";
		log_error += std::string(infoLog);

		std::cout << log_error << std::endl;

		return nullptr;
	}

	std::cout << "Shader successfully compiled." << std::endl;

	return shader;
}

ShaderProgram* GfxDevice::create_shader_program(Shader** shaders, uint32_t count)
{
	ShaderProgram* shaderProgram = new ShaderProgram();
	GL_CHECK_ERROR(shaderProgram->id = glCreateProgram());

	for (uint32_t i = 0; i < count; i++)
	{
		if (count > 1 && shaders[i]->type == GFX_SHADER_STAGE_COMPUTE)
        {
			LOG_ERROR("Compute shader programs cannot have more than one shader!");
            delete shaderProgram;
            return nullptr;
        }

		GL_CHECK_ERROR(glAttachShader(shaderProgram->id, shaders[i]->id));
        shaderProgram->shader_map[shaders[i]->type] = shaders[i];
	}

	GL_CHECK_ERROR(glLinkProgram(shaderProgram->id));

	GLint success;
	char infoLog[512];

	glGetProgramiv(shaderProgram->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram->id, 512, NULL, infoLog);

		std::string log_error = "Shader program linking failed";
		log_error += std::string(infoLog);

        LOG_ERROR(log_error);

		return nullptr;
	}

    // Bind Uniform Buffers
    for (auto it : shaderProgram->shader_map)
    {
        StringList uboList = Utility::find_line("#binding", it.second->source);

        for (int i = 0; i < uboList.size(); i++)
        {
            StringList tokens = Utility::delimit(" ", uboList[i]);
            std::string uniformName = tokens[3];
            GLuint binding = (GLuint)atoi(tokens[5].c_str());
            const GLchar* uniformNameChar = uniformName.c_str();
            GL_CHECK_ERROR(GLuint uboIndex = glGetUniformBlockIndex(shaderProgram->id, uniformNameChar));

            if (uboIndex == GL_INVALID_INDEX)
            {
                String uniform_error = "Failed to get Uniform Block Index for Uniform Buffer : ";
                uniform_error += uniformName;

                LOG_ERROR(uniform_error);
            }
            else
                GL_CHECK_ERROR(glUniformBlockBinding(shaderProgram->id, uboIndex, binding));
        }
    }

    // Bind Uniform Samplers
    for (auto it : shaderProgram->shader_map)
    {
        StringList samplerList = Utility::find_line("uniform sampler", it.second->source);

        for (int i = 0; i < samplerList.size(); i++)
        {
            StringList tokens = Utility::delimit(" ", samplerList[i]);
            std::string uniformName = tokens[2];
            uniformName.erase(uniformName.end() - 1, uniformName.end());

            GLuint binding = (GLuint)atoi(tokens[4].c_str());
            const GLchar* uniformNameChar = uniformName.c_str();
            GL_CHECK_ERROR(GLuint location = glGetUniformLocation(shaderProgram->id, uniformNameChar));

            if (location == GL_INVALID_INDEX)
            {
                String uniform_error = "Failed to get Uniform Location for Uniform : ";
                uniform_error += uniformName;

                LOG_ERROR(uniform_error);
            }

            it.second->sampler_bindings[binding] = location;
        }
    }

	return shaderProgram;
}

void GfxDevice::attach_render_target(Framebuffer* framebuffer, const RenderTargetDesc& desc)
{
	framebuffer->render_targets[framebuffer->num_render_targets++] = desc.texture;

    GL_CHECK_ERROR(glBindTexture(desc.texture->gl_texture_target, desc.texture->id));
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));

	// Bind specific layer of a layered texture
#ifndef __EMSCRIPTEN__
    if (desc.texture->gl_texture_target == GL_TEXTURE_1D_ARRAY        ||
        desc.texture->gl_texture_target == GL_TEXTURE_2D_ARRAY        ||
        desc.texture->gl_texture_target == GL_TEXTURE_CUBE_MAP_ARRAY)
#else
	if (desc.texture->gl_texture_target == GL_TEXTURE_2D_ARRAY)
#endif
    {
        int layer = desc.arraySlice;

        GL_CHECK_ERROR(glFramebufferTextureLayer(GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + (framebuffer->num_render_targets - 1),
            desc.texture->id,
            desc.mipSlice,
            layer));
    }
    else if (desc.texture->gl_texture_target == GL_TEXTURE_CUBE_MAP && desc.arraySlice > TextureType::TEXTURECUBE)
    {
        GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->num_render_targets - 1), kTextureTargetTable[desc.arraySlice], desc.texture->id, desc.mipSlice));
    }
    else
    {
		// Bind regular texture or entire layered texture
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->num_render_targets - 1), desc.texture->gl_texture_target, desc.texture->id, desc.mipSlice));
    }

	GLuint drawBuffers[MAX_RENDER_TARGETS];

	for (int i = 0; i < framebuffer->num_render_targets; i++)
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

	GL_CHECK_ERROR(glDrawBuffers(framebuffer->num_render_targets, &drawBuffers[0]));

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string error = "Framebuffer Incomplete: ";
        
        switch (status)
        {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            {
                error += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
                break;
            }
#ifndef __EMSCRIPTEN__
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            {
                error += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
                break;
            }
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            {
                error += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
                break;
            }
#endif
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            {
                error += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
                break;
            }
            case GL_FRAMEBUFFER_UNSUPPORTED:
            {
                error += "GL_FRAMEBUFFER_UNSUPPORTED";
                break;
            }
            default:
                break;
        }
        
        LOG_ERROR(error);
    }
    
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK_ERROR(glBindTexture(desc.texture->gl_texture_target, 0));
}

void GfxDevice::attach_depth_stencil_target(Framebuffer* framebuffer, const DepthStencilTargetDesc& desc)
{
	framebuffer->depth_target = desc.texture;
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));

#ifndef __EMSCRIPTEN__
	if (desc.texture->gl_texture_target == GL_TEXTURE_1D_ARRAY ||
		desc.texture->gl_texture_target == GL_TEXTURE_2D_ARRAY ||
		desc.texture->gl_texture_target == GL_TEXTURE_CUBE_MAP_ARRAY)
#else
	if (desc.texture->gl_texture_target == GL_TEXTURE_2D_ARRAY)
#endif
	{
		int layer = desc.arraySlice;

		GL_CHECK_ERROR(glFramebufferTextureLayer(GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			desc.texture->id,
			desc.mipSlice,
			layer));
	}
	else
	{
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, desc.texture->id, desc.mipSlice));
	}
    
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::string error = "Framebuffer Incomplete: ";

		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		}
#ifndef __EMSCRIPTEN__
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		}
#endif
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			error += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		}
		case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			error += "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		default:
			break;
		}

		LOG_ERROR(error);
	}
    
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CHECK_ERROR(glBindTexture(desc.texture->gl_texture_target, 0));
}

Framebuffer* GfxDevice::create_framebuffer(const FramebufferCreateDesc& desc)
{
	Framebuffer* framebuffer = new Framebuffer();
    
    framebuffer->num_render_targets = 0;

	GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer->id));

	for (int i = 0; i < desc.renderTargetCount; i++)
		attach_render_target(framebuffer, desc.renderTargets[i]);

	if (desc.depthStencilTarget.texture)
		attach_depth_stencil_target(framebuffer, desc.depthStencilTarget);

	return framebuffer;
}

VertexBuffer* GfxDevice::create_vertex_buffer(const BufferCreateDesc& desc)
{
	VertexBuffer* buffer = new VertexBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = kBufferUsageTable[desc.usage_type];

	buffer->buffer_type = GL_ARRAY_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;

	return buffer;
}

UniformBuffer* GfxDevice::create_uniform_buffer(const BufferCreateDesc& desc)
{
	UniformBuffer* buffer = new UniformBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = kBufferUsageTable[desc.usage_type];

	GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, buffer->id));
	GL_CHECK_ERROR(glBufferData(GL_UNIFORM_BUFFER, desc.size, desc.data, glusageType));
	GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	buffer->buffer_type = GL_UNIFORM_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;

	return buffer;
}

IndexBuffer* GfxDevice::create_index_buffer(const BufferCreateDesc& desc)
{
	IndexBuffer* buffer = new IndexBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = kBufferUsageTable[desc.usage_type];

	buffer->buffer_type = GL_ELEMENT_ARRAY_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;
	buffer->type = kBufferDataTypeTable[desc.data_type];

	return buffer;
}

InputLayout* GfxDevice::create_input_layout(const InputLayoutCreateDesc& desc)
{
    InputLayout* ia  = new InputLayout();
    
    memcpy(&ia->elements[0], &desc.elements[0], sizeof(InputElement) * desc.num_elements);
    
    ia->num_elements = desc.num_elements;
    ia->vertex_size  = desc.vertex_size;
    
    return ia;
}

VertexArray* GfxDevice::create_vertex_array(const VertexArrayCreateDesc& desc)
{
	VertexArray* vertexArray = new VertexArray();
	vertexArray->ib = desc.index_buffer;
	vertexArray->vb = desc.vertex_buffer;

	GL_CHECK_ERROR(glGenVertexArrays(1, &vertexArray->id));
	GL_CHECK_ERROR(glBindVertexArray(vertexArray->id));

	GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, desc.vertex_buffer->id));
	GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, desc.vertex_buffer->size, desc.vertex_buffer->data, desc.vertex_buffer->usage_type));

    if(desc.index_buffer)
    {
        GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->id));
        GL_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->size, desc.index_buffer->data, desc.index_buffer->usage_type));
    }
	
	for (uint32_t i = 0; i < desc.layout->num_elements; i++)
	{
		GL_CHECK_ERROR(glEnableVertexAttribArray(i));
		GL_CHECK_ERROR(glVertexAttribPointer(i, 
										     desc.layout->elements[i].num_sub_elements,
											 kBufferDataTypeTable[desc.layout->elements[i].type],
											 desc.layout->elements[i].normalized,
											 desc.layout->vertex_size,
											 (GLvoid*)((uint64_t)desc.layout->elements[i].offset)));
	}

	GL_CHECK_ERROR(glBindVertexArray(0));
	GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	return vertexArray;
}

Texture2D* GfxDevice::create_texture_2d(const Texture2DCreateDesc& desc)
{
	Texture2D* texture = new Texture2D();

	GL_CHECK_ERROR(glGenTextures(1, &texture->id));
	texture->gl_texture_target = GL_TEXTURE_2D;

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture->id));

	texture->internalFormat = kTextureFormatTable[desc.format][0];
    texture->format = kTextureFormatTable[desc.format][1];
	texture->type = kTextureFormatTable[desc.format][2];

	GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, texture->internalFormat, desc.width, desc.height, 0, texture->format, texture->type, desc.data));

	if (desc.mipmap_levels > 1) // @TODO: Allocate memory for mipmaps?
	{
		GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
	}

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

	return texture;
}

Texture2D* GfxDevice::create_texture_2d_array(const Texture2DArrayCreateDesc& desc)
{
	Texture2D* texture = new Texture2D();

	GL_CHECK_ERROR(glGenTextures(1, &texture->id));
	texture->gl_texture_target = GL_TEXTURE_2D_ARRAY;

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id));

	texture->internalFormat = kTextureFormatTable[desc.format][0];
	texture->format = kTextureFormatTable[desc.format][1];
	texture->type = kTextureFormatTable[desc.format][2];

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, texture->internalFormat, desc.width, desc.height, desc.array_slices, 0, texture->format, texture->type, nullptr);

	if (desc.mipmap_levels > 1) // @TODO: Allocate memory for mipmaps?
	{
		GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
	}

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

	return texture;
}

TextureCube* GfxDevice::create_texture_cube(const TextureCubeCreateDesc& desc)
{
	int mipLevels = 0;

	if (desc.mipmapLevels == 0)
		mipLevels = 1; // Base mip
	else
		mipLevels = desc.mipmapLevels;

	TextureCube* texture = new TextureCube();

	GL_CHECK_ERROR(glGenTextures(1, &texture->id));
	texture->gl_texture_target = GL_TEXTURE_CUBE_MAP;

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id));

	texture->internalFormat = kTextureFormatTable[desc.format][0];
	texture->format = kTextureFormatTable[desc.format][1];
	texture->type = kTextureFormatTable[desc.format][2];

	// Array order [+X, �X, +Y, �Y, +Z, �Z]
	for (int i = 0; i < 6; i++)
	{
		GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			texture->internalFormat,
			desc.width,
			desc.height,
			0,
			texture->format,
			texture->type,
			desc.initialData[i]));
	}

	if (desc.mipmapLevels > 0)
	{
		GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	}

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

	return texture;
}

void GfxDevice::set_texture_data(Texture* texture, const int& mipSlice, const int& arraySlice, uint16_t& width, uint16_t& height, void* data)
{
	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));

	GL_CHECK_ERROR(glTexImage2D(kTextureTargetTable[arraySlice],
								mipSlice,
								texture->internalFormat,
								width,
								height,
								0,
								texture->format,
								texture->type,
								data));

	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, 0));
}

void GfxDevice::generate_mipmaps(Texture* texture)
{
	if (texture)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void GfxDevice::wait_for_idle()
{
	glFinish();
}

RasterizerState* GfxDevice::create_rasterizer_state(const RasterizerStateCreateDesc& desc)
{
	RasterizerState* rasterizerState = new RasterizerState();

	rasterizerState->cull_face = kCullModeTable[desc.cull_mode];
	rasterizerState->enable_cull_face = desc.cull_mode != CullMode::NONE;
	rasterizerState->polygon_mode = kFillModeTable[desc.fill_mode];
	rasterizerState->enable_multisample = desc.multisample;
	rasterizerState->enable_scissor = desc.scissor;
	rasterizerState->enable_front_face_ccw = desc.front_winding_ccw;

	return rasterizerState;
}

DepthStencilState* GfxDevice::create_depth_stencil_state(const DepthStencilStateCreateDesc& desc)
{
	DepthStencilState* depthStencilState = new DepthStencilState();

	// Set Depth Options

	depthStencilState->enable_depth = desc.enable_depth_test;
	depthStencilState->depth_func = kComparisonFunctionTable[desc.depth_cmp_func];
	depthStencilState->depth_mask = (desc.depth_mask) ? GL_TRUE : GL_FALSE;

	// Set Stencil Options

	depthStencilState->enable_stencil = desc.enable_stencil_test;
	depthStencilState->front_stencil_comparison = kComparisonFunctionTable[0];
	depthStencilState->back_stencil_comparison = kComparisonFunctionTable[desc.back_stencil_cmp_func];

	// Front Stencil Operation

	depthStencilState->front_stencil_fail = kStencilOperationTable[desc.front_stencil_fail];
	depthStencilState->front_stencil_pass_depth_pass = kStencilOperationTable[desc.front_stencil_pass_depth_pass];
	depthStencilState->front_stencil_pass_depth_fail = kStencilOperationTable[desc.front_stencil_pass_depth_fail];

	// Back Stencil Operation

	depthStencilState->back_stencil_fail = kStencilOperationTable[desc.back_stencil_fail];
	depthStencilState->back_stencil_pass_depth_pass = kStencilOperationTable[desc.back_stencil_pass_depth_pass];
	depthStencilState->back_stencil_pass_depth_fail = kStencilOperationTable[desc.back_stencil_pass_depth_fail];
	depthStencilState->stencil_mask = desc.stencil_mask;

	return depthStencilState;
}

BlendState* GfxDevice::create_blend_state(const BlendStateCreateDesc& desc)
{
	BlendState* state = new BlendState();

	state->enable = desc.enable;
	state->src_func = desc.src_func;
	state->dst_func = desc.dst_func;
	state->blend_op = desc.blend_op;
	state->src_func_alpha = desc.src_func_alpha;
	state->dst_func_alpha = desc.dst_func_alpha;
	state->blend_op_alpha = desc.blend_op_alpha;

	return state;
}

SamplerState* GfxDevice::create_sampler_state(const SamplerStateCreateDesc& desc)
{
	SamplerState* samplerState = new SamplerState();

	GL_CHECK_ERROR(glGenSamplers(1, &samplerState->id));

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_S, kTextureWrapModeTable[desc.wrap_mode_u]));

#ifndef __EMSCRIPTEN__
	if (desc.wrap_mode_u == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_T, kTextureWrapModeTable[desc.wrap_mode_v]));

	if (desc.wrap_mode_v == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_R, kTextureWrapModeTable[desc.wrap_mode_w]));

	if (desc.wrap_mode_w == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}
#endif

	// Texture Filtering

    if ((desc.min_filter == TextureFilteringMode::ANISOTROPIC_ALL || desc.mag_filter == TextureFilteringMode::ANISOTROPIC_ALL) && desc.max_anisotropy > 0)
    {
#ifdef __EMSCRIPTEN__
		bool result = false;
#else
		bool result = ogl_ext_EXT_texture_filter_anisotropic == ogl_LOAD_SUCCEEDED;
#endif
        if (result)
        {
            GLfloat glmaxAnisotropy;
            GL_CHECK_ERROR(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glmaxAnisotropy));

            if (desc.max_anisotropy <= glmaxAnisotropy) {
                GL_CHECK_ERROR(glSamplerParameterf(samplerState->id, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc.max_anisotropy));
            }
            else {
                GL_CHECK_ERROR(glSamplerParameterf(samplerState->id, GL_TEXTURE_MAX_ANISOTROPY_EXT, glmaxAnisotropy));
            }
        }
        else
        {
            GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        }
    }
    else
    {
        GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MAG_FILTER, kTextureMagFilteringModeTable[desc.mag_filter]));
        GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MIN_FILTER, kTextureMinFilteringModeTable[desc.min_filter]));
    }

	return samplerState;
}

void GfxDevice::destroy(Shader* shader)
{
	if (shader)
	{
		GL_CHECK_ERROR(glDeleteShader(shader->id));
		delete shader;
	}
}

void GfxDevice::destroy(ShaderProgram* program)
{
	if (program)
	{
		GL_CHECK_ERROR(glDeleteProgram(program->id));
		delete program;
	}
}

void GfxDevice::destroy(VertexBuffer* vertex_buffer)
{
	if (vertex_buffer)
	{
		GL_CHECK_ERROR(glDeleteBuffers(1, &vertex_buffer->id));
		delete vertex_buffer;
	}
}

void GfxDevice::destroy(IndexBuffer* index_buffer)
{
	if (index_buffer)
	{
		GL_CHECK_ERROR(glDeleteBuffers(1, &index_buffer->id));
		delete index_buffer;
	}
}

void GfxDevice::destroy(UniformBuffer* buffer)
{
	GL_CHECK_ERROR(glDeleteBuffers(1, &buffer->id));
	delete buffer;
}

void GfxDevice::destroy(VertexArray* vertex_array)
{
	if (vertex_array)
	{
		GL_CHECK_ERROR(glDeleteVertexArrays(1, &vertex_array->id));
		delete vertex_array;
	}
}

void GfxDevice::destroy(Texture* texture)
{
	if (texture)
	{
		GL_CHECK_ERROR(glDeleteTextures(1, &texture->id));
		delete texture;
	}
}

void GfxDevice::destroy(RasterizerState* state)
{
	delete state;
}

void GfxDevice::destroy(BlendState* state)
{
	delete state;
}

void GfxDevice::destroy(SamplerState* state)
{
	GL_CHECK_ERROR(glDeleteSamplers(1, &state->id));
	delete state;
}

void GfxDevice::destroy(DepthStencilState* state)
{
	delete state;
}

void GfxDevice::destroy(PipelineStateObject* pso)
{
	destroy(pso->depth_stencil_state);
	destroy(pso->rasterizer_state);

	delete pso;
}

void GfxDevice::destroy(Framebuffer* framebuffer)
{
	if (framebuffer)
	{
		GL_CHECK_ERROR(glDeleteFramebuffers(1, &framebuffer->id));
		delete framebuffer;
	}
}

void GfxDevice::texture_extents(Texture* texture, const int& mipSlice, int& width, int& height)
{
#ifndef __EMSCRIPTEN__
	GLenum target = texture->gl_texture_target;

	if (texture->gl_texture_target == GL_TEXTURE_CUBE_MAP)
		target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; // If the texture is a cubemap, then query the extents of the +X face since they're all the same size.

	GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0));
	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));

	GL_CHECK_ERROR(glGetTexLevelParameteriv(target, mipSlice, GL_TEXTURE_WIDTH, &width));
	GL_CHECK_ERROR(glGetTexLevelParameteriv(target, mipSlice, GL_TEXTURE_HEIGHT, &height));

	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, 0));
#endif
}

void GfxDevice::texture_data(Texture* texture, const int& mipSlice, const int& arraySlice, void* data)
{
#ifndef __EMSCRIPTEN__
	const GLenum target = kTextureTargetTable[arraySlice];
	GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0));
	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));
	GL_CHECK_ERROR(glGetTexImage(target, mipSlice, texture->format, texture->type, data));
	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, 0));
#endif
}

void GfxDevice::bind_pipeline_state_object(PipelineStateObject* pso)
{
	bind_depth_stencil_state(pso->depth_stencil_state);
	bind_rasterizer_state(pso->rasterizer_state);
	set_primitive_type(pso->primitive);
}

int GfxDevice::uniform_buffer_alignment()
{
	GLint uniformBufferAlignSize = 0;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferAlignSize);
	return uniformBufferAlignSize;
}

void GfxDevice::bind_texture(Texture* texture, uint32_t shader_stage, uint32_t buffer_slot)
{
	if (m_device_data.last_sampler_location != GL_INVALID_INDEX)
	{
		GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));
	}
}

void GfxDevice::bind_uniform_buffer(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot)
{
	GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, buffer_slot, uniform_buffer->id));
}

void GfxDevice::bind_uniform_buffer_range(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot, size_t offset, size_t size)
{
	GL_CHECK_ERROR(glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, uniform_buffer->id, offset, size));
}

void GfxDevice::bind_vertex_array(VertexArray* vertex_array)
{
	m_device_data.current_index_buffer = vertex_array->ib;
	GL_CHECK_ERROR(glBindVertexArray(vertex_array->id));
}

void GfxDevice::bind_rasterizer_state(RasterizerState* state)
{
	if (state->enable_cull_face)
    {
        glEnable(GL_CULL_FACE);
        GL_CHECK_ERROR(glCullFace(state->cull_face));
    }
	else
		glDisable(GL_CULL_FACE);

#ifndef __EMSCRIPTEN__
	GL_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, state->polygon_mode));

	if (state->enable_multisample)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);
#endif

	if (state->enable_scissor)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	if (state->enable_front_face_ccw)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
}

void GfxDevice::bind_sampler_state(SamplerState* state, uint32_t shader_stage, uint32_t slot)
{
	GLuint location = m_device_data.current_program->shader_map[shader_stage]->sampler_bindings[slot];
	m_device_data.last_sampler_location = location;

	if (location != GL_INVALID_INDEX)
	{
		GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK_ERROR(glBindSampler(slot, state->id));
		GL_CHECK_ERROR(glUniform1i(m_device_data.current_program->shader_map[shader_stage]->sampler_bindings[slot], slot));
	}
}

void GfxDevice::bind_blend_state(BlendState* state)
{
	if (state->enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendEquationSeparate(kBlendEquationTable[state->blend_op], kBlendEquationTable[state->blend_op_alpha]);
	glBlendFuncSeparate(kBlendFuncTable[state->src_func], kBlendFuncTable[state->dst_func], kBlendFuncTable[state->src_func_alpha], kBlendFuncTable[state->dst_func_alpha]);
}

void GfxDevice::bind_framebuffer(Framebuffer* framebuffer)
{
	if (framebuffer)
	{
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));
	}
	else
	{
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}

void GfxDevice::bind_depth_stencil_state(DepthStencilState* state)
{
	// Set Depth Options

	if (state->enable_depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	GL_CHECK_ERROR(glDepthFunc(state->depth_func));
	GL_CHECK_ERROR(glDepthMask((state->depth_mask) ? GL_TRUE : GL_FALSE));

	// Set Stencil Options

	if (state->enable_stencil)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);

	GL_CHECK_ERROR(glStencilFuncSeparate(GL_FRONT, state->front_stencil_comparison, 1, state->stencil_mask));
	GL_CHECK_ERROR(glStencilFuncSeparate(GL_FRONT, state->back_stencil_comparison, 1, state->stencil_mask));

	// Front Stencil Operation

	GL_CHECK_ERROR(glStencilOpSeparate(GL_FRONT, state->front_stencil_fail, state->front_stencil_pass_depth_fail, state->front_stencil_pass_depth_pass));
	GL_CHECK_ERROR(glStencilOpSeparate(GL_BACK, state->back_stencil_fail, state->back_stencil_pass_depth_fail, state->back_stencil_pass_depth_pass));
}

void GfxDevice::bind_shader_program(ShaderProgram* program)
{
	m_device_data.current_program = program;
	GL_CHECK_ERROR(glUseProgram(program->id));
}

void* GfxDevice::map_buffer(Buffer* buffer, uint32_t type)
{
#ifndef __EMSCRIPTEN__
	void* ptr = nullptr;
	GL_CHECK_ERROR(glBindBuffer(buffer->buffer_type, buffer->id));
	GL_CHECK_ERROR(ptr = glMapBuffer(buffer->buffer_type, kMapUsageTable[type]));
	return ptr;
#else
	return nullptr;
#endif
}

void GfxDevice::unmap_buffer(Buffer* buffer)
{
#ifndef __EMSCRIPTEN__
	GL_CHECK_ERROR(glUnmapBuffer(buffer->buffer_type));
	GL_CHECK_ERROR(glBindBuffer(buffer->buffer_type, 0));
#endif
}

void GfxDevice::set_primitive_type(uint32_t primitive)
{
	m_device_data.primitive_type = kDrawPrimitiveTypeTable[primitive];
}

void GfxDevice::clear_framebuffer(uint32_t clear_target, float* clear_color)
{
	GL_CHECK_ERROR(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));

	uint32_t bits = ((clear_target & GFX_CLEAR_TARGET_COLOR) == GFX_CLEAR_TARGET_COLOR) ? GL_COLOR_BUFFER_BIT   : 0;
	bits |= ((clear_target & GFX_CLEAR_TARGET_DEPTH) == GFX_CLEAR_TARGET_DEPTH)         ? GL_DEPTH_BUFFER_BIT   : 0;
	bits |= ((clear_target & GFX_CLEAR_TARGET_STENCIL) == GFX_CLEAR_TARGET_STENCIL)     ? GL_STENCIL_BUFFER_BIT : 0;

    GL_CHECK_ERROR(glClear(bits));
}

void GfxDevice::set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
{
	GL_CHECK_ERROR(glViewport(top_left_x, top_left_y, width, height));
}

void GfxDevice::dispatch_compute(uint32_t x, uint32_t y, uint32_t z)
{
#ifndef __EMSCRIPTEN__
	GL_CHECK_ERROR(glDispatchCompute(x, y, z));
#endif
}

void GfxDevice::draw(uint32_t first_index, uint32_t count)
{
	GL_CHECK_ERROR(glDrawArrays(m_device_data.primitive_type, first_index, count));
}

void GfxDevice::draw_indexed(uint32_t index_count)
{
    GL_CHECK_ERROR(glDrawElements(m_device_data.primitive_type,
                                  index_count,
                                  ((m_device_data.current_index_buffer) ? m_device_data.current_index_buffer->type : GL_UNSIGNED_INT),
                                  0));
}

void GfxDevice::draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
{
#ifndef __EMSCRIPTEN__
	GL_CHECK_ERROR(glDrawElementsBaseVertex(m_device_data.primitive_type,
											index_count, 
											((m_device_data.current_index_buffer) ? m_device_data.current_index_buffer->type : GL_UNSIGNED_INT), 
											(void*)(sizeof(unsigned int) * base_index), 
											base_vertex));
#endif
}

#endif