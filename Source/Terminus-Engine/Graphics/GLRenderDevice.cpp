#include "GLRenderDevice.h"
#include "../Utility/StringUtility.h"
#include <iostream>

#if defined(TERMINUS_OPENGL)

#ifdef TERMINUS_OPENGL
#ifdef __APPLE__
#define GL_MAX_MAJOR_VERSION 4
#define GL_MAX_MINOR_VERSION 1
#else
#define GL_MAX_MAJOR_VERSION 4
#define GL_MAX_MINOR_VERSION 5
#endif
#endif

namespace Terminus { namespace Graphics {

	RenderDevice::RenderDevice()
	{

	}

	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::Initialize()
	{
		m_window = PlatformBackend::GetWindow();
        
#ifdef __APPLE__
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAX_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MAX_MINOR_VERSION);
        
        m_gl_context = SDL_GL_CreateContext(m_window);
        SDL_GL_MakeCurrent(m_window, m_gl_context);
        
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "ERROR" << std::endl;
        }
        
        GL_CHECK_ERROR();
        
        m_texture_res_id = 0;
        m_buffer_res_id = 0;
        m_framebuffer_res_id = 0;
        m_vertex_array_res_id = 0;
        m_shader_program_res_id = 0;
    }

	void RenderDevice::Shutdown()
	{
        SDL_GL_DeleteContext(m_gl_context);
	}

	Texture1D* RenderDevice::CreateTexture1D(uint16 width,
											 void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture1D* texture = new Texture1D();
        texture->m_resource_id = m_texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_1D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_1D, texture->m_id));

		GLenum texformat, textype;

		switch (format)
		{
		case TextureFormat::R32G32B32_FLOAT:
		{
			texformat = GL_RGB;
			textype = GL_FLOAT;
		}
			break;
		case TextureFormat::R32G32B32A32_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
			break;
		case TextureFormat::R32G32B32_UINT:
		{
			texformat = GL_RGB;
			textype = GL_UNSIGNED_INT;
		}
			break;
		case TextureFormat::R32G32B32A32_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
			break;
		case TextureFormat::R32G32B32_INT:
		{
			texformat = GL_RGB;
			textype = GL_INT;
		}
			break;
		case TextureFormat::R32G32B32A32_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
			break;
		case TextureFormat::R16G16B16A16_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
			break;
		case TextureFormat::R16G16B16A16_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
			break;
		case TextureFormat::R16G16B16A16_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
			break;
		case TextureFormat::R8G8B8A8_UNORM:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_BYTE;
		}
			break;
		case TextureFormat::R8G8B8A8_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
			break;
		case TextureFormat::D32_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
			break;
		case TextureFormat::D24_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_UNSIGNED_INT_24_8;
		}
			break;
		case TextureFormat::D16_FLOAT:
		{
			texformat = GL_DEPTH_COMPONENT;
			textype = GL_FLOAT;
		}
			break;
		default:
			break;
		}

		GL_CHECK_ERROR(glTexImage1D(GL_TEXTURE_1D, 0, texformat, width, 0, texformat, textype, data));

		if (generateMipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_1D));
		}
		
		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_1D, 0));

		return texture;
	}

	Texture2D* RenderDevice::CreateTexture2D(uint16 width,
											 uint16 height,
											 void* data,
											 TextureFormat format,
											 bool createRenderTargetView,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture2D* texture = new Texture2D();
        texture->m_resource_id = m_texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_2D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, texture->m_id));

		GLenum texformat, textype;

		switch (format)
		{
		case TextureFormat::R32G32B32_FLOAT:
		{
			texformat = GL_RGB;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R32G32B32A32_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R32G32B32_UINT:
		{
			texformat = GL_RGB;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R32G32B32A32_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R32G32B32_INT:
		{
			texformat = GL_RGB;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R32G32B32A32_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R16G16B16A16_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R16G16B16A16_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R16G16B16A16_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R8G8B8A8_UNORM:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_BYTE;
		}
		break;
		case TextureFormat::R8G8B8A8_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::D32_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
		break;
		case TextureFormat::D24_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_UNSIGNED_INT_24_8;
		}
		break;
		case TextureFormat::D16_FLOAT:
		{
			texformat = GL_DEPTH_COMPONENT;
			textype = GL_FLOAT;
		}
		break;
		default:
			break;
		}

		GL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, texformat, width, height,  0, texformat, textype, data));

		if (generateMipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
		}

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

		return texture;
	}

	Texture3D* RenderDevice::CreateTexture3D(uint16 width,
											 uint16 height,
											 uint16 depth,
											 void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture3D* texture = new Texture3D();
        texture->m_resource_id = m_texture_res_id++;

		GL_CHECK_ERROR(glGenTextures(1, &texture->m_id));
		texture->m_glTextureTarget = GL_TEXTURE_3D;

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_3D, texture->m_id));

		GLenum texformat, textype;

		switch (format)
		{
		case TextureFormat::R32G32B32_FLOAT:
		{
			texformat = GL_RGB;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R32G32B32A32_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R32G32B32_UINT:
		{
			texformat = GL_RGB;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R32G32B32A32_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R32G32B32_INT:
		{
			texformat = GL_RGB;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R32G32B32A32_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R16G16B16A16_FLOAT:
		{
			texformat = GL_RGBA;
			textype = GL_FLOAT;
		}
		break;
		case TextureFormat::R16G16B16A16_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::R16G16B16A16_INT:
		{
			texformat = GL_RGBA;
			textype = GL_INT;
		}
		break;
		case TextureFormat::R8G8B8A8_UNORM:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_BYTE;
		}
		break;
		case TextureFormat::R8G8B8A8_UINT:
		{
			texformat = GL_RGBA;
			textype = GL_UNSIGNED_INT;
		}
		break;
		case TextureFormat::D32_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
		break;
		case TextureFormat::D24_FLOAT_S8_UINT:
		{
			texformat = GL_DEPTH_STENCIL;
			textype = GL_UNSIGNED_INT_24_8;
		}
		break;
		case TextureFormat::D16_FLOAT:
		{
			texformat = GL_DEPTH_COMPONENT;
			textype = GL_FLOAT;
		}
		break;
		default:
			break;
		}

		//glTexImage3D(GL_TEXTURE_3D, 0, texformat, width, height, 0, texformat, textype, data);

		if (generateMipmaps)
		{
			GL_CHECK_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
		}

		GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, 0));

		return texture;
	}

	TextureCube* RenderDevice::CreateTextureCube(uint16 width,
												 uint16 height,
												 uint16 depth, 
												 void* data,
												 TextureFormat format,
												 bool generateMipmaps,
												 uint mipMapLevels)
	{
		return nullptr;
	}

	VertexBuffer* RenderDevice::CreateVertexBuffer(void* data,
												   uint size,
												   BufferUsageType usageType)
	{
		VertexBuffer* buffer = new VertexBuffer();
        buffer->m_resource_id = m_buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		buffer->bufferType = GL_ARRAY_BUFFER;
		buffer->Data = data;
		buffer->Size = size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	IndexBuffer* RenderDevice::CreateIndexBuffer(void* data,
												 uint size,
												 BufferUsageType usageType)
	{
		IndexBuffer* buffer = new IndexBuffer();
        buffer->m_resource_id = m_buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		buffer->bufferType = GL_ELEMENT_ARRAY_BUFFER;
		buffer->Data = data;
		buffer->Size = size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	UniformBuffer* RenderDevice::CreateUniformBuffer(void* data,
													 uint size,
													 BufferUsageType usageType)
	{
		UniformBuffer* buffer = new UniformBuffer();
        buffer->m_resource_id = m_buffer_res_id++;
		GL_CHECK_ERROR(glGenBuffers(1, &buffer->m_id));

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, buffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_UNIFORM_BUFFER, size, data, glusageType));
		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER,0));

		buffer->bufferType = GL_UNIFORM_BUFFER;
		buffer->Data = data;
		buffer->Size = size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	VertexArray* RenderDevice::CreateVertexArray(VertexBuffer* vertexBuffer,
												 IndexBuffer* indexBuffer,
												 InputLayoutType layoutType,
												 InputLayout* layout)
	{
		VertexArray* vertexArray = new VertexArray();
        vertexArray->m_resource_id = m_vertex_array_res_id++;

		GL_CHECK_ERROR(glGenVertexArrays(1, &vertexArray->m_id));
		GL_CHECK_ERROR(glBindVertexArray(vertexArray->m_id));

		GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, vertexBuffer->Size, vertexBuffer->Data, vertexBuffer->UsageType));

		GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_id));
		GL_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->Size, indexBuffer->Data, indexBuffer->UsageType));

		if (layoutType == InputLayoutType::STANDARD_VERTEX)
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
		else if (layoutType == InputLayoutType::STANDARD_SKINNED_VERTEX)
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
		else if (layoutType == InputLayoutType::CUSTOM_VERTEX)
		{
			for (int i = 0; i < layout->m_Elements.size(); i++)
			{
				GL_CHECK_ERROR(glEnableVertexAttribArray(i));

				GLenum dataType = 0;

				switch (layout->m_Elements[i].m_type)
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

				GL_CHECK_ERROR(glVertexAttribPointer(i, layout->m_Elements[i].m_numSubElements, dataType, layout->m_Elements[i].m_isNormalized, layout->m_vertexSize, (GLvoid*)0));
			}
		}

		GL_CHECK_ERROR(glBindVertexArray(vertexArray->m_id));
		GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id));
		GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_id));

		return vertexArray;
	}

	RasterizerState* RenderDevice::CreateRasterizerState(CullMode cullMode,
														 FillMode fillMode,
														 bool frontWindingCCW,
														 bool multisample,
														 bool scissor)
	{
		RasterizerState* rasterizerState = new RasterizerState();

		switch (cullMode)
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

		switch (fillMode)
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

		if (multisample)
			rasterizerState->m_enableMultisample = true;
		else
			rasterizerState->m_enableMultisample = false;

		if (scissor)
			rasterizerState->m_enableScissor = true;
		else
			rasterizerState->m_enableScissor = false;

		if (frontWindingCCW)
			rasterizerState->m_enableFrontFaceCCW = true;
		else
			rasterizerState->m_enableFrontFaceCCW = false;

		return rasterizerState;
	}

	SamplerState* RenderDevice::CreateSamplerState(TextureFilteringMode minFilter,
												   TextureFilteringMode magFilter,
												   TextureWrapMode wrapModeU,
												   TextureWrapMode wrapModeV,
												   TextureWrapMode wrapModeW,
												   float maxAnisotropy,
												   float borderRed,
												   float borderGreen,
												   float borderBlue,
												   float borderAlpha)
	{
		SamplerState* samplerState = new SamplerState();

		GL_CHECK_ERROR(glGenSamplers(1, &samplerState->m_id));

		switch (wrapModeU)
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
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		switch (wrapModeV)
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
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		switch (wrapModeW)
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
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			GL_CHECK_ERROR(glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor));
			break;
		}

		if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}
		else if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		}
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		}
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		}
		else if (minFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST && magFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST)
		{
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK_ERROR(glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		}
		else if (minFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR && magFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR)
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

			if (maxAnisotropy <= glmaxAnisotropy)
			{
				GL_CHECK_ERROR(glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy));
			}
			else
			{
				GL_CHECK_ERROR(glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, glmaxAnisotropy));
			}
		}

		return samplerState;
	}

	Framebuffer* RenderDevice::CreateFramebuffer()
	{
		Framebuffer* framebuffer = new Framebuffer();
        framebuffer->m_resource_id = m_framebuffer_res_id++;

		GL_CHECK_ERROR(glGenFramebuffers(1, &framebuffer->m_id));

		return framebuffer;
	}

	Shader* RenderDevice::CreateShader(ShaderType type,
									   const char* shaderSource)
	{
		Shader* shader = new Shader();
		shader->m_type  = type;

		switch (type)
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

		shader->m_source = std::string(shaderSource);

		const GLchar* source = shaderSource;

		GL_CHECK_ERROR(glShaderSource(shader->m_id, 1, &source, NULL));
		GL_CHECK_ERROR(glCompileShader(shader->m_id));
		GL_CHECK_ERROR(glGetShaderiv(shader->m_id, GL_COMPILE_STATUS, &success));

		if (success == GL_FALSE)
		{
			GL_CHECK_ERROR(glGetShaderInfoLog(shader->m_id, 512, NULL, infoLog));
			std::cout << "Shader Compilation Failed" << std::endl;
			return nullptr;
		}

		return shader;
	}

	ShaderProgram* RenderDevice::CreateShaderProgram(Shader* vertexShader,
													 Shader* pixelShader,
													 Shader* geometryShader,
													 Shader* controlShader,
													 Shader* evaluationShader)
	{
		ShaderProgram* shaderProgram = new ShaderProgram();
        shaderProgram->m_resource_id = m_shader_program_res_id++;

		GL_CHECK_ERROR(shaderProgram->m_id = glCreateProgram());

		shaderProgram->m_shader_map[ShaderType::VERTEX] = vertexShader;
		shaderProgram->m_shader_map[ShaderType::PIXEL] = pixelShader;

		if (geometryShader)
			shaderProgram->m_shader_map[ShaderType::GEOMETRY] = geometryShader;

		if (controlShader && evaluationShader)
		{
			shaderProgram->m_shader_map[ShaderType::TESSELLATION_CONTROL] = controlShader;
			shaderProgram->m_shader_map[ShaderType::TESSELLATION_EVALUATION] = evaluationShader;
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
			    GLuint uboIndex = glGetUniformBlockIndex(shaderProgram->m_id, uniformNameChar);
				glUniformBlockBinding(shaderProgram->m_id, uboIndex, binding);
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
				GLuint location = glGetUniformLocation(shaderProgram->m_id, uniformNameChar);
				it.second->m_sampler_bindings[binding] = location;
			}
		}

		return shaderProgram;
	}

	DepthStencilState* RenderDevice::CreateDepthStencilState(bool enableDepthTest,
															 bool enableStencilTest,
															 bool depthMask,
															 ComparisonFunction depthComparisonFunction,
															 StencilOperation frontStencilFail,
															 StencilOperation frontStencilPassDepthFail,
															 StencilOperation frontStencilPassDepthPass,
															 ComparisonFunction frontStencilComparisonFunction,
															 StencilOperation backStencilFail,
															 StencilOperation backStencilPassDepthFail,
															 StencilOperation backStencilPassDepthPass,
															 ComparisonFunction backStencilComparisonFunction,
															 uint stencilMask)
	{
		DepthStencilState* depthStencilState = new DepthStencilState();

		// Set Depth Options

		if (enableDepthTest)
			depthStencilState->m_enableDepth = true;
		else
			depthStencilState->m_enableDepth = false;

		switch (depthComparisonFunction)
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

		depthStencilState->m_depthMask = (depthMask) ? GL_TRUE : GL_FALSE;

		// Set Stencil Options

		if (enableStencilTest)
			depthStencilState->m_enableStencil = true;
		else
			depthStencilState->m_enableStencil = false;

		switch (frontStencilComparisonFunction)
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

		switch (backStencilComparisonFunction)
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

		switch (frontStencilFail)
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

		switch (frontStencilPassDepthPass)
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

		switch (frontStencilPassDepthFail)
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

		switch (backStencilFail)
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

		switch (backStencilPassDepthPass)
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

		switch (backStencilPassDepthFail)
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

	void RenderDevice::AttachRenderTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{
		framebuffer->m_render_targets.push_back(renderTarget);

		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id));
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->m_render_targets.size() - 1), renderTarget->m_glTextureTarget, renderTarget->m_id, 0));

		std::vector<GLuint> drawBuffers;

		for (int i = 0; i < framebuffer->m_render_targets.size(); i++)
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

		GL_CHECK_ERROR(glDrawBuffers(framebuffer->m_render_targets.size(), &drawBuffers[0]));

		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, 0));
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void RenderDevice::AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{
		framebuffer->m_depth_target = renderTarget;
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id));
		GL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderTarget->m_glTextureTarget, renderTarget->m_id, 0));
		GL_CHECK_ERROR(glBindTexture(renderTarget->m_glTextureTarget, 0));
		GL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void RenderDevice::DestroyTexture1D(Texture1D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::DestroyTexture2D(Texture2D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::DestroyTexture3D(Texture3D* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::DestroyTextureCube(TextureCube* texture)
	{
		glDeleteTextures(1, &texture->m_id);
		delete texture;
	}

	void RenderDevice::DestroyVertexBuffer(VertexBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::DestroyIndexBuffer(IndexBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::DestroyUniformBuffer(UniformBuffer* buffer)
	{
		glDeleteBuffers(1, &buffer->m_id);
		delete buffer;
	}

	void RenderDevice::DestroyVertexArray(VertexArray* vertexArray)
	{
		GL_CHECK_ERROR(glDeleteVertexArrays(1, &vertexArray->m_id));
		delete vertexArray;
	}

	void RenderDevice::DestroyRasterizerState(RasterizerState* state)
	{
		delete state;
	}

	void RenderDevice::DestroySamplerState(SamplerState* state)
	{
		GL_CHECK_ERROR(glDeleteSamplers(1, &state->m_id));
		delete state;
	}

	void RenderDevice::DestroyDepthStencilState(DepthStencilState* state)
	{
		delete state;
	}

	void RenderDevice::DestroyFramebuffer(Framebuffer* framebuffer)
	{
		glDeleteFramebuffers(1, &framebuffer->m_id);
		delete framebuffer;
	}

	void RenderDevice::DestroyShader(Shader* shader)
	{
		delete shader;
	}

	void RenderDevice::DestoryShaderProgram(ShaderProgram* program)
	{
		for (auto it : program->m_shader_map)
			delete it.second;

		glDeleteProgram(program->m_id);
		delete program;
	}

	void RenderDevice::BindTexture(Texture* texture,
								   ShaderType shaderStage,
								   uint bufferSlot)
	{
		GL_CHECK_ERROR(glBindTexture(texture->m_glTextureTarget ,texture->m_id));
	}

	void RenderDevice::BindUniformBuffer(UniformBuffer* uniformBuffer,
									     ShaderType shaderStage,
									     uint bufferSlot)
	{
		GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, bufferSlot, uniformBuffer->m_id));
	}

	void RenderDevice::BindVertexArray(VertexArray* vertexArray)
	{
		glBindVertexArray(vertexArray->m_id);
	}

	void RenderDevice::BindRasterizerState(RasterizerState* state)
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

	void RenderDevice::BindSamplerState(SamplerState* state,
										ShaderType shaderStage,
										uint slot)
	{
		GL_CHECK_ERROR(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK_ERROR(glBindSampler(slot, state->m_id));
		GL_CHECK_ERROR(glUniform1i(m_current_program->m_shader_map[shaderStage]->m_sampler_bindings[slot], slot));
	}

	void RenderDevice::BindFramebuffer(Framebuffer* framebuffer)
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

	void RenderDevice::BindDepthStencilState(DepthStencilState* state)
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

	void RenderDevice::BindShaderProgram(ShaderProgram* program)
	{
		m_current_program = program;
		GL_CHECK_ERROR(glUseProgram(program->m_id));
	}

	void* RenderDevice::MapBuffer(Buffer* buffer, BufferMapType type)
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

	void RenderDevice::UnmapBuffer(Buffer* buffer)
	{
		GL_CHECK_ERROR(glUnmapBuffer(buffer->bufferType));
		glBindBuffer(buffer->bufferType, 0);
	}

	void RenderDevice::SetPrimitiveType(DrawPrimitive _primitive)
	{
		switch (_primitive)
		{
		case DrawPrimitive::POINTS:
			m_primitive_type = GL_POINTS;
			break;

		case DrawPrimitive::TRIANGLES:
			m_primitive_type = GL_TRIANGLES;
			break;

		case DrawPrimitive::TRIANGLE_STRIP:
			m_primitive_type = GL_TRIANGLE_STRIP;
			break;

		case DrawPrimitive::LINES:
			m_primitive_type = GL_LINES;
			break;

		case DrawPrimitive::LINE_STRIP:
			m_primitive_type = GL_LINE_STRIP;
			break;
		}
	}

	void RenderDevice::ClearFramebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor)
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

	void RenderDevice::SetViewport(int width, int height, int topLeftX, int topLeftY)
	{
		glViewport(topLeftX,
			      (PlatformBackend::GetHeight() - (height + topLeftY)),
				   width,
				   height);
	}

	void RenderDevice::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_window);
	}

	void RenderDevice::Draw(int firstIndex,
							int count)
	{
		GL_CHECK_ERROR(glDrawArrays(m_primitive_type, firstIndex, count));
	}

	void RenderDevice::DrawIndexed(int indexCount)
	{
		GL_CHECK_ERROR(glDrawElements(m_primitive_type, indexCount, GL_UNSIGNED_INT, 0));
	}

	void RenderDevice::DrawIndexedBaseVertex(int indexCount,
											 unsigned int baseIndex,
											 unsigned int baseVertex)
	{
		GL_CHECK_ERROR(glDrawElementsBaseVertex(m_primitive_type, indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * baseIndex), baseVertex));
	}

	void RenderDevice::DrawInstanced()
	{


	}

	void RenderDevice::DrawIndexedInstanced()
	{

	}

} }

#endif
