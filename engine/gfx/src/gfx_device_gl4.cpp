#include "gfx_device.hpp"
#include "filesystem.hpp"
#include "string_utility.hpp"

GFX_BEGIN_NAMESPACE

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
case GL_INVALID_OPERATION:				error = "INVALID_OPERATION";			  break;		  \
case GL_INVALID_ENUM:					error = "INVALID_ENUM";					  break;		  \
case GL_INVALID_VALUE:					error = "INVALID_VALUE";				  break;		  \
case GL_OUT_OF_MEMORY:					error = "OUT_OF_MEMORY";				  break;		  \
case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;		  \
}																								  \
																								  \
std::string formatted_error = "OPENGL ERROR : ";                                                  \
formatted_error += error;                                                                         \
std::cout << formatted_error << std::endl;                                                        \
err = glGetError();																				  \
}																								  \
}
#else
#define GL_CHECK_ERROR(x)	x
#endif

const GLenum g_texture_format_table[][2] =
{
	{ GL_RGB, GL_FLOAT } ,
	{ GL_RGBA, GL_FLOAT } ,
	{ GL_RGB, GL_UNSIGNED_INT } ,
	{ GL_RGBA, GL_UNSIGNED_INT } ,
	{ GL_RGB, GL_INT } ,
	{ GL_RGBA, GL_INT } ,
	{ GL_RGBA, GL_FLOAT } ,
	{ GL_RGBA, GL_UNSIGNED_INT } ,
	{ GL_RGBA, GL_INT } ,
	{ GL_RGBA, GL_UNSIGNED_BYTE } ,
	{ GL_RGBA, GL_UNSIGNED_INT } ,
	{ GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV } ,
	{ GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 } ,
	{ GL_DEPTH_COMPONENT, GL_FLOAT }
};

const GLenum g_shader_type_table[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER
};

const GLenum g_buffer_usage_type_table[] =
{
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW
};

const GLenum g_map_type_table[] =
{
	GL_READ_ONLY,
	GL_WRITE_ONLY,
	GL_READ_WRITE
};

const GLenum g_buffer_data_type_table[] =
{
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_INT,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT,
	GL_FLOAT
};

const GLenum g_cull_mode_table[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

const GLenum g_fill_mode_table[] =
{
	GL_FILL,
	GL_LINE
};

const GLenum g_comparison_function_table[] =
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

const GLenum g_stencil_operation_table[] =
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

const GLenum g_texture_wrap_mode_table[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER
};

const GLenum g_texture_mag_filtering_mode_table[] =
{
	GL_LINEAR,
	GL_NEAREST,
	GL_TEXTURE_MAX_ANISOTROPY_EXT,
	GL_LINEAR,
	GL_NEAREST
};

const GLenum g_texture_min_filtering_mode_table[] =
{
	GL_LINEAR_MIPMAP_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_TEXTURE_MAX_ANISOTROPY_EXT,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR
};

const GLenum g_draw_primitive_type_table[] =
{
	GL_POINTS,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_LINES,
	GL_LINE_STRIP
};

Device::Device()
{
    
}

Device::~Device()
{
    
}

bool Device::init()
{
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "Failed to load functions" << std::endl;
		return false;
	}
	else
		return true;
}

Shader* Device::create_shader(const char* source, uint32_t type)
{
	Shader* shader = new Shader();
	shader->type = type;

	GL_CHECK_ERROR(shader->id = glCreateShader(g_shader_type_table[type]));

	GLint success;
	GLchar infoLog[512];

	shader->source = std::string(source);

	const GLchar* src = source;

	GL_CHECK_ERROR(glShaderSource(shader->id, 1, &src, NULL));
	GL_CHECK_ERROR(glCompileShader(shader->id));
	GL_CHECK_ERROR(glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success));

	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader->id, 512, NULL, infoLog);

		std::string log_error = "Shader compilation failed";
		log_error += std::string(infoLog);

		std::cout << log_error << std::endl;

		return nullptr;
	}

	std::cout << "Shader successfully compiled." << std::endl;

	return shader;
}

