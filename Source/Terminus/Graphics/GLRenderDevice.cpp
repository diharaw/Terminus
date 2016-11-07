#include "RenderDevice.h"
#include <iostream>

#ifdef TERMINUS_OPENGL

namespace Terminus { namespace Graphics {

	RenderDevice::RenderDevice()
	{

	}

	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::Initialize(void* memory, size_t size)
	{

	}

	Texture1D* RenderDevice::CreateTexture1D(uint16 width,
											 void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture1D* texture = new Texture1D();

		glGenTextures(1, &texture->m_id);
		texture->m_glTextureTarget = GL_TEXTURE_1D;

		glBindTexture(GL_TEXTURE_1D, texture->m_id);

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

		glTexImage1D(GL_TEXTURE_1D, 0, texformat, width, 0, texformat, textype, data);

		if(generateMipmaps)
			glGenerateMipmap(GL_TEXTURE_1D);

		glBindTexture(GL_TEXTURE_1D, 0);

		return texture;
	}

	Texture2D* RenderDevice::CreateTexture2D(uint16 width,
											 uint16 height,
											 void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture2D* texture = new Texture2D();

		glGenTextures(1, &texture->m_id);
		texture->m_glTextureTarget = GL_TEXTURE_2D;

		glBindTexture(GL_TEXTURE_2D, texture->m_id);

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

		glTexImage2D(GL_TEXTURE_2D, 0, texformat, width, height,  0, texformat, textype, data);

		if (generateMipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

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

		glGenTextures(1, &texture->m_id);
		texture->m_glTextureTarget = GL_TEXTURE_3D;

		glBindTexture(GL_TEXTURE_3D, texture->m_id);

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
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

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
		glGenBuffers(1, &buffer->m_id);

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

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
		glGenBuffers(1, &buffer->m_id);

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

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
		glGenBuffers(1, &buffer->m_id);

		GLenum glusageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			glusageType = GL_STATIC_DRAW;

		case BufferUsageType::DYNAMIC:
			glusageType = GL_DYNAMIC_DRAW;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, buffer->m_id);
		glBufferData(GL_UNIFORM_BUFFER, size, data, glusageType);
		glBindBuffer(GL_UNIFORM_BUFFER,0);

		buffer->Data = data;
		buffer->Size = size;
		buffer->UsageType = glusageType;

		return buffer;
	}

	VertexArray* RenderDevice::CreateVertexArray(VertexBuffer* vertexBuffer,
												 IndexBuffer* indexBuffer,
												 InputLayoutType layoutType,
												 InputLayout* layout = nullptr)
	{
		VertexArray* vertexArray = new VertexArray();

		glGenVertexArrays(1, &vertexArray->m_id);
		glBindVertexArray(vertexArray->m_id);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer->Size, vertexBuffer->Data, vertexBuffer->UsageType);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->Size, indexBuffer->Data, indexBuffer->UsageType);

