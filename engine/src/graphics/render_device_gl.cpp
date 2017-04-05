#include <graphics/render_device_gl.h>
#include <core/context.h>
#include <utility/string_utility.h>
#include <utility/profiler.h>
#include <iostream>

#if defined(TERMINUS_OPENGL)

namespace terminus
{
	RenderDevice::RenderDevice()
	{

	}

	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::initialize()
	{
		_platform = context::get_platform();
		_platform->create_opengl_context();

#if defined(TERMINUS_PROFILING)
		rmt_BindOpenGL();
#endif
        
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "ERROR" << std::endl;
        }
        
        GL_CHECK_ERROR();
        
        _texture_res_id = 0;
        _buffer_res_id = 0;
        _framebuffer_res_id = 0;
        _vertex_array_res_id = 0;
        _shader_program_res_id = 0;
        _sampler_res_id = 0;
		_last_sampler_location = 0;

		_width = _platform->get_width();
		_height = _platform->get_height();
		_drawable_width = _platform->get_drawable_width();
		_drawable_height = _platform->get_drawable_height();
    }

	void RenderDevice::shutdown()
	{
#if defined(TERMINUS_PROFILING)
		rmt_UnbindOpenGL();
#endif
		_platform->destroy_opengl_context();
	}
    
    Texture1D* RenderDevice::create_texture_1d(Texture1DCreateDesc desc)
	{
		Texture1D* texture = new Texture1D();
        texture->m_resource_id = _texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_1D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_1D, texture->m_id));

		GLenum texformat, textype;

        texformat = g_texture_formats[desc.format][0];
        textype = g_texture_formats[desc.format][1];

		GL_CHECK_ERROR(glTexImage1D(GL_TEXTURE_1D, 0, texformat, desc.width, 0, texformat, textype, desc.data));

		if (desc.generate_mipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_1D));
		}
		
		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_1D, 0));

		return texture;
	}

	Texture2D* RenderDevice::create_texture_2d(Texture2DCreateDesc desc)
	{
		Texture2D* texture = new Texture2D();
        texture->m_resource_id = _texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_2D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture->m_id));

		GLenum texformat, textype;

        texformat = g_texture_formats[desc.format][0];
        textype = g_texture_formats[desc.format][1];

		GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, texformat, desc.width, desc.height,  0, texformat, textype, desc.data));

		if (desc.generate_mipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
		}

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

		return texture;
	}

    Texture3D* RenderDevice::create_texture_3d(Texture3DCreateDesc desc)
	{
		Texture3D* texture = new Texture3D();
        texture->m_resource_id = _texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_3D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_3D, texture->m_id));

		GLenum texformat, textype;

        texformat = g_texture_formats[desc.format][0];
        textype = g_texture_formats[desc.format][1];

		//glTexImage3D(GL_TEXTURE_3D, 0, texformat, width, height, 0, texformat, textype, data);

		if (desc.generate_mipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
		}

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

		return texture;
	}

	TextureCube* RenderDevice::create_texture_cube(TextureCubeCreateDesc desc)
	{
        TextureCube* texture = new TextureCube();
        texture->m_resource_id = _texture_res_id++;
        
        GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
        texture->m_glTextureTarget = GL_TEXTURE_CUBE_MAP;
        
        GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_id));
        
        GLenum texformat = g_texture_formats[desc.format][0];
        GLenum textype = g_texture_formats[desc.format][1];
        
		// Array order [+X, –X, +Y, –Y, +Z, –Z]
		for (int i = 0; i < 6; i++)
		{
			GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
										0,
										texformat,
										desc.width,
										desc.height,
										0,
										texformat,
										textype,
										desc.data[i]);)
		}
        
        if (desc.generate_mipmaps)
        {
            GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
        }
        
        GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
        
        return texture;
	}

    VertexBuffer* RenderDevice::create_vertex_buffer(BufferCreateDesc desc)
	{
		VertexBuffer* buffer = new VertexBuffer();
        buffer->m_resource_id = _buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (desc.usage_type)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		buffer->bufferType = GL_ARRAY_BUFFER;
		buffer->Data = desc.data;
		buffer->Size = desc.size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	IndexBuffer* RenderDevice::create_index_buffer(BufferCreateDesc desc)
    {
		IndexBuffer* buffer = new IndexBuffer();
        buffer->m_resource_id = _buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (desc.usage_type)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		buffer->bufferType = GL_ELEMENT_ARRAY_BUFFER;
		buffer->Data = desc.data;
		buffer->Size = desc.size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	UniformBuffer* RenderDevice::create_uniform_buffer(BufferCreateDesc desc)
	{
		UniformBuffer* buffer = new UniformBuffer();
        buffer->m_resource_id = _buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (desc.usage_type)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, buffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_UNIFORM_BUFFER, desc.size, desc.data, glusageType));
		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER,0));

		buffer->bufferType = GL_UNIFORM_BUFFER;
		buffer->Data = desc.data;
		buffer->Size = desc.size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	VertexArray* RenderDevice::create_vertex_array(VertexArrayCreateDesc desc)
	{
		VertexArray* vertexArray = new VertexArray();
        vertexArray->m_resource_id = _vertex_array_res_id++;
        vertexArray->_ib = desc.index_buffer;
        vertexArray->_vb = desc.vertex_buffer;

		GL_CHECK_ERROR(glGenVertexArrays(1, &vertexArray->m_id));
		GL_CHECK_ERROR(glBindVertexArray(vertexArray->m_id));

		GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, desc.vertex_buffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, desc.vertex_buffer->Size, desc.vertex_buffer->Data, desc.vertex_buffer->UsageType));

		GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.index_buffer->Size, desc.index_buffer->Data, desc.index_buffer->UsageType));

		if (desc.layout_type == InputLayoutType::STANDARD_VERTEX)
		{
			// Vertices

			GL_CHECK_ERROR(glEnableVertexAttribArray(0));
			GL_CHECK_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

			// Tex Coords

			GL_CHECK_ERROR(glEnableVertexAttribArray(1));
			GL_CHECK_ERROR(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TexCoord)));

			// Normals

			GL_CHECK_ERROR(glEnableVertexAttribArray(2));
			GL_CHECK_ERROR(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal)));

			// Tangents

			GL_CHECK_ERROR(glEnableVertexAttribArray(3));
			GL_CHECK_ERROR(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Tangent)));
		}
		else if (desc.layout_type == InputLayoutType::STANDARD_SKINNED_VERTEX)
		{
			// Vertices

			GL_CHECK_ERROR(glEnableVertexAttribArray(0));
			GL_CHECK_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)0));

			// Tex Coords

			GL_CHECK_ERROR(glEnableVertexAttribArray(1));
			GL_CHECK_ERROR(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_TexCoord)));

			// Normals

			GL_CHECK_ERROR(glEnableVertexAttribArray(2));
			GL_CHECK_ERROR(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_Normal)));

			// Tangents

			GL_CHECK_ERROR(glEnableVertexAttribArray(3));
			GL_CHECK_ERROR(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_Tangent)));

			// Bone Indices

			GL_CHECK_ERROR(glEnableVertexAttribArray(4));
			GL_CHECK_ERROR(glVertexAttribIPointer(4, 4, GL_INT, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_BoneIndices)));

			// Bone Weights

			GL_CHECK_ERROR(glEnableVertexAttribArray(5));
			GL_CHECK_ERROR(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)(offsetof(SkeletalVertex, m_BoneWeights))));
		}
		else if (desc.layout_type == InputLayoutType::CUSTOM_VERTEX)
		{
			for (int i = 0; i < desc.layout->m_Elements.size(); i++)
			{
				GL_CHECK_ERROR(glEnableVertexAttribArray(i));

				GLenum dataType = 0;

				switch (desc.layout->m_Elements[i].m_type)
				{
				case BufferDataType::FLOAT:
					dataType = GL_FLOAT;
					break;

				case BufferDataType::INT:
					dataType = GL_INT;
					break;

				case BufferDataType::UINT:
					break;
				}

				GL_CHECK_ERROR(glVertexAttribPointer(i,
                                                     desc.layout->m_Elements[i].m_numSubElements,
                                                     dataType, desc.layout->m_Elements[i].m_isNormalized,
                                                     desc.layout->m_vertexSize, (GLvoid*)0));
			}
		}

        GL_CHECK_ERROR(glBindVertexArray(0));
		GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		return vertexArray;
	}

    RasterizerState* RenderDevice::create_rasterizer_state(RasterizerStateCreateDesc desc)
	{
		RasterizerState* rasterizerState = new RasterizerState();
        
		switch (desc.cull_mode)
		{
		case CullMode::FRONT:
			rasterizerState->m_cullFace = GL_FRONT;
			rasterizerState->m_enableCullFace = true;
			break;

		case CullMode::BACK:
			rasterizerState->m_cullFace = GL_BACK;
			rasterizerState->m_enableCullFace = true;
			break;

		case CullMode::NONE:
			rasterizerState->m_cullFace = GL_BACK;
			rasterizerState->m_enableCullFace = false;
			break;

		default:
			rasterizerState->m_cullFace = GL_BACK;
			rasterizerState->m_enableCullFace = true;
			break;
		}

		switch (desc.fill_mode)
		{
		case FillMode::SOLID:
			rasterizerState->m_polygonMode = GL_FILL;
			break;

		case FillMode::WIREFRAME:
			rasterizerState->m_polygonMode = GL_LINE;
			break;

		default:
			rasterizerState->m_polygonMode = GL_FILL;
			break;
		}

		if (desc.multisample)
			rasterizerState->m_enableMultisample = true;
		else
			rasterizerState->m_enableMultisample = false;

		if (desc.scissor)
			rasterizerState->m_enableScissor = true;
		else
			rasterizerState->m_enableScissor = false;

		if (desc.front_winding_ccw)
			rasterizerState->m_enableFrontFaceCCW = true;
		else
			rasterizerState->m_enableFrontFaceCCW = false;

		return rasterizerState;
	}

    SamplerState* RenderDevice::create_sampler_state(SamplerStateCreateDesc desc)
	{
		SamplerState* samplerState = new SamplerState();
        samplerState->m_resource_id = _sampler_res_id++;

		GL_CHECK_ERROR(glGenSamplers(1, &samplerState->m_id));

		switch (desc.wrap_mode_u)
		{
		case TextureWrapMode::REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_REPEAT));
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		switch (desc.wrap_mode_v)
		{
		case TextureWrapMode::REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_REPEAT));
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		switch (desc.wrap_mode_w)
		{
		case TextureWrapMode::REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_REPEAT));
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT));
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
			GLfloat borderColor[] = { desc.border_color.x, desc.border_color.y, desc.border_color.z, desc.border_color.w };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		if (desc.min_filter == TextureFilteringMode::LINEAR_ALL && desc.mag_filter == TextureFilteringMode::LINEAR_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}
		else if (desc.min_filter == TextureFilteringMode::LINEAR_ALL && desc.mag_filter == TextureFilteringMode::NEAREST_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}
		else if (desc.min_filter == TextureFilteringMode::NEAREST_ALL && desc.mag_filter == TextureFilteringMode::LINEAR_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		}
		else if (desc.min_filter == TextureFilteringMode::NEAREST_ALL && desc.mag_filter == TextureFilteringMode::NEAREST_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		}
		else if (desc.min_filter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST && desc.mag_filter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		}
		else if (desc.min_filter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR && desc.mag_filter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
		}
		else
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}

		if (GLEW_EXT_texture_filter_anisotropic)
		{
			GLfloat glmaxAnisotropy;
			GL_CHECK_ERROR(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glmaxAnisotropy));

			if (desc.max_anisotropy <= glmaxAnisotropy)
			{
				GL_CHECK_ERROR(glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc.max_anisotropy));
			}
			else
			{
				GL_CHECK_ERROR(glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, glmaxAnisotropy));
			}
		}

		return samplerState;
	}

    Framebuffer* RenderDevice::create_framebuffer(FramebufferCreateDesc desc)
	{
		Framebuffer* framebuffer = new Framebuffer();
        framebuffer->m_resource_id = _framebuffer_res_id++;

		GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer->m_id));

        for(int i = 0; i < desc.num_render_targets; i++)
            attach_render_target(framebuffer, desc.render_targets[i]);
        
        if(desc.depth_target)
            attach_depth_stencil_target(framebuffer, desc.depth_target);
        
		return framebuffer;
	}

    Shader* RenderDevice::create_shader(ShaderCreateDesc desc)
	{
		Shader* shader = new Shader();
		shader->m_type  = desc.type;

		switch (desc.type)
		{
		case ShaderType::VERTEX:
		{
			GL_CHECK_ERROR(shader->m_id = glCreateShader(GL_VERTEX_SHADER));
		}
			break;

		case ShaderType::GEOMETRY:
			shader->m_id = glCreateShader(GL_GEOMETRY_SHADER);
			break;

		case ShaderType::TESSELLATION_CONTROL:
			shader->m_id = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			shader->m_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;

		case ShaderType::PIXEL:
			shader->m_id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}

		GLint success;
		GLchar infoLog[512];

		shader->m_source = std::string(desc.shader_source);

		const GLchar* source = desc.shader_source;

		GL_CHECK_ERROR(glShaderSource(shader->m_id, 1, &source, NULL));
		GL_CHECK_ERROR(glCompileShader(shader->m_id));
		GL_CHECK_ERROR(glGetShaderiv(shader->m_id, GL_COMPILE_STATUS, &success));

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader->m_id, 512, NULL, infoLog);
            
            String log_error = "Shader compilation failed";
            log_error       += String(infoLog);
            
            T_LOG_ERROR(log_error);
            
			return nullptr;
		}

        T_LOG_INFO("Shader successfully compiled.");
        
		return shader;
	}
    
    ShaderProgram* RenderDevice::create_shader_program(ShaderProgramCreateDesc desc)
	{
		ShaderProgram* shaderProgram = new ShaderProgram();
        shaderProgram->m_resource_id = _shader_program_res_id++;

		GL_CHECK_ERROR(shaderProgram->m_id = glCreateProgram());

		shaderProgram->m_shader_map[ShaderType::VERTEX] = desc.vertex;
		shaderProgram->m_shader_map[ShaderType::PIXEL] = desc.pixel;

		if (desc.geometry)
			shaderProgram->m_shader_map[ShaderType::GEOMETRY] = desc.geometry;

		if (desc.tessellation_control && desc.tessellation_evaluation)
		{
			shaderProgram->m_shader_map[ShaderType::TESSELLATION_CONTROL] = desc.tessellation_control;
			shaderProgram->m_shader_map[ShaderType::TESSELLATION_EVALUATION] = desc.tessellation_evaluation;
		}

		for (auto it : shaderProgram->m_shader_map)
		{
			GL_CHECK_ERROR(glAttachShader(shaderProgram->m_id, it.second->m_id));
		}
		
		GL_CHECK_ERROR(glLinkProgram(shaderProgram->m_id));

		GLint success;
		char infoLog[512];

		glGetProgramiv(shaderProgram->m_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram->m_id, 512, NULL, infoLog);
            
            String log_error = "Shader program linking failed";
            log_error       += String(infoLog);
            
            T_LOG_ERROR(log_error);
            
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
                
                if(uboIndex == GL_INVALID_INDEX)
                {
                    String uniform_error = "Failed to get Uniform Block Index for Uniform Buffer : ";
                    uniform_error       += uniformName;
                    
                    T_LOG_ERROR(uniform_error);
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
                
                if(location == GL_INVALID_INDEX)
                {
                    String uniform_error = "Failed to get Uniform Location for Uniform : ";
                    uniform_error       += uniformName;
                    
                    T_LOG_ERROR(uniform_error);
                }
                
                it.second->m_sampler_bindings[binding] = location;
			}
		}
        
        T_LOG_INFO("Shader program successfully linked.");

		return shaderProgram;
	}
    
    DepthStencilState* RenderDevice::create_depth_stencil_state(DepthStencilStateCreateDesc desc)
	{
		DepthStencilState* depthStencilState = new DepthStencilState();

		// Set Depth Options

		if (desc.enable_depth_test)
			depthStencilState->m_enableDepth = true;
		else
			depthStencilState->m_enableDepth = false;

		switch (desc.depth_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_depthFunc = GL_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_depthFunc = GL_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_depthFunc = GL_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_depthFunc = GL_LEQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_depthFunc = GL_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_depthFunc = GL_NOTEQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_depthFunc = GL_GEQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_depthFunc = GL_ALWAYS;
			break;

		default:
			break;
		}

		depthStencilState->m_depthMask = (desc.depth_mask) ? GL_TRUE : GL_FALSE;

		// Set Stencil Options

		if (desc.enable_stencil_test)
			depthStencilState->m_enableStencil = true;
		else
			depthStencilState->m_enableStencil = false;

		switch (desc.front_stencil_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_frontStencilComparison = GL_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_frontStencilComparison = GL_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_frontStencilComparison = GL_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_frontStencilComparison = GL_LEQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_frontStencilComparison = GL_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_frontStencilComparison = GL_NOTEQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_frontStencilComparison = GL_GEQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_frontStencilComparison = GL_ALWAYS;
			break;

		default:
			depthStencilState->m_frontStencilComparison = GL_LESS;
			break;
		}

		switch (desc.back_stencil_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_backStencilComparison = GL_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_backStencilComparison = GL_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_backStencilComparison = GL_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_backStencilComparison = GL_LEQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_backStencilComparison = GL_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_backStencilComparison = GL_NOTEQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_backStencilComparison = GL_GEQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_backStencilComparison = GL_ALWAYS;
			break;

		default:
			depthStencilState->m_backStencilComparison = GL_LESS;
			break;
		}

		// Front Stencil Operation

		switch (desc.front_stencil_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_frontStencilFail = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_frontStencilFail = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_frontStencilFail = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_frontStencilFail = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_frontStencilFail = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_frontStencilFail = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_frontStencilFail = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_frontStencilFail = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_frontStencilFail = GL_KEEP;
			break;
		}

		switch (desc.front_stencil_pass_depth_pass)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_frontStencilPassDepthPass = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_frontStencilPassDepthPass = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_frontStencilPassDepthPass = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_frontStencilPassDepthPass = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_frontStencilPassDepthPass = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_frontStencilPassDepthPass = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_frontStencilPassDepthPass = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_frontStencilPassDepthPass = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_frontStencilPassDepthPass = GL_KEEP;
			break;
		}

		switch (desc.front_stencil_pass_depth_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_frontStencilPassDepthFail = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_frontStencilPassDepthFail = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_frontStencilPassDepthFail = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_frontStencilPassDepthFail = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_frontStencilPassDepthFail = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_frontStencilPassDepthFail = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_frontStencilPassDepthFail = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_frontStencilPassDepthFail = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_frontStencilPassDepthFail = GL_KEEP;
			break;
		}

		// Back Stencil Operation

		switch (desc.back_stencil_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_backStencilFail = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_backStencilFail = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_backStencilFail = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_backStencilFail = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_backStencilFail = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_backStencilFail = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_backStencilFail = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_backStencilFail = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_backStencilFail = GL_KEEP;
			break;
		}

		switch (desc.back_stencil_pass_depth_pass)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_backStencilPassDepthPass = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_backStencilPassDepthPass = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_backStencilPassDepthPass = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_backStencilPassDepthPass = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_backStencilPassDepthPass = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_backStencilPassDepthPass = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_backStencilPassDepthPass = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_backStencilPassDepthPass = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_backStencilPassDepthPass = GL_KEEP;
			break;
		}

		switch (desc.back_stencil_pass_depth_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_backStencilPassDepthFail = GL_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_backStencilPassDepthFail = GL_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_backStencilPassDepthFail = GL_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_backStencilPassDepthFail = GL_INCR;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_backStencilPassDepthFail = GL_DECR;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_backStencilPassDepthFail = GL_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_backStencilPassDepthFail = GL_INCR_WRAP;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_backStencilPassDepthFail = GL_DECR_WRAP;
			break;

		default:
			depthStencilState->m_backStencilPassDepthFail = GL_KEEP;
			break;
		}

		return depthStencilState;
	}

	void RenderDevice::attach_render_target(Framebuffer* framebuffer, Texture* renderTarget)
	{
        framebuffer->_num_render_targets++;
		framebuffer->m_render_targets[framebuffer->_num_render_targets++] = renderTarget;

		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id));
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->_num_render_targets - 1), renderTarget->m_glTextureTarget, renderTarget->m_id, 0));

		std::vector<GLuint> drawBuffers;

		for (int i = 0; i < framebuffer->_num_render_targets; i++)
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

		GL_CHECK_ERROR(glDrawBuffers(framebuffer->_num_render_targets, &drawBuffers[0]));

		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, 0));
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void RenderDevice::attach_depth_stencil_target(Framebuffer* framebuffer, Texture* renderTarget)
	{
		framebuffer->m_depth_target = renderTarget;
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id));
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderTarget->m_glTextureTarget, renderTarget->m_id, 0));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, 0));
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
    
	void RenderDevice::destroy_texture_1d(Texture1D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::destroy_texture_2d(Texture2D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::destroy_texture_3d(Texture3D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::destroy_texture_cube(TextureCube* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::destroy_vertex_buffer(VertexBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::destroy_index_buffer(IndexBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::destroy_uniform_buffer(UniformBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::destroy_vertex_array(VertexArray* vertexArray)
	{
		if (vertexArray->_ib)
			destroy_index_buffer(vertexArray->_ib);

		if (vertexArray->_vb)
			destroy_vertex_buffer(vertexArray->_vb);

		GL_CHECK_ERROR(glDeleteVertexArrays(1, &vertexArray->m_id));
		delete vertexArray;
	}

	void RenderDevice::destroy_rasterizer_state(RasterizerState* state)
	{
		delete state;
	}

	void RenderDevice::destroy_sampler_state(SamplerState* state)
	{
		GL_CHECK_ERROR(glDeleteSamplers(1, &state->m_id));
		delete state;
	}

	void RenderDevice::destroy_depth_stencil_state(DepthStencilState* state)
	{
		delete state;
	}

	void RenderDevice::destroy_framebuffer(Framebuffer* framebuffer)
	{
		glDeleteFramebuffers(1, &framebuffer->m_id);
		delete framebuffer;
	}

	void RenderDevice::destroy_shader(Shader* shader)
	{
		delete shader;
	}

	void RenderDevice::destory_shader_program(ShaderProgram* program)
	{
		for (auto it : program->m_shader_map)
			delete it.second;

		glDeleteProgram(program->m_id);
		delete program;
	}

	void RenderDevice::bind_texture(Texture* texture,
								   ShaderType shaderStage,
								   uint bufferSlot)
	{
		if (_last_sampler_location != GL_INVALID_INDEX)
			GL_CHECK_ERROR(glBindTexture(texture->m_glTextureTarget, texture->m_id));
	}

	void RenderDevice::bind_uniform_buffer(UniformBuffer* uniformBuffer,
									     ShaderType shaderStage,
									     uint bufferSlot)
	{
		GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, bufferSlot, uniformBuffer->m_id));
	}

	void RenderDevice::bind_vertex_array(VertexArray* vertexArray)
	{
		glBindVertexArray(vertexArray->m_id);
	}

	void RenderDevice::bind_rasterizer_state(RasterizerState* state)
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

	void RenderDevice::bind_sampler_state(SamplerState* state,
										ShaderType shaderStage,
										uint slot)
	{
        GLuint location = _current_program->m_shader_map[shaderStage]->m_sampler_bindings[slot];
		_last_sampler_location = location;

        if(location != GL_INVALID_INDEX)
        {
            GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + slot));
            GL_CHECK_ERROR(glBindSampler(slot, state->m_id));
            GL_CHECK_ERROR(glUniform1i(_current_program->m_shader_map[shaderStage]->m_sampler_bindings[slot], slot));
        }
	}

	void RenderDevice::bind_framebuffer(Framebuffer* framebuffer)
	{
		if (framebuffer)
		{
			GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
		}
		else
		{
			GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}
	}

	void RenderDevice::bind_depth_stencil_state(DepthStencilState* state)
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

	void RenderDevice::bind_shader_program(ShaderProgram* program)
	{
		_current_program = program;
		GL_CHECK_ERROR(glUseProgram(program->m_id));
	}

	void* RenderDevice::map_buffer(Buffer* buffer, BufferMapType type)
	{
		void* bufferPointer;

		GL_CHECK_ERROR(glBindBuffer(buffer->bufferType, buffer->m_id));

		switch (type)
		{

		case BufferMapType::READ:
		{
			GL_CHECK_ERROR(bufferPointer = glMapBuffer(buffer->bufferType, GL_READ_ONLY));
			break;
		}

		case BufferMapType::WRITE:
		{
			GL_CHECK_ERROR(bufferPointer = glMapBuffer(buffer->bufferType, GL_WRITE_ONLY));
			break;
		}

		case BufferMapType::READ_WRITE:
		{
			GL_CHECK_ERROR(bufferPointer = glMapBuffer(buffer->bufferType, GL_READ_WRITE));
			break;
		}

		default:
			bufferPointer = nullptr;
			break;
		}

		return bufferPointer;
	}

	void RenderDevice::unmap_buffer(Buffer* buffer)
	{
		GL_CHECK_ERROR(glUnmapBuffer(buffer->bufferType));
		glBindBuffer(buffer->bufferType, 0);
	}

	void RenderDevice::set_primitive_type(DrawPrimitive _primitive)
	{
		switch (_primitive)
		{
		case DrawPrimitive::POINTS:
			_primitive_type = GL_POINTS;
			break;

		case DrawPrimitive::TRIANGLES:
			_primitive_type = GL_TRIANGLES;
			break;

		case DrawPrimitive::TRIANGLE_STRIP:
			_primitive_type = GL_TRIANGLE_STRIP;
			break;

		case DrawPrimitive::LINES:
			_primitive_type = GL_LINES;
			break;

		case DrawPrimitive::LINE_STRIP:
			_primitive_type = GL_LINE_STRIP;
			break;
		}
	}

	void RenderDevice::clear_framebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor)
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

		switch (clearTarget)
		{
		case FramebufferClearTarget::DEPTH:
			glClear(GL_DEPTH_BUFFER_BIT);
			break;

		case FramebufferClearTarget::ALL:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;
                
        case FramebufferClearTarget::COLOR:
            glClear(GL_COLOR_BUFFER_BIT);
            break;
		}
	}

	void RenderDevice::set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
	{
        // TODO : Cache pointer.
        if(width == 0 && height == 0)
        {
            glViewport(top_left_x,
                       top_left_y,
                       _drawable_width,
                       _drawable_height);
        }
		else
        {
            glViewport(top_left_x,
                       (_height - (height + top_left_y)),
                       width,
                       height);
        }
	}

	void RenderDevice::swap_buffers()
	{
		_platform->swap_buffers_opengl();
	}

    void RenderDevice::draw(uint32_t first_index, uint32_t count)
	{
		GL_CHECK_ERROR(glDrawArrays(_primitive_type, first_index, count));
	}

    void RenderDevice::draw_indexed(uint32_t index_count)
	{
		GL_CHECK_ERROR(glDrawElements(_primitive_type, index_count, GL_UNSIGNED_INT, 0));
	}

	void RenderDevice::draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
	{
		GL_CHECK_ERROR(glDrawElementsBaseVertex(_primitive_type, index_count, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * base_index), base_vertex));
	}

	void RenderDevice::draw_instanced()
	{


	}
    
	void RenderDevice::draw_indexed_instanced()
	{

	}
} // namespace terminus

#endif