ShaderProgram* Device::create_shader_program(Shader** shaders, uint32_t count)
{
	ShaderProgram* shaderProgram = new ShaderProgram();
	GL_CHECK_ERROR(shaderProgram->id = glCreateProgram());

	bool is_compute = false;
	for (uint32_t i = 0; i < 6; i++)
	{
		if (count > 1 || shaders[i]->type == ShaderType::COMPUTE)
			return nullptr;

		GL_CHECK_ERROR(glAttachShader(shaderProgram->id, shaders[i]->id));
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

		std::cout << log_error << std::endl;

		return nullptr;
	}

	// Bind Uniform Buffers
	for (auto it : shaderProgram->shader_map)
    {
		StringList uboList = StringUtility::find_line("#binding", it.second->source);

		for (int i = 0; i < uboList.size(); i++)
		{
			StringList tokens = StringUtility::delimit(" ", uboList[i]);
			std::string uniformName = tokens[3];
			GLuint binding = (GLuint)atoi(tokens[5].c_str());
			const GLchar* uniformNameChar = uniformName.c_str();
			GL_CHECK_ERROR(GLuint uboIndex = glGetUniformBlockIndex(shaderProgram->id, uniformNameChar));

			if (uboIndex == GL_INVALID_INDEX)
			{
				String uniform_error = "Failed to get Uniform Block Index for Uniform Buffer : ";
				uniform_error += uniformName;

				std::cout << uniform_error << std::endl;
			}
			else
				GL_CHECK_ERROR(glUniformBlockBinding(shaderProgram->id, uboIndex, binding));
		}
    }

	// Bind Uniform Samplers 
	for (auto it : shaderProgram->shader_map)
	{
		StringList samplerList = StringUtility::find_line("uniform sampler", it.second->source);

		for (int i = 0; i < samplerList.size(); i++)
		{
			StringList tokens = StringUtility::delimit(" ", samplerList[i]);
			std::string uniformName = tokens[2];
			uniformName.erase(uniformName.end() - 1, uniformName.end());

			GLuint binding = (GLuint)atoi(tokens[4].c_str());
			const GLchar* uniformNameChar = uniformName.c_str();
			GL_CHECK_ERROR(GLuint location = glGetUniformLocation(shaderProgram->id, uniformNameChar));

			if (location == GL_INVALID_INDEX)
			{
				String uniform_error = "Failed to get Uniform Location for Uniform : ";
				uniform_error += uniformName;

				std::cout << uniform_error << std::endl;
			}

			it.second->sampler_bindings[binding] = location;
		}
	}

	return shaderProgram;
}

void Device::attach_render_target(Framebuffer* framebuffer, Texture* render_target)
{
	framebuffer->num_render_targets++;
	framebuffer->render_targets[framebuffer->num_render_targets++] = render_target;

	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));
	GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, framebuffer->id));
	GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->num_render_targets - 1),
		render_target->gl_texture_target,
		render_target->id, 0));

	std::vector<GLuint> drawBuffers;

	for (int i = 0; i < framebuffer->num_render_targets; i++)
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

	GL_CHECK_ERROR(glDrawBuffers(framebuffer->num_render_targets, &drawBuffers[0]));

	GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, 0));
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Device::attach_depth_stencil_target(Framebuffer* framebuffer, Texture* render_target)
{
	framebuffer->depth_target = render_target;
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));
	GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, framebuffer->id));
	GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, render_target->gl_texture_target, render_target->id, 0));
	GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, 0));
	GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer* Device::create_framebuffer(FramebufferCreateDesc desc)
{
	Framebuffer* framebuffer = new Framebuffer();

	GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer->id));

	for (int i = 0; i < desc.num_render_targets; i++)
		attach_render_target(framebuffer, desc.render_targets[i]);

	if (desc.depth_target)
		attach_depth_stencil_target(framebuffer, desc.depth_target);

	return framebuffer;
}


VertexBuffer* Device::create_vertex_buffer(BufferCreateDesc desc)
{
	VertexBuffer* buffer = new VertexBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = g_buffer_usage_type_table[desc.usage_type];

	buffer->buffer_type = GL_ARRAY_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;

	return buffer;
}

UniformBuffer* Device::create_uniform_buffer(BufferCreateDesc desc)
{
	UniformBuffer* buffer = new UniformBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = g_buffer_usage_type_table[desc.usage_type];

	GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, buffer->id));
	GL_CHECK_ERROR(glBufferData(GL_UNIFORM_BUFFER, desc.size, desc.data, glusageType));
	GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	buffer->buffer_type = GL_UNIFORM_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;

	return buffer;
}