		if (layoutType == InputLayoutType::STANDARD_VERTEX)
		{
			// Vertices

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			// Tex Coords

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TexCoord));

			// Normals

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal));

			// Tangents

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Tangent));
		}
		else if (layoutType == InputLayoutType::STANDARD_SKINNED_VERTEX)
		{
			// Vertices

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)0);

			// Tex Coords

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_TexCoord));

			// Normals

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_Normal));

			// Tangents

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_Tangent));

			// Bone Indices

			glEnableVertexAttribArray(4);
			glVertexAttribIPointer(4, 4, GL_INT, sizeof(SkeletalVertex), (GLvoid*)offsetof(SkeletalVertex, m_BoneIndices));

			// Bone Weights

			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex), (GLvoid*)(offsetof(SkeletalVertex, m_BoneWeights)));
		}
		else if (layoutType == InputLayoutType::CUSTOM_VERTEX)
		{
			for (int i = 0; i < layout->m_Elements.size(); i++)
			{
				glEnableVertexAttribArray(i);

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

				glVertexAttribPointer(i, layout->m_Elements[i].m_numSubElements, dataType, layout->m_Elements[i].m_isNormalized, layout->m_vertexSize, (GLvoid*)0);
			}
		}

		glBindVertexArray(vertexArray->m_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_id);

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

		glGenSamplers(1, &samplerState->m_id);

		switch (wrapModeU)
		{
		case TextureWrapMode::REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
			break;
		}

		switch (wrapModeV)
		{
		case TextureWrapMode::REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
			break;
		}

		switch (wrapModeW)
		{
		case TextureWrapMode::REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { borderRed, borderGreen, borderBlue, borderAlpha };
			glSamplerParameterfv(samplerState->m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
			break;
		}

		if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		else if (minFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST && magFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		else if (minFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR && magFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR)
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		}
		else
		{
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(samplerState->m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (GLEW_EXT_texture_filter_anisotropic)
		{
			GLfloat glmaxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &glmaxAnisotropy);

			if (maxAnisotropy <= glmaxAnisotropy)
				glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			else
				glSamplerParameterf(samplerState->m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, glmaxAnisotropy);
		}

		return samplerState;
	}

	Framebuffer* RenderDevice::CreateFramebuffer()
	{
		Framebuffer* framebuffer = new Framebuffer();

		glGenFramebuffers(1, &framebuffer->m_id);

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
			shader->m_id = glCreateShader(GL_VERTEX_SHADER);
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

		glShaderSource(shader->m_id, 1, &source, NULL);
		glCompileShader(shader->m_id);
		glGetShaderiv(shader->m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader->m_id, 512, NULL, infoLog);
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

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id);
		glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (framebuffer->m_render_targets.size() - 1), renderTarget->m_glTextureTarget, renderTarget->m_id, 0);

		std::vector<GLuint> drawBuffers;

		for (int i = 0; i < framebuffer->m_render_targets.size(); i++)
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

		glDrawBuffers(framebuffer->m_render_targets.size(), &drawBuffers[0]);

		glBindTexture(renderTarget->m_glTextureTarget, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderDevice::AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{
		framebuffer->m_depth_target = renderTarget;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id);
		glBindTexture(renderTarget->m_glTextureTarget, framebuffer->m_id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderTarget->m_glTextureTarget, renderTarget->m_id, 0);
		glBindTexture(renderTarget->m_glTextureTarget, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderDevice::DestroyTexture1D(Texture1D* texture)
	{

	}

	void RenderDevice::DestroyTexture2D(Texture2D* texture)
	{

	}

	void RenderDevice::DestroyTexture3D(Texture3D* texture)
	{

	}

	void RenderDevice::DestroyTextureCube(TextureCube* texture)
	{


	}

	void RenderDevice::DestroyVertexBuffer(VertexBuffer* buffer)
	{

	}

	void RenderDevice::DestroyIndexBuffer(IndexBuffer* buffer)
	{

	}

	void RenderDevice::DestroyUniformBuffer(UniformBuffer* buffer)
	{

	}

	void RenderDevice::DestroyRasterizerState(RasterizerState* state)
	{

	}

	void RenderDevice::DestroySamplerState(SamplerState* state)
	{

	}

	void RenderDevice::DestroyDepthStencilState(DepthStencilState* state)
	{


	}

	void RenderDevice::DestroyFramebuffer(Framebuffer* framebuffer)
	{

	}

	void RenderDevice::DestroyShader(Shader* shader)
	{

	}

	void RenderDevice::DestoryShaderProgram(ShaderProgram* program)
	{

	}

	void RenderDevice::BindTexture(Texture* texture,
								   ShaderType shaderStage)
	{

	}

	void RenderDevice::BindUniformBuffer(UniformBuffer* uniformBuffer,
									     ShaderType shaderStage,
									     uint bufferSlot)
	{

	}

	void RenderDevice::BindRasterizerState(RasterizerState* state)
	{

	}

	void RenderDevice::BindSamplerState(SamplerState* state,
										ShaderType shaderStage,
										uint slot)
	{

	}

	void RenderDevice::BindFramebuffer(Framebuffer* framebuffer)
	{

	}

	void RenderDevice::BindDepthStencilState(DepthStencilState* state)
	{

	}

	void RenderDevice::BindShaderProgram(ShaderProgram* program)
	{

	}

	void* RenderDevice::MapBuffer(Buffer* buffer, BufferMapType type)
	{

	}

	void RenderDevice::UnmapBuffer(Buffer* buffer)
	{

	}

	void RenderDevice::Draw(int firstIndex,
							int count)
	{

	}

	void RenderDevice::DrawIndexed(int indexCount)
	{

	}

	void RenderDevice::DrawIndexedBaseVertex(int indexCount,
											 unsigned int baseIndex,
											 unsigned int baseVertex)
	{

	}

	void RenderDevice::DrawInstanced()
	{


	}

	void RenderDevice::DrawIndexedInstanced()
	{

	}

} }

#endif