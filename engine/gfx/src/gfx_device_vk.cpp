#include "render_device.hpp"
#include "filesystem.hpp"
#include "string_utility.h"

PROTOGFX_BEGIN_NAMESPACE

namespace render_device
{
	GLenum		   m_primitive_type;
	ShaderProgram* m_current_program;
	GLuint		   m_last_sampler_location;
	IndexBuffer*   m_current_index_buffer = nullptr;

	const GLenum g_texture_format_table[14][2] =
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

	const GLenum g_shader_type_table[6] = 
	{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER,
		GL_COMPUTE_SHADER
	};

	const GLenum g_buffer_usage_type_table[3] =
	{
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW,
		GL_STREAM_DRAW
	};

	const GLenum g_map_type_table[3] =
	{
		GL_READ_ONLY,
		GL_WRITE_ONLY,
		GL_READ_WRITE
	};

	const GLenum g_buffer_data_type_table[7] =
	{
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_INT,
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_INT,
		GL_FLOAT
	};

	const GLenum g_cull_mode_table[3] =
	{
		GL_FRONT,
		GL_BACK,
		GL_FRONT_AND_BACK
	};

	const GLenum g_fill_mode_table[2] =
	{
		GL_FILL,
		GL_LINE
	};

	const GLenum g_comparison_function_table[8] =
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

	const GLenum g_stencil_operation_table[8] =
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