IndexBuffer* Device::create_index_buffer(BufferCreateDesc desc)
{
	IndexBuffer* buffer = new IndexBuffer();
	GL_CHECK_ERROR(glGenBuffers(1, &buffer->id));

	GLenum glusageType = g_buffer_usage_type_table[desc.usage_type];

	buffer->buffer_type = GL_ELEMENT_ARRAY_BUFFER;
	buffer->data = desc.data;
	buffer->size = desc.size;
	buffer->usage_type = glusageType;
	buffer->type = g_buffer_data_type_table[desc.data_type];

	return buffer;
}

InputLayout* Device::create_input_layout(InputLayoutCreateDesc desc)
{
    InputLayout* ia  = new InputLayout();
    
    memcpy(&ia->elements[0], &desc.elements[0], sizeof(InputElement) * desc.num_elements);
    
    ia->num_elements = desc.num_elements;
    ia->vertex_size  = desc.vertex_size;
    
    return ia;
}

VertexArray* Device::create_vertex_array(VertexArrayCreateDesc desc)
{
	VertexArray* vertexArray = new VertexArray();
	vertexArray->ib = desc.index_buffer;
	vertexArray->vb = desc.vertex_buffer;

	GL_CHECK_ERROR(glGenVertexArrays(1, &vertexArray->id));
	GL_CHECK_ERROR(glBindVertexArray(vertexArray->id));

	GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, desc.vertex_buffer->id));
	GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, desc.vertex_buffer->size, desc.vertex_buffer->data, desc.vertex_buffer->usage_type));

	GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->id));
	GL_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->size, desc.index_buffer->data, desc.index_buffer->usage_type));
	
	for (uint32_t i = 0; i < desc.layout->num_elements; i++)
	{
		GL_CHECK_ERROR(glEnableVertexAttribArray(i));
		GL_CHECK_ERROR(glVertexAttribPointer(i, 
										     desc.layout->elements[i].num_sub_elements,
											 g_buffer_data_type_table[desc.layout->elements[i].type],
											 desc.layout->elements[i].normalized,
											 desc.layout->vertex_size,
											 (GLvoid*)(desc.layout->elements[i].offset)));
	}

	GL_CHECK_ERROR(glBindVertexArray(0));
	GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	return vertexArray;
}

Texture2D* Device::create_texture_2d(Texture2DCreateDesc desc)
{
	Texture2D* texture = new Texture2D();

	GL_CHECK_ERROR(glGenTextures(1, &texture->id));
	texture->gl_texture_target = GL_TEXTURE_2D;

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture->id));

	GLenum texformat, textype;

	texformat = g_texture_format_table[desc.format][0];
	textype = g_texture_format_table[desc.format][1];

	GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, texformat, desc.width, desc.height, 0, texformat, textype, desc.data));

	if (desc.generate_mipmaps)
	{
		GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
	}

	GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

	return texture;
}

RasterizerState* Device::create_rasterizer_state(RasterizerStateCreateDesc desc)
{
	RasterizerState* rasterizerState = new RasterizerState();

	rasterizerState->cull_face = g_cull_mode_table[desc.cull_mode];
	rasterizerState->enable_cull_face = desc.cull_mode != CullMode::NONE;
	rasterizerState->polygon_mode = g_fill_mode_table[desc.fill_mode];
	rasterizerState->enable_multisample = desc.multisample;
	rasterizerState->enable_scissor = desc.scissor;
	rasterizerState->enable_front_face_ccw = desc.front_winding_ccw;

	return rasterizerState;
}

DepthStencilState* Device::create_depth_stencil_state(DepthStencilStateCreateDesc desc)
{
	DepthStencilState* depthStencilState = new DepthStencilState();

	// Set Depth Options

	depthStencilState->enable_depth = desc.enable_depth_test;
	depthStencilState->depth_func = g_comparison_function_table[desc.depth_cmp_func];
	depthStencilState->depth_mask = (desc.depth_mask) ? GL_TRUE : GL_FALSE;

	// Set Stencil Options

	depthStencilState->enable_stencil = desc.enable_stencil_test;
	depthStencilState->front_stencil_comparison = g_comparison_function_table[desc.front_stencil_cmp_func];
	depthStencilState->back_stencil_comparison = g_comparison_function_table[desc.back_stencil_cmp_func];

	// Front Stencil Operation

	depthStencilState->front_stencil_fail = g_stencil_operation_table[desc.front_stencil_fail];
	depthStencilState->front_stencil_pass_depth_pass = g_stencil_operation_table[desc.front_stencil_pass_depth_pass];
	depthStencilState->front_stencil_pass_depth_fail = g_stencil_operation_table[desc.front_stencil_pass_depth_fail];

	// Back Stencil Operation

	depthStencilState->back_stencil_fail = g_stencil_operation_table[desc.back_stencil_fail];
	depthStencilState->back_stencil_pass_depth_pass = g_stencil_operation_table[desc.back_stencil_pass_depth_pass];
	depthStencilState->back_stencil_pass_depth_fail = g_stencil_operation_table[desc.back_stencil_pass_depth_fail];
	depthStencilState->stencil_mask = desc.stencil_mask;

	return depthStencilState;
}

PipelineStateObject* Device::create_pipeline_state_object(PipelineStateObjectCreateDesc desc)
{
	PipelineStateObject* pso = new PipelineStateObject();

	pso->depth_stencil_state = create_depth_stencil_state(desc.depth_stencil_state);
	pso->rasterizer_state = create_rasterizer_state(desc.rasterizer_state);
	pso->primitive = desc.primitive;

	return pso;
}


SamplerState* Device::create_sampler_state(SamplerStateCreateDesc desc)
{
	SamplerState* samplerState = new SamplerState();

	GL_CHECK_ERROR(glGenSamplers(1, &samplerState->id));

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_S, g_texture_wrap_mode_table[desc.wrap_mode_u]));

	if (desc.wrap_mode_u == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_T, g_texture_wrap_mode_table[desc.wrap_mode_v]));

	if (desc.wrap_mode_v == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}

	GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_R, g_texture_wrap_mode_table[desc.wrap_mode_w]));

	if (desc.wrap_mode_w == TextureWrapMode::CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { desc.border_color[0], desc.border_color[1], desc.border_color[2], desc.border_color[3] };
		GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
	}

	// Texture Filtering

	if (desc.min_filter == TextureFilteringMode::ANISOTROPIC_ALL || desc.mag_filter == TextureFilteringMode::LINEAR_ALL)
	{
		if (ogl_ext_EXT_texture_filter_anisotropic == ogl_LOAD_SUCCEEDED)
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
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}
	}
	else
	{
		GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MAG_FILTER, g_texture_mag_filtering_mode_table[desc.mag_filter]));
		GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MIN_FILTER, g_texture_min_filtering_mode_table[desc.min_filter]));
	}

	return samplerState;
}

void Device::destroy_shader(Shader* shader)
{
	if (shader)
	{
		glDeleteShader(shader->id);
		delete shader;
	}
}

void Device::destroy_shader_program(ShaderProgram* program)
{
	if (program)
	{
		glDeleteProgram(program->id);

		for (auto& shader : program->shader_map)
			destroy_shader(shader.second);

		delete program;
	}
}

void Device::destroy_vertex_buffer(VertexBuffer* vertex_buffer)
{
	if (vertex_buffer)
	{
		glDeleteBuffers(1, &vertex_buffer->id);
		delete vertex_buffer;
	}
}

void Device::destroy_index_buffer(IndexBuffer* index_buffer)
{
	if (index_buffer)
	{
		glDeleteBuffers(1, &index_buffer->id);
		delete index_buffer;
	}
}

void Device::destroy_uniform_buffer(UniformBuffer* buffer)
{
	glDeleteBuffers(1, &buffer->id);
	delete buffer;
}

void Device::destroy_vertex_array(VertexArray* vertex_array)
{
	if (vertex_array)
	{
		glDeleteVertexArrays(1, &vertex_array->id);
		glDeleteBuffers(1, &vertex_array->ib->id);
		glDeleteBuffers(1, &vertex_array->vb->id);
		delete vertex_array;
	}
}

void Device::destroy_texture(Texture* texture)
{
	if (texture)
	{
		glDeleteTextures(1, &texture->id);
		delete texture;
	}
}

void Device::destroy_rasterizer_state(RasterizerState* state)
{
	delete state;
}

void Device::destroy_sampler_state(SamplerState* state)
{
	GL_CHECK_ERROR(glDeleteSamplers(1, &state->id));
	delete state;
}

void Device::destroy_depth_stencil_state(DepthStencilState* state)
{
	delete state;
}

void Device::destroy_pipeline_state_object(PipelineStateObject* pso)
{
	destroy_depth_stencil_state(pso->depth_stencil_state);
	destroy_rasterizer_state(pso->rasterizer_state);

	delete pso;
}

void Device::destroy_framebuffer(Framebuffer* framebuffer)
{
	if (framebuffer)
	{
		glDeleteFramebuffers(1, &framebuffer->id);

		for (int i = 0; i < framebuffer->num_render_targets; i++)
			destroy_texture(framebuffer->render_targets[i]);

		if (framebuffer->depth_target)
			destroy_texture(framebuffer->depth_target);

		delete framebuffer;
	}
}

