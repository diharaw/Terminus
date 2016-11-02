#include "../../Third Party/glew/include/GL/glew.h"
#include "../Platform/PlatformBackend.h"
#include "RenderBackend.h"
#include "../Resource/AssetCommon.h"
#include "CommandList.h"
#include "../Utility/SlotMap.h"

struct SamplerState
{
	GLuint m_id;
};

struct ITexture
{
	GLuint m_id;
};

struct Texture2D : public ITexture
{
	uint16_t m_Width;
	uint16_t m_Height;
};

struct TextureCube : public ITexture
{

};

struct Framebuffer
{
	GLuint m_id;
};

struct IBuffer
{
	GLuint m_id;
	GLenum UsageType;
	void* Data;
	uint Size;
};

struct VertexArray
{
	GLuint m_id;
};

struct VertexBuffer : public IBuffer
{

};

struct IndexBuffer : public IBuffer
{

};

struct UniformBuffer : public IBuffer
{

};

struct Shader
{
	GLuint m_id;
};

struct ShaderProgram
{
    GLuint m_id;
};

namespace RenderBackend
{
	// Resource Pools
	SlotMap<Texture2D,     MAX_TEXTURE_2D>	   m_Texture2DPool;
	SlotMap<TextureCube,   MAX_TEXTURE_CUBE>   m_TextureCubePool;
	SlotMap<Framebuffer,   MAX_FRAMEBUFFER>    m_FramebufferPool;
	SlotMap<VertexArray,   MAX_VERTEX_ARRAY>   m_VertexArrayPool;
	SlotMap<VertexBuffer,  MAX_VERTEX_BUFFER>  m_VertexBufferPool;
	SlotMap<UniformBuffer, MAX_UNIFORM_BUFFER> m_UniformBufferPool;
	SlotMap<IndexBuffer,   MAX_INDEX_BUFFER>   m_IndexBufferPool;
	SlotMap<ShaderProgram, MAX_SHADER_PROGRAM> m_ShaderProgramPool;
	SlotMap<Shader,        MAX_SHADER>		   m_ShaderPool;
	SlotMap<SamplerState,  MAX_SAMPLER_STATE>  m_SamplerStatePool;
    
    // GLFW Window
    GLFWwindow* m_Window;
    
    void Initialize()
    {
        m_Window = PlatformBackend::GetWindow();
        glfwMakeContextCurrent(m_Window);

		glewExperimental = GL_TRUE;
		glewInit();
    }
    
    void SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

	void Draw(int _firstIndex, int _count)
	{
		glDrawArrays(GL_TRIANGLES, _firstIndex, _count);
	}

	void DrawIndexed(int _indexCount)
	{
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
	}

	void DrawIndexedBaseVertex(int _indexCount, unsigned int _baseIndex, unsigned int _baseVertex)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * _baseIndex), _baseVertex);
	}

	void ClearRenderTarget(Vector4 _ClearColor, FramebufferClearTarget _Target)
	{
		glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

		switch (_Target)
		{
		case FB_TARGET_DEPTH:
			glClear(GL_DEPTH_BUFFER_BIT);
			break;

		case FB_TARGET_ALL:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;
		}
	}

	void BindSamplerState(ResourceHandle _SamplerState, int _Slot)
	{
		glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, m_SamplerStatePool.lookup(_Slot)->m_id);
	}

	void UnbindSamplerState(int _Slot)
	{
        glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, 0);
	}

	void BindTexture2D(ResourceHandle _Texture2D)
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture2DPool.lookup(_Texture2D)->m_id);
	}

	void UnbindTexture2D()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BindTextureCube(ResourceHandle _TextureCube)
	{

	}

	void BindShaderProgram(ResourceHandle _ShaderProgram)
	{

	}

	void BindVertexArray(ResourceHandle _VertexArray)
	{
		glBindVertexArray(m_VertexArrayPool.lookup(_VertexArray)->m_id);
	}

	void UnbindVertexArray()
	{
		glBindVertexArray(0);
	}

	void BindUniformBuffer(ResourceHandle _UniformBuffer)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferPool.lookup(_UniformBuffer)->m_id);
	}

	void BindVertexBuffer(ResourceHandle _VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferPool.lookup(_VertexBuffer)->m_id);
	}

	void BindIndexBuffer(ResourceHandle _IndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferPool.lookup(_IndexBuffer)->m_id);
	}

	void UnbindUniformBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UnbindVertexBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UnbindIndexBuffer()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void BindFramebuffer(ResourceHandle _Framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferPool.lookup(_Framebuffer)->m_id);
	}

	void UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* MapUniformBuffer(ResourceHandle _UniformBuffer, BufferMapType _Type)
	{
		BindUniformBuffer(_UniformBuffer);

		void* bufferPointer;

		switch (_Type)
		{

		case MAP_READ:
			bufferPointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
			break;

		case MAP_WRITE:
			bufferPointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			break;

		case MAP_READ_WRITE:
			bufferPointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
			break;

		default:
			bufferPointer = nullptr;
			break;
		}

		return bufferPointer;
	}

	void UnmapUniformBuffer()
	{
		glUnmapBuffer(GL_UNIFORM_BUFFER);
		UnbindUniformBuffer();
	}

    ResourceHandle CreateTexture2D(uint16_t _Width, uint16_t _Height, void* _Data, bool _MipMaps)
	{
        ResourceHandle handle = m_Texture2DPool.add();
		Texture2D* texture = m_Texture2DPool.lookup(handle);
		texture->m_Width = _Width;
		texture->m_Height = _Height;
		glGenTextures(1, &texture->m_id);
		
		if (_MipMaps)
		{
			glBindTexture(GL_TEXTURE_2D, texture->m_id);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindTexture(GL_TEXTURE_2D, texture->m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return handle;
	}

	ResourceHandle CreateTextureCube()
	{
        return 0;
	}

	ResourceHandle CreateTextureCubeIndividual()
	{
        return 0;
    }

	ResourceHandle CreateFramebuffer(ResourceHandle* _RenderTargets, int _Count, ResourceHandle _DepthTarget)
	{
        ResourceHandle handle = m_FramebufferPool.add();
		Framebuffer* framebuffer = m_FramebufferPool.lookup(handle);
		glGenFramebuffers(1, &framebuffer->m_id);
        
		BindFramebuffer(handle);

		for (int i = 0; i < _Count; i++)
		{
            Texture2D* texture = m_Texture2DPool.lookup(_RenderTargets[i]);
            
			BindTexture2D(_RenderTargets[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->m_id, 0);
			UnbindTexture2D();
		}

        Texture2D* depth = m_Texture2DPool.lookup(_DepthTarget);
        
		BindTexture2D(_DepthTarget);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->m_id, 0);
		UnbindTexture2D();

		std::vector<GLuint> drawBuffers;

		for (int i = 0; i < _Count; i++)
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

		glDrawBuffers(_Count, &drawBuffers[0]);
		UnbindTexture2D();

		return handle;
	}

	ResourceHandle CreateVertexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
        ResourceHandle handle = m_VertexBufferPool.add();
        
		VertexBuffer* buffer = m_VertexBufferPool.lookup(handle);
		glGenBuffers(1, &buffer->m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		BindVertexBuffer(handle);
		glBufferData(GL_ARRAY_BUFFER, _Size, _Data, usageType);
        UnbindVertexBuffer();
        
        return handle;
	}

	ResourceHandle CreateIndexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
        ResourceHandle handle = m_IndexBufferPool.add();
        
		IndexBuffer* buffer = m_IndexBufferPool.lookup(handle);
		glGenBuffers(1, &buffer->m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		buffer->Data = _Data;
		buffer->Size = _Size;
		buffer->UsageType = usageType;

        return handle;
	}

	ResourceHandle CreateUniformBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
        ResourceHandle handle = m_UniformBufferPool.add();
        
		UniformBuffer* buffer = m_UniformBufferPool.lookup(handle);
		glGenBuffers(1, &buffer->m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		BindUniformBuffer(handle);
		glBufferData(GL_UNIFORM_BUFFER, _Size, _Data, usageType);
		UnbindUniformBuffer();

        return handle;
	}

	ResourceHandle CreateVertexArray(ResourceHandle _vertexBuffer,
                                     ResourceHandle _indexBuffer,
                                     InputLayoutType _layoutType,
                                     InputLayout* _layout)
	{
        ResourceHandle handle = m_VertexArrayPool.add();
        
		VertexArray* vertexArray = m_VertexArrayPool.lookup(handle);

		glGenVertexArrays(1, &vertexArray->m_id);
		glBindVertexArray(vertexArray->m_id);

        VertexBuffer* vertexBuffer = m_VertexBufferPool.lookup(_vertexBuffer);
        
		BindVertexBuffer(_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer->Size, vertexBuffer->Data, vertexBuffer->UsageType);
		UnbindVertexBuffer();
        
        IndexBuffer* indexBuffer = m_IndexBufferPool.lookup(_indexBuffer);

		BindIndexBuffer(_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->Size, indexBuffer->Data, indexBuffer->UsageType);
		UnbindIndexBuffer();

		if (_layoutType == LAYOUT_STANDARD_VERTEX)
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
		else if (_layoutType == LAYOUT_STANDARD_SKINNED_VERTEX)
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
		else if (_layoutType == LAYOUT_CUSTOM)
		{
			for (int i = 0; i < _layout->m_Elements.size(); i++)
			{
				glEnableVertexAttribArray(i);

				GLenum dataType;

				switch (_layout->m_Elements[i].m_type)
				{
				case BUFFER_FLOAT:
					dataType = GL_FLOAT;
					break;

				case BUFFER_INT:
					dataType = GL_INT;
					break;
				}

				glVertexAttribPointer(i, _layout->m_Elements[i].m_numSubElements, dataType, _layout->m_Elements[i].m_isNormalized, _layout->m_vertexSize, (GLvoid*)0);
			}
		}
		
        return handle;
	}
    
    ResourceHandle CreateShaderBase(char* _Data, GLenum _shaderType)
    {
        ResourceHandle handle = m_ShaderPool.add();
        
        Shader* shader = m_ShaderPool.lookup(handle);
        shader->m_id = glCreateShader(_shaderType);
        
        GLint success;
        GLchar infoLog[512];
        
        const GLchar* source = (GLchar*)_Data;
        
        glShaderSource(shader->m_id, 1, &source, NULL);
        glCompileShader(shader->m_id);
        glGetShaderiv(shader->m_id, GL_COMPILE_STATUS, &success);
        
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(shader->m_id, 512, NULL, infoLog);
            return USHRT_MAX;
        }
        else
        {
            return handle;
        }
    }
    
    ResourceHandle CreateVertexShader(char* _Data)
	{
        return CreateShaderBase(_Data, GL_VERTEX_SHADER);
	}

	ResourceHandle CreateGeometryShader(char* _Data)
	{
        return CreateShaderBase(_Data, GL_GEOMETRY_SHADER);
	}

	ResourceHandle CreateControlShader(char* _Data)
	{
        return CreateShaderBase(_Data, GL_TESS_CONTROL_SHADER);
	}

	ResourceHandle CreateEvaluationShader(char* _Data)
	{
        return CreateShaderBase(_Data, GL_TESS_EVALUATION_SHADER);
	}

	ResourceHandle CreatePixelShader(char* _Data)
	{
		return CreateShaderBase(_Data, GL_FRAGMENT_SHADER);
	}

	ResourceHandle CreateShaderProgram(ResourceHandle _Vertex,
									   ResourceHandle _Geometry,
									   ResourceHandle _TessellationControl,
									   ResourceHandle _TessellationEvalution,
									   ResourceHandle _Pixel)
	{
        assert(_Vertex != USHRT_MAX);
        assert(_Pixel != USHRT_MAX);
        
        ResourceHandle handle = m_ShaderProgramPool.add();
        
        Shader* shader;
        ShaderProgram* program = m_ShaderProgramPool.lookup(handle);
        
        shader = m_ShaderPool.lookup(_Vertex);
        glAttachShader(program->m_id, shader->m_id);
        
        shader = m_ShaderPool.lookup(_Pixel);
        glAttachShader(program->m_id, shader->m_id);
        
        if(_Geometry != USHRT_MAX)
        {
            shader = m_ShaderPool.lookup(_Geometry);
            glAttachShader(program->m_id, shader->m_id);
        }
        if(_TessellationControl != USHRT_MAX)
        {
            shader = m_ShaderPool.lookup(_TessellationControl);
            glAttachShader(program->m_id, shader->m_id);
        }
        if(_TessellationEvalution != USHRT_MAX)
        {
            shader = m_ShaderPool.lookup(_TessellationEvalution);
            glAttachShader(program->m_id, shader->m_id);
        }
        
        glLinkProgram(program->m_id);
        
        GLint success;
        char infoLog[512];
        
        glGetProgramiv(program->m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program->m_id, 512, NULL, infoLog);
            return USHRT_MAX;
        }

        return handle;
	}

	void SetTexture2DMipmap(ResourceHandle _Texture2D, int _Level, void* _Data)
	{
        assert(_Texture2D != USHRT_MAX);
        
        Texture2D* texture = m_Texture2DPool.lookup(_Texture2D);
        
		glBindTexture(GL_TEXTURE_2D, texture->m_id);
		glTexImage2D(GL_TEXTURE_2D, _Level, GL_RGBA, texture->m_Width, texture->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SetTextureCubeMipmap(ResourceHandle _TextureCube, int _Level, void* _Data)
	{
        assert(_TextureCube != USHRT_MAX);
        
        TextureCube* texture = m_TextureCubePool.lookup(_TextureCube);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_id);
		//glTexImage2D(GL_TEXTURE_CUBE_MAP, _Level, GL_RGBA, _TextureCube->m_Width, m_Texture2DPool[_Handle].m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

namespace GPUCommand
{
	void Draw(void* data)
	{
		CommandData::Draw* cmd_data = static_cast<CommandData::Draw*>(data);
		RenderBackend::Draw(cmd_data->FirstIndex, cmd_data->Count);
	}

	void DrawIndexed(void* data)
	{
		CommandData::DrawIndexed* cmd_data = static_cast<CommandData::DrawIndexed*>(data);
		RenderBackend::DrawIndexed(cmd_data->IndexCount);
	}

	void DrawIndexedBaseVertex(void* data)
	{
		CommandData::DrawIndexedBaseVertex* cmd_data = static_cast<CommandData::DrawIndexedBaseVertex*>(data);
		RenderBackend::DrawIndexedBaseVertex(cmd_data->IndexCount, cmd_data->BaseIndex, cmd_data->BaseVertex);
	}

	void UniformBufferCopy(void* data)
	{
		CommandData::UniformBufferCopy* cmd_data = static_cast<CommandData::UniformBufferCopy*>(data);
		
		void* ptr = RenderBackend::MapUniformBuffer(cmd_data->Buffer, cmd_data->MapType);
		memcpy(ptr, cmd_data->Data, cmd_data->Size);
		RenderBackend::UnmapUniformBuffer();
	}

	void ClearRenderTarget(void* data)
	{
		CommandData::ClearRenderTarget* cmd_data = static_cast<CommandData::ClearRenderTarget*>(data);
		RenderBackend::ClearRenderTarget(cmd_data->ClearColor, cmd_data->Target);
	}
}