	const GLenum g_texture_wrap_mode_table[4] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER
	};

	const GLenum g_texture_mag_filtering_mode_table[5] =
	{
		GL_LINEAR,
		GL_NEAREST,
		GL_TEXTURE_MAX_ANISOTROPY_EXT,
		GL_LINEAR,
		GL_NEAREST
	};

	const GLenum g_texture_min_filtering_mode_table[5] =
	{
		GL_LINEAR_MIPMAP_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_TEXTURE_MAX_ANISOTROPY_EXT,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR
	};

	const GLenum g_draw_primitive_type_table[5] =
	{
		GL_POINTS,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_LINES,
		GL_LINE_STRIP
	};

	const GLenum g_clear_target_type_table[3] =
	{
		GL_COLOR_BUFFER_BIT,
		GL_DEPTH_BUFFER_BIT,
		GL_STENCIL_BUFFER_BIT
	};

	bool init()
	{
		if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
		{
			std::cout << "Failed to load functions" << std::endl;
			return false;
		}
		else
			return true;
	}

	Shader* create_shader(const char* source, ShaderType type)
	{
		Shader* shader = new Shader();
		shader->m_type = type;

		GL_CHECK_ERROR(shader->m_id = glCreateShader(g_shader_type_table[type]));

		GLint success;
		GLchar infoLog[512];

		shader->m_source = std::string(source);

		const GLchar* src = source;

		GL_CHECK_ERROR(glShaderSource(shader->m_id, 1, &src, NULL));
		GL_CHECK_ERROR(glCompileShader(shader->m_id));
		GL_CHECK_ERROR(glGetShaderiv(shader->m_id, GL_COMPILE_STATUS, &success));

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader->m_id, 512, NULL, infoLog);

			std::string log_error = "Shader compilation failed";
			log_error += std::string(infoLog);

			std::cout << log_error << std::endl;

			return nullptr;
		}

		std::cout << "Shader successfully compiled." << std::endl;

		return shader;
	}

	ShaderProgram* create_shader_program(Shader** shaders, uint32_t count)
	{
		ShaderProgram* shaderProgram = new ShaderProgram();
		GL_CHECK_ERROR(shaderProgram->m_id = glCreateProgram());

		bool is_compute = false;
		for (uint32_t i = 0; i < 6; i++)
		{
			if (count > 1 || shaders[i]->m_type == ShaderType::COMPUTE)
				return nullptr;

			GL_CHECK_ERROR(glAttachShader(shaderProgram->m_id, shaders[i]->m_id));
		}

		GL_CHECK_ERROR(glLinkProgram(shaderProgram->m_id));

		GLint success;
		char infoLog[512];

		glGetProgramiv(shaderProgram->m_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram->m_id, 512, NULL, infoLog);

			std::string log_error = "Shader program linking failed";
			log_error += std::string(infoLog);

			std::cout << log_error << std::endl;

			return nullptr;
		}

		// Bind Uniform Buffers
		for (auto it : shaderProgram->m_shader_map)
		{
			StringList uboList = StringUtility::find_line("#binding", it.second->m_source);

			for (int i = 0; i < uboList.size(); i++)
			{
				StringList tokens = StringUtility::delimit(" ", uboList[i]);
				std::string uniformName = tokens[3];
				GLuint binding = (GLuint)atoi(tokens[5].c_str());
				const GLchar* uniformNameChar = uniformName.c_str();
				GL_CHECK_ERROR(GLuint uboIndex = glGetUniformBlockIndex(shaderProgram->m_id, uniformNameChar));

				if (uboIndex == GL_INVALID_INDEX)
				{
					String uniform_error = "Failed to get Uniform Block Index for Uniform Buffer : ";
					uniform_error += uniformName;

					std::cout << uniform_error << std::endl;
				}
				else
					GL_CHECK_ERROR(glUniformBlockBinding(shaderProgram->m_id, uboIndex, binding));
			}
		}

		// Bind Uniform Samplers 
		for (auto it : shaderProgram->m_shader_map)
		{
			StringList samplerList = StringUtility::find_line("uniform sampler", it.second->m_source);

			for (int i = 0; i < samplerList.size(); i++)
			{
				StringList tokens = StringUtility::delimit(" ", samplerList[i]);
				std::string uniformName = tokens[2];
				uniformName.erase(uniformName.end() - 1, uniformName.end());

				GLuint binding = (GLuint)atoi(tokens[4].c_str());
				const GLchar* uniformNameChar = uniformName.c_str();
				GL_CHECK_ERROR(GLuint location = glGetUniformLocation(shaderProgram->m_id, uniformNameChar));

				if (location == GL_INVALID_INDEX)
				{
					String uniform_error = "Failed to get Uniform Location for Uniform : ";
					uniform_error += uniformName;

					std::cout << uniform_error << std::endl;
				}

				it.second->m_sampler_bindings[binding] = location;
			}
		}

		return shaderProgram;
	}

	void attach_render_target(Framebuffer* framebuffer, Texture* render_target)
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

	void attach_depth_stencil_target(Framebuffer* framebuffer, Texture* render_target)
	{
		framebuffer->depth_target = render_target;
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id));
		GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, framebuffer->id));
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, render_target->gl_texture_target, render_target->id, 0));
		GL_CHECK_ERROR(glBindTexture(render_target->gl_texture_target, 0));
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	Framebuffer* create_framebuffer(FramebufferCreateDesc desc)
	{
		Framebuffer* framebuffer = new Framebuffer();

		GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer->id));

		for (int i = 0; i < desc.num_render_targets; i++)
			attach_render_target(framebuffer, desc.render_targets[i]);

		if (desc.depth_target)
			attach_depth_stencil_target(framebuffer, desc.depth_target);

		return framebuffer;
	}


	VertexBuffer* create_vertex_buffer(BufferCreateDesc desc)
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

	UniformBuffer* create_uniform_buffer(BufferCreateDesc desc)
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

	IndexBuffer* create_index_buffer(BufferCreateDesc desc)
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

	VertexArray* create_vertex_array(VertexArrayCreateDesc desc)
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
		
		for (uint32_t i = 0; i < desc.layout.num_elements; i++)
		{
			GL_CHECK_ERROR(glEnableVertexAttribArray(i));
			GL_CHECK_ERROR(glVertexAttribPointer(i, 
											     desc.layout.elements[i].num_sub_elements, 
												 g_buffer_data_type_table[desc.layout.elements[i].type], 
												 desc.layout.elements[i].normalized, 
												 desc.layout.vertex_size, 
												 (GLvoid*)(desc.layout.elements[i].offset)));
		}

		GL_CHECK_ERROR(glBindVertexArray(0));
		GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		return vertexArray;
	}

	Texture2D* create_texture_2d(Texture2DCreateDesc desc)
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

	RasterizerState* create_rasterizer_state(RasterizerStateCreateDesc desc)
	{
		RasterizerState* rasterizerState = new RasterizerState();

		rasterizerState->m_cullFace = g_cull_mode_table[desc.cull_mode];
		rasterizerState->m_enableCullFace = desc.cull_mode != CullMode::NONE;
		rasterizerState->m_polygonMode = g_fill_mode_table[desc.fill_mode];
		rasterizerState->m_enableMultisample = desc.multisample;
		rasterizerState->m_enableScissor = desc.scissor;
		rasterizerState->m_enableFrontFaceCCW = desc.front_winding_ccw;

		return rasterizerState;
	}

	DepthStencilState* create_depth_stencil_state(DepthStencilStateCreateDesc desc)
	{
		DepthStencilState* depthStencilState = new DepthStencilState();

		// Set Depth Options

		depthStencilState->m_enableDepth = desc.enable_depth_test;
		depthStencilState->m_depthFunc = g_comparison_function_table[desc.depth_cmp_func];
		depthStencilState->m_depthMask = (desc.depth_mask) ? GL_TRUE : GL_FALSE;

		// Set Stencil Options

		depthStencilState->m_enableStencil = desc.enable_stencil_test;
		depthStencilState->m_frontStencilComparison = g_comparison_function_table[desc.front_stencil_cmp_func];
		depthStencilState->m_backStencilComparison = g_comparison_function_table[desc.back_stencil_cmp_func];

		// Front Stencil Operation

		depthStencilState->m_frontStencilFail = g_stencil_operation_table[desc.front_stencil_fail];
		depthStencilState->m_frontStencilPassDepthPass = g_stencil_operation_table[desc.front_stencil_pass_depth_pass];
		depthStencilState->m_frontStencilPassDepthFail = g_stencil_operation_table[desc.front_stencil_pass_depth_fail];
	
		// Back Stencil Operation

		depthStencilState->m_backStencilFail = g_stencil_operation_table[desc.back_stencil_fail];
		depthStencilState->m_backStencilPassDepthPass = g_stencil_operation_table[desc.back_stencil_pass_depth_pass];
		depthStencilState->m_backStencilPassDepthFail = g_stencil_operation_table[desc.back_stencil_pass_depth_fail];
		depthStencilState->m_stencilMask = desc.stencil_mask;

		return depthStencilState;
	}

	PipelineStateObject* create_pipeline_state_object(PipelineStateObjectCreateDesc desc)
	{
		PipelineStateObject* pso = new PipelineStateObject();

		pso->depth_stencil_state = create_depth_stencil_state(desc.depth_stencil_state);
		pso->rasterizer_state = create_rasterizer_state(desc.rasterizer_state);
		pso->primitive = desc.primitive;

		return pso;
	}

	
	SamplerState* create_sampler_state(SamplerStateCreateDesc desc)
	{
		SamplerState* samplerState = new SamplerState();

		GL_CHECK_ERROR(glGenSamplers(1, &samplerState->id));

		GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_S, g_texture_wrap_mode_table[desc.wrap_mode_u]));

		if (desc.wrap_mode_u == TextureWrapMode::CLAMP_TO_BORDER)
		{
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
		}

		GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_T, g_texture_wrap_mode_table[desc.wrap_mode_v]));

		if (desc.wrap_mode_v == TextureWrapMode::CLAMP_TO_BORDER)
		{
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->id, GL_TEXTURE_BORDER_COLOR, borderColor));
		}

		GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_WRAP_R, g_texture_wrap_mode_table[desc.wrap_mode_w]));

		if (desc.wrap_mode_w == TextureWrapMode::CLAMP_TO_BORDER)
		{
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
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
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->id, GL_TEXTURE_MIN_FILTER, g_texture_mag_filtering_mode_table[desc.min_filter]));
		}

		return samplerState;
	}

	void destroy_shader(Shader* shader)
	{
		if (shader)
		{
			glDeleteShader(shader->m_id);
			delete shader;
		}
	}

	void destroy_shader_program(ShaderProgram* program)
	{
		if (program)
		{
			glDeleteProgram(program->m_id);

			for (auto& shader : program->m_shader_map)
				destroy_shader(shader.second);

			delete program;
		}
	}

	void destroy_vertex_buffer(VertexBuffer* vertex_buffer)
	{
		if (vertex_buffer)
		{
			glDeleteBuffers(1, &vertex_buffer->id);
			delete vertex_buffer;
		}
	}

	void destroy_index_buffer(IndexBuffer* index_buffer)
	{
		if (index_buffer)
		{
			glDeleteBuffers(1, &index_buffer->id);
			delete index_buffer;
		}
	}

	void destroy_uniform_buffer(UniformBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->id);
		delete buffer;
	}

	void destroy_vertex_array(VertexArray* vertex_array)
	{
		if (vertex_array)
		{
			glDeleteVertexArrays(1, &vertex_array->id);
			glDeleteBuffers(1, &vertex_array->ib->id);
			glDeleteBuffers(1, &vertex_array->vb->id);
			delete vertex_array;
		}
	}

	void destroy_texture(Texture* texture)
	{
		if (texture)
		{
			glDeleteTextures(1, &texture->id);
			delete texture;
		}
	}

	void destroy_rasterizer_state(RasterizerState* state)
	{
		delete state;
	}

	void destroy_sampler_state(SamplerState* state)
	{
		GL_CHECK_ERROR(glDeleteSamplers(1, &state->id));
		delete state;
	}

	void destroy_depth_stencil_state(DepthStencilState* state)
	{
		delete state;
	}

	void destroy_pipeline_state_object(PipelineStateObject* pso)
	{
		destroy_depth_stencil_state(pso->depth_stencil_state);
		destroy_rasterizer_state(pso->rasterizer_state);

		delete pso;
	}

	void destroy_framebuffer(Framebuffer* framebuffer)
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

	void bind_pipeline_state_object(PipelineStateObject* pso)
	{
		bind_depth_stencil_state(pso->depth_stencil_state);
		bind_rasterizer_state(pso->rasterizer_state);
		set_primitive_type(pso->primitive);
	}

	void bind_texture(Texture* texture, ShaderType shaderStage, uint32_t bufferSlot)
	{
		GL_CHECK_ERROR(glBindTexture(texture->gl_texture_target, texture->id));
	}

	void bind_uniform_buffer(UniformBuffer* uniformBuffer, ShaderType shaderStage, uint32_t bufferSlot)
	{
		GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, bufferSlot, uniformBuffer->id));
	}

	void bind_uniform_buffer_range(UniformBuffer* uniformBuffer, ShaderType shaderStage, uint32_t bufferSlot, size_t offset, size_t size)
	{
		GL_CHECK_ERROR(glBindBufferRange(GL_UNIFORM_BUFFER, bufferSlot, uniformBuffer->id, offset, size));
	}

	void bind_vertex_array(VertexArray* vertexArray)
	{
		m_current_index_buffer = vertexArray->ib;
		GL_CHECK_ERROR(glBindVertexArray(vertexArray->id));
	}

	void bind_rasterizer_state(RasterizerState* state)
	{
		if (state->m_enableCullFace)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		GL_CHECK_ERROR(glCullFace(state->m_cullFace));

		GL_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, state->m_polygonMode));

		if (state->m_enableMultisample)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);

		if (state->m_enableScissor)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);

		if (state->m_enableFrontFaceCCW)
			glFrontFace(GL_CCW);
		else
			glFrontFace(GL_CW);
	}

	void bind_sampler_state(SamplerState* state, ShaderType shaderStage, uint32_t slot)
	{
		GLuint location = m_current_program->m_shader_map[shaderStage]->m_sampler_bindings[slot];
		m_last_sampler_location = location;

		if (location != GL_INVALID_INDEX)
		{
			GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + slot));
			GL_CHECK_ERROR(glBindSampler(slot, state->id));
			GL_CHECK_ERROR(glUniform1i(m_current_program->m_shader_map[shaderStage]->m_sampler_bindings[slot], slot));
		}
	}

	void bind_framebuffer(Framebuffer* framebuffer)
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

	void bind_depth_stencil_state(DepthStencilState* state)
	{
		// Set Depth Options

		if (state->m_enableDepth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthFunc(state->m_depthFunc);
		glDepthMask((state->m_depthMask) ? GL_TRUE : GL_FALSE);

		// Set Stencil Options

		if (state->m_enableStencil)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		GL_CHECK_ERROR(glStencilFuncSeparate(GL_FRONT, state->m_frontStencilComparison, 1, state->m_stencilMask));
		GL_CHECK_ERROR(glStencilFuncSeparate(GL_FRONT, state->m_backStencilComparison, 1, state->m_stencilMask));

		// Front Stencil Operation

		GL_CHECK_ERROR(glStencilOpSeparate(GL_FRONT, state->m_frontStencilFail, state->m_frontStencilPassDepthFail, state->m_frontStencilPassDepthPass));
		GL_CHECK_ERROR(glStencilOpSeparate(GL_BACK, state->m_backStencilFail, state->m_backStencilPassDepthFail, state->m_backStencilPassDepthPass));
	}

	void bind_shader_program(ShaderProgram* program)
	{
		m_current_program = program;
		GL_CHECK_ERROR(glUseProgram(program->m_id));
	}

	void* map_buffer(Buffer* buffer, BufferMapType type)
	{
		void* bufferPointer = nullptr;
		GL_CHECK_ERROR(glBindBuffer(buffer->buffer_type, buffer->id));
		GL_CHECK_ERROR(bufferPointer = glMapBuffer(buffer->buffer_type, g_map_type_table[type]));
		return bufferPointer;
	}

	void unmap_buffer(Buffer* buffer)
	{
		GL_CHECK_ERROR(glUnmapBuffer(buffer->buffer_type));
		glBindBuffer(buffer->buffer_type, 0);
	}

	void set_primitive_type(DrawPrimitive _primitive)
	{
		m_primitive_type = g_draw_primitive_type_table[_primitive];
	}

	void clear_framebuffer(uint32_t clearTarget, Vector4 clearColor)
	{
		GL_CHECK_ERROR(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));

		uint32_t bits = ((clearTarget & FramebufferClearTarget::COLOR) == FramebufferClearTarget::COLOR) ? g_clear_target_type_table[FramebufferClearTarget::COLOR] : 0;
		bits |= ((clearTarget & FramebufferClearTarget::DEPTH) == FramebufferClearTarget::DEPTH) ? g_clear_target_type_table[FramebufferClearTarget::DEPTH] : 0;
		bits |= ((clearTarget & FramebufferClearTarget::STENCIL) == FramebufferClearTarget::STENCIL) ? g_clear_target_type_table[FramebufferClearTarget::STENCIL] : 0;

		GL_CHECK_ERROR(glClear(bits));
	}

	void set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
	{
		GL_CHECK_ERROR(glViewport(top_left_x, top_left_y, width, height));
	}

	void draw(uint32_t first_index, uint32_t count)
	{
		GL_CHECK_ERROR(glDrawArrays(m_primitive_type, first_index, count));
	}

	void draw_indexed(uint32_t index_count)
	{
		GL_CHECK_ERROR(glDrawElements(m_primitive_type, 
									  index_count, 
									  ((m_current_index_buffer) ? m_current_index_buffer->type : GL_UNSIGNED_INT), 
									  0));
	}

	void draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
	{
		GL_CHECK_ERROR(glDrawElementsBaseVertex(m_primitive_type, 
												index_count, 
												((m_current_index_buffer) ? m_current_index_buffer->type : GL_UNSIGNED_INT), 
												(void*)(sizeof(unsigned int) * base_index), 
												base_vertex));
	}
}

PROTOGFX_END_NAMESPACE