void Device::bind_pipeline_state_object(PipelineStateObject* pso)
{
	bind_depth_stencil_state(pso->depth_stencil_state);
	bind_rasterizer_state(pso->rasterizer_state);
	set_primitive_type(pso->primitive);
}

void Device::bind_texture(Texture* texture, uint32_t shader_stage, uint32_t buffer_slot)
{
	GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));
}

void Device::bind_uniform_buffer(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot)
{
	GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, buffer_slot, uniform_buffer->id));
}

void Device::bind_uniform_buffer_range(UniformBuffer* uniform_buffer, uint32_t shader_stage, uint32_t buffer_slot, size_t offset, size_t size)
{
	GL_CHECK_ERROR(glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, uniform_buffer->id, offset, size));
}

void Device::bind_vertex_array(VertexArray* vertex_array)
{
	m_device_data.current_index_buffer = vertex_array->ib;
	GL_CHECK_ERROR(glBindVertexArray(vertex_array->id));
}

void Device::bind_rasterizer_state(RasterizerState* state)
{
	if (state->enable_cull_face)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	GL_CHECK_ERROR(glCullFace(state->cull_face));

	GL_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, state->polygon_mode));

	if (state->enable_multisample)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);

	if (state->enable_scissor)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	if (state->enable_front_face_ccw)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
}

void Device::bind_sampler_state(SamplerState* state, uint32_t shader_stage, uint32_t slot)
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

void Device::bind_framebuffer(Framebuffer* framebuffer)
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

void Device::bind_depth_stencil_state(DepthStencilState* state)
{
	// Set Depth Options

	if (state->enable_depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glDepthFunc(state->depth_func);
	glDepthMask((state->depth_mask) ? GL_TRUE : GL_FALSE);

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

void Device::bind_shader_program(ShaderProgram* program)
{
	m_device_data.current_program = program;
	GL_CHECK_ERROR(glUseProgram(program->id));
}

void* Device::map_buffer(Buffer* buffer, uint32_t type)
{
	void* ptr = nullptr;
	GL_CHECK_ERROR(glBindBuffer(buffer->buffer_type, buffer->id));
	GL_CHECK_ERROR(ptr = glMapBuffer(buffer->buffer_type, g_map_type_table[type]));
	return ptr;
}

void Device::unmap_buffer(Buffer* buffer)
{
	GL_CHECK_ERROR(glUnmapBuffer(buffer->buffer_type));
	glBindBuffer(buffer->buffer_type, 0);
}

void Device::set_primitive_type(uint32_t primitive)
{
	m_device_data.primitive_type = g_draw_primitive_type_table[primitive];
}

void Device::clear_framebuffer(uint32_t clear_target, float* clear_color)
{
	GL_CHECK_ERROR(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));

	uint32_t bits = ((clear_target & ClearTarget::COLOR) == ClearTarget::COLOR) ? GL_COLOR_BUFFER_BIT   : 0;
	bits |= ((clear_target & ClearTarget::DEPTH) == ClearTarget::DEPTH)         ? GL_DEPTH_BUFFER_BIT   : 0;
	bits |= ((clear_target & ClearTarget::STENCIL) == ClearTarget::STENCIL)     ? GL_STENCIL_BUFFER_BIT : 0;

	GL_CHECK_ERROR(glClear(bits));
}

void Device::set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
{
	GL_CHECK_ERROR(glViewport(top_left_x, top_left_y, width, height));
}

void Device::draw(uint32_t first_index, uint32_t count)
{
	GL_CHECK_ERROR(glDrawArrays(m_device_data.primitive_type, first_index, count));
}

void Device::draw_indexed(uint32_t index_count)
{
	GL_CHECK_ERROR(glDrawElements(m_device_data.primitive_type,
								  index_count, 
								  ((m_device_data.current_index_buffer) ? m_device_data.current_index_buffer->type : GL_UNSIGNED_INT),
								  0));
}

void Device::draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
{
	GL_CHECK_ERROR(glDrawElementsBaseVertex(m_device_data.primitive_type,
											index_count, 
											((m_device_data.current_index_buffer) ? m_device_data.current_index_buffer->type : GL_UNSIGNED_INT), 
											(void*)(sizeof(unsigned int) * base_index), 
											base_vertex));
}

GFX_END_NAMESPACE
