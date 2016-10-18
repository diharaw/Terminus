#include "../../Third Party/glew/include/GL/glew.h"
#include "../Platform/PlatformBackend.h"
#include "RenderBackend.h"
#include "../Resource/AssetCommon.h"
#include "CommandList.h"

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
	std::vector<Texture2D>	   m_Texture2DPool;
	std::vector<TextureCube>   m_TextureCubePool;
	std::vector<Framebuffer>   m_FramebufferPool;
	std::vector<VertexArray>   m_VertexArrayPool;
	std::vector<VertexBuffer>  m_VertexBufferPool;
	std::vector<UniformBuffer> m_UniformBufferPool;
	std::vector<IndexBuffer>   m_IndexBufferPool;
	std::vector<ShaderProgram> m_ShaderProgramPool;
	std::vector<Shader>		   m_ShaderPool;
	std::vector<SamplerState>  m_SamplerStatePool;
    
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

	void BindSamplerState(SamplerState* _SamplerState, int _Slot)
	{
		glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, _SamplerState->m_id);
	}

	void UnbindSamplerState(int _Slot)
	{
        glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, 0);
	}

	void BindTexture2D(Texture2D* _Texture2D)
	{
		glBindTexture(GL_TEXTURE_2D, _Texture2D->m_id);
	}

	void UnbindTexture2D()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BindTextureCube(TextureCube* _TextureCube)
	{

	}

	void BindShaderProgram(ShaderProgram* _ShaderProgram)
	{

	}

	void BindVertexArray(VertexArray* _VertexArray)
	{
		glBindVertexArray(_VertexArray->m_id);
	}

	void UnbindVertexArray()
	{
		glBindVertexArray(0);
	}

	void BindUniformBuffer(UniformBuffer* _UniformBuffer)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _UniformBuffer->m_id);
	}

	void BindVertexBuffer(VertexBuffer* _VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer->m_id);
	}

	void BindIndexBuffer(IndexBuffer* _IndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer->m_id);
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

	void BindFramebuffer(Framebuffer* _Framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _Framebuffer->m_id);
	}

	void UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* MapUniformBuffer(UniformBuffer* _UniformBuffer, BufferMapType _Type)
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

    Texture2D* CreateTexture2D(uint16_t _Width, uint16_t _Height, void* _Data, bool _MipMaps)
	{
		Texture2D texture;
		texture.m_Width = _Width;
		texture.m_Height = _Height;
		glGenTextures(1, &texture.m_id);
		
		if (_MipMaps)
		{
			glBindTexture(GL_TEXTURE_2D, texture.m_id);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindTexture(GL_TEXTURE_2D, texture.m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_Texture2DPool.push_back(texture);

		return &texture;
	}

	TextureCube* CreateTextureCube()
	{
        return 0;
	}

	TextureCube* CreateTextureCubeIndividual()
	{
        return 0;
    }

	Framebuffer* CreateFramebuffer(Texture2D* _RenderTargets, int _Count, Texture2D* _DepthTarget)
	{
		Framebuffer framebuffer;
		glGenFramebuffers(1, &framebuffer.m_id);
		BindFramebuffer(&framebuffer);

		for (int i = 0; i < _Count; i++)
		{
			BindTexture2D(&_RenderTargets[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _RenderTargets[i].m_id, 0);
			UnbindTexture2D();
		}

		BindTexture2D(_DepthTarget);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthTarget->m_id, 0);
		UnbindTexture2D();

		std::vector<GLuint> drawBuffers;

		for (int i = 0; i < _Count; i++)
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

		glDrawBuffers(_Count, &drawBuffers[0]);
		UnbindTexture2D();

		m_FramebufferPool.push_back(framebuffer);

		return &framebuffer;
	}

	VertexBuffer* CreateVertexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
		VertexBuffer buffer;
		glGenBuffers(1, &buffer.m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		BindVertexBuffer(&buffer);
		glBufferData(GL_ARRAY_BUFFER, _Size, _Data, usageType);
		UnbindVertexBuffer();

		m_VertexBufferPool.push_back(buffer);
        
        return &buffer;
	}

	IndexBuffer* CreateIndexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
		IndexBuffer buffer;
		glGenBuffers(1, &buffer.m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		buffer.Data = _Data;
		buffer.Size = _Size;
		buffer.UsageType = usageType;

        return &buffer;
	}

	UniformBuffer* CreateUniformBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
		UniformBuffer buffer;
		glGenBuffers(1, &buffer.m_id);

		GLenum usageType;

		switch (_UsageType)
		{
		case USAGE_STATIC:
			usageType = GL_STATIC_DRAW;

		case USAGE_DYNAMIC:
			usageType = GL_DYNAMIC_DRAW;
		}

		BindUniformBuffer(&buffer);
		glBufferData(GL_UNIFORM_BUFFER, _Size, _Data, usageType);
		UnbindUniformBuffer();
        
		m_UniformBufferPool.push_back(buffer);

        return &buffer;
	}

	VertexArray* CreateVertexArray(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer, InputLayoutType _layoutType, InputLayout* _layout)
	{
		VertexArray vertexArray;

		glGenVertexArrays(1, &vertexArray.m_id);
		glBindVertexArray(vertexArray.m_id);

		BindVertexBuffer(_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, _vertexBuffer->Size, _vertexBuffer->Data, _vertexBuffer->UsageType);
		UnbindVertexBuffer();

		BindIndexBuffer(_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer->Size, _indexBuffer->Data, _indexBuffer->UsageType);
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

		m_VertexArrayPool.push_back(vertexArray);
		
		return &vertexArray;
	}

	Shader* CreateVertexShader(char* _Data)
	{
		Shader shader;
		shader.m_id = glCreateShader(GL_VERTEX_SHADER);

		GLint success;
		GLchar infoLog[512];

		const GLchar* source = (GLchar*)_Data;

		glShaderSource(shader.m_id, 1, &source, NULL);
		glCompileShader(shader.m_id);
		glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
			return nullptr;
		}
		else
		{
			m_ShaderPool.push_back(shader);
			return &shader;
		}
	}

	Shader* CreateGeometryShader(char* _Data)
	{
		Shader shader;
		shader.m_id = glCreateShader(GL_GEOMETRY_SHADER);

		GLint success;
		GLchar infoLog[512];

		const GLchar* source = (GLchar*)_Data;

		glShaderSource(shader.m_id, 1, &source, NULL);
		glCompileShader(shader.m_id);
		glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
			return nullptr;
		}
		else
		{
			m_ShaderPool.push_back(shader);
			return &shader;
		}
	}

	Shader* CreateControlShader(char* _Data)
	{
		Shader shader;
		shader.m_id = glCreateShader(GL_TESS_CONTROL_SHADER);

		GLint success;
		GLchar infoLog[512];

		const GLchar* source = (GLchar*)_Data;

		glShaderSource(shader.m_id, 1, &source, NULL);
		glCompileShader(shader.m_id);
		glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
			return nullptr;
		}
		else
		{
			m_ShaderPool.push_back(shader);
			return &shader;
		}
	}

	Shader* CreateEvaluationShader(char* _Data)
	{
		Shader shader;
		shader.m_id = glCreateShader(GL_TESS_EVALUATION_SHADER);

		GLint success;
		GLchar infoLog[512];

		const GLchar* source = (GLchar*)_Data;

		glShaderSource(shader.m_id, 1, &source, NULL);
		glCompileShader(shader.m_id);
		glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
			return nullptr;
		}
		else
		{
			m_ShaderPool.push_back(shader);
			return &shader;
		}
	}

	Shader* CreatePixelShader(char* _Data)
	{
		Shader shader;
		shader.m_id = glCreateShader(GL_FRAGMENT_SHADER);

		GLint success;
		GLchar infoLog[512];

		const GLchar* source = (GLchar*)_Data;

		glShaderSource(shader.m_id, 1, &source, NULL);
		glCompileShader(shader.m_id);
		glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
			return nullptr;
		}
		else
		{
			m_ShaderPool.push_back(shader);
			return &shader;
		}
	}

	ShaderProgram* CreateShaderProgram(Shader* _Vertex,
									   Shader* _Geometry,
									   Shader* _TessellationControl,
									   Shader* _TessellationEvalution,
									   Shader* _Pixel)
	{
        assert(_Vertex != nullptr);
        assert(_Pixel != nullptr);
        
        ShaderProgram program;
        
        glAttachShader(program.m_id, _Vertex->m_id);
        glAttachShader(program.m_id, _Pixel->m_id);
        
        if(_Geometry)
            glAttachShader(program.m_id, _Geometry->m_id);
        if(_TessellationControl)
            glAttachShader(program.m_id, _TessellationControl->m_id);
        if(_TessellationEvalution)
            glAttachShader(program.m_id, _TessellationEvalution->m_id);
        
        glLinkProgram(program.m_id);
        
        GLint success;
        char infoLog[512];
        
        glGetProgramiv(program.m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program.m_id, 512, NULL, infoLog);
            return nullptr;
        }
        
		m_ShaderProgramPool.push_back(program);

        return &program;
	}

	void SetTexture2DMipmap(Texture2D* _Texture2D, int _Level, void* _Data)
	{
        assert(_Texture2D != nullptr);
		glBindTexture(GL_TEXTURE_2D, _Texture2D->m_id);
		glTexImage2D(GL_TEXTURE_2D, _Level, GL_RGBA, _Texture2D->m_Width, _Texture2D->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SetTextureCubeMipmap(TextureCube* _TextureCube, int _Level, void* _Data)
	{
        assert(_TextureCube != nullptr);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _TextureCube->m_id);
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