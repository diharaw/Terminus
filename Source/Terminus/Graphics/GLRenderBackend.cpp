#include "../../Third Party/glew/include/GL/glew.h"
#include "../Platform/PlatformBackend.h"
#include "RenderBackend.h"
#include "../Resource/AssetCommon.h"
#include "CommandList.h"
#include "../Utility/SlotMap.h"
#include "../Utility/StringUtility.h"
#include <unordered_map>

using BindingMap = std::unordered_map<GLuint, GLuint>;

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

struct RasterizerState
{
    GLenum m_cullFace;
    GLenum m_polygonMode;
    bool m_enableCullFace;
    bool m_enableMultisample;
    bool m_enableFrontFaceCCW;
    bool m_enableScissor;
};

struct DepthStencilState
{
    bool m_enableDepth;
    bool m_enableStencil;
    
    GLenum m_depthFunc;
    bool m_depthMask;
    
    GLenum m_frontStencilComparison;
    GLenum m_backStencilComparison;
    GLuint m_stencilMask;
    
    GLenum m_frontStencilFail;
    GLenum m_frontStencilPassDepthFail;
    GLenum m_frontStencilPassDepthPass;
    
    GLenum m_backStencilFail;
    GLenum m_backStencilPassDepthFail;
    GLenum m_backStencilPassDepthPass;
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
    BindingMap m_sampler_bindings;
    std::string m_source;
};

struct ShaderProgram
{
    GLuint m_id;
    ResourceHandle m_shaders[5];
    int m_shader_count;
};

namespace RenderBackend
{
	// Resource Pools
	SlotMap<Texture2D,       MAX_TEXTURE_2D>	   m_Texture2DPool;
	SlotMap<TextureCube,     MAX_TEXTURE_CUBE>     m_TextureCubePool;
	SlotMap<Framebuffer,     MAX_FRAMEBUFFER>      m_FramebufferPool;
	SlotMap<VertexArray,     MAX_VERTEX_ARRAY>     m_VertexArrayPool;
	SlotMap<VertexBuffer,    MAX_VERTEX_BUFFER>    m_VertexBufferPool;
	SlotMap<UniformBuffer,   MAX_UNIFORM_BUFFER>   m_UniformBufferPool;
	SlotMap<IndexBuffer,     MAX_INDEX_BUFFER>     m_IndexBufferPool;
	SlotMap<ShaderProgram,   MAX_SHADER_PROGRAM>   m_ShaderProgramPool;
	SlotMap<Shader,          MAX_SHADER>		   m_ShaderPool;
	SlotMap<SamplerState,    MAX_SAMPLER_STATE>    m_SamplerStatePool;
    SlotMap<RasterizerState, MAX_RASTERIZER_STATE> m_RasterizerStatePool;
    SlotMap<DepthStencilState, MAX_DEPTH_STENCIL_STATE> m_DepthStencilStatePool;
    
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
    
    void SetDepthStencilState(ResourceHandle _depthStencilState)
    {
        DepthStencilState& dss = m_DepthStencilStatePool.lookup(_depthStencilState);
        
        // Set Depth Options
        
        if (dss.m_enableDepth)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        
        glDepthFunc(dss.m_depthFunc);
        glDepthMask((dss.m_depthMask) ? GL_TRUE : GL_FALSE);
        
        // Set Stencil Options
        
        if (dss.m_enableStencil)
            glEnable(GL_STENCIL_TEST);
        else
            glDisable(GL_STENCIL_TEST);
        
        glStencilFuncSeparate(GL_FRONT, dss.m_frontStencilComparison, 1, dss.m_stencilMask);
        glStencilFuncSeparate(GL_FRONT, dss.m_backStencilComparison, 1, dss.m_stencilMask);
        
        // Front Stencil Operation
        
        glStencilOpSeparate(GL_FRONT, dss.m_frontStencilFail, dss.m_frontStencilPassDepthFail, dss.m_frontStencilPassDepthPass);
        glStencilOpSeparate(GL_BACK, dss.m_backStencilFail, dss.m_backStencilPassDepthFail, dss.m_backStencilPassDepthPass);
    }
    
    void SetRasterizerState(ResourceHandle _rasterizerState)
    {
        RasterizerState& rs = m_RasterizerStatePool.lookup(_rasterizerState);
        
        if(rs.m_enableCullFace)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
        
        glCullFace(rs.m_cullFace);
        
        glPolygonMode(GL_FRONT_AND_BACK, rs.m_polygonMode);
        
        if (rs.m_enableMultisample)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);
        
        if (rs.m_enableScissor)
            glEnable(GL_SCISSOR_TEST);
        else
            glDisable(GL_SCISSOR_TEST);
        
        if (rs.m_enableFrontFaceCCW)
            glFrontFace(GL_CCW);
        else
            glFrontFace(GL_CW);
    }
    
    void SetViewport(int _width, int _height, int _topLeftX, int _topLeftY)
    {
        glViewport(_topLeftX,
                   (PlatformBackend::GetHeight() - (_height + _topLeftY)),
                   _width,
                   _height);
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
            case FramebufferClearTarget::FB_TARGET_DEPTH:
                glClear(GL_DEPTH_BUFFER_BIT);
                break;

            case FramebufferClearTarget::FB_TARGET_ALL:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                break;
                
            case FramebufferClearTarget::FB_TARGET_COLOR:
                break;
                
            case FramebufferClearTarget::FB_TARGET_STENCIL:
                break;
		}
	}

	void BindSamplerState(ResourceHandle _SamplerState, int _Slot)
	{
		glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, m_SamplerStatePool.lookup(_Slot).m_id);
	}

	void UnbindSamplerState(int _Slot)
	{
        glActiveTexture(GL_TEXTURE0 + _Slot);
		glBindSampler(_Slot, 0);
	}

	void BindTexture2D(ResourceHandle _Texture2D)
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture2DPool.lookup(_Texture2D).m_id);
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
        glUseProgram(m_ShaderProgramPool.lookup(_ShaderProgram).m_id);
	}

	void BindVertexArray(ResourceHandle _VertexArray)
	{
		glBindVertexArray(m_VertexArrayPool.lookup(_VertexArray).m_id);
	}

	void UnbindVertexArray()
	{
		glBindVertexArray(0);
	}

	void BindUniformBuffer(ResourceHandle _UniformBuffer)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferPool.lookup(_UniformBuffer).m_id);
	}
    
    void BindUniformBuffer(ResourceHandle _uniformBuffer, ShaderType _shaderStage, unsigned int _bufferSlot)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, _bufferSlot, m_UniformBufferPool.lookup(_uniformBuffer).m_id);
    }

	void BindVertexBuffer(ResourceHandle _VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferPool.lookup(_VertexBuffer).m_id);
	}

	void BindIndexBuffer(ResourceHandle _IndexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferPool.lookup(_IndexBuffer).m_id);
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
        if(HANDLE_VALID(_Framebuffer))
            glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferPool.lookup(_Framebuffer).m_id);
        else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

        case BufferMapType::MAP_READ:
			bufferPointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
			break;

		case BufferMapType::MAP_WRITE:
			bufferPointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			break;

		case BufferMapType::MAP_READ_WRITE:
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
    
    void DestroyVertexBuffer(ResourceHandle _Handle)
    {
        glDeleteBuffers(1, &m_VertexBufferPool.lookup(_Handle).m_id);
        m_VertexBufferPool.remove(_Handle);
    }
    
    void DestroyIndexBuffer(ResourceHandle _Handle)
    {
        glDeleteBuffers(1, &m_IndexBufferPool.lookup(_Handle).m_id);
        m_IndexBufferPool.remove(_Handle);
    }
    
    void DestroyUniformBuffer(ResourceHandle _Handle)
    {
        glDeleteBuffers(1, &m_UniformBufferPool.lookup(_Handle).m_id);
        m_UniformBufferPool.remove(_Handle);
    }
    
    void DestroyVertexArray(ResourceHandle _Handle)
    {
        glDeleteVertexArrays(1, &m_VertexArrayPool.lookup(_Handle).m_id);
        m_VertexArrayPool.remove(_Handle);
    }
    
    void DestroyShader(ResourceHandle _Handle)
    {
        glDeleteShader(m_ShaderPool.lookup(_Handle).m_id);
        m_ShaderPool.remove(_Handle);
    }
    
    void DestroyShaderProgram(ResourceHandle _Handle)
    {
        ShaderProgram& program = m_ShaderProgramPool.lookup(_Handle);
        
        for (int i = 0; i < program.m_shader_count; i++)
        {
            DestroyShader(program.m_shaders[i]);
        }
        
        glDeleteProgram(program.m_id);
        m_ShaderProgramPool.remove(_Handle);
    }
    
    void DestroySamplerState(ResourceHandle _Handle)
    {
        glDeleteSamplers(1, &m_SamplerStatePool.lookup(_Handle).m_id);
        m_SamplerStatePool.remove(_Handle);
    }

    ResourceHandle CreateTexture2D(uint16_t _Width, uint16_t _Height, void* _Data, bool _MipMaps)
	{
        ResourceHandle handle = m_Texture2DPool.add();
		Texture2D& texture = m_Texture2DPool.lookup(handle);
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
		Framebuffer& framebuffer = m_FramebufferPool.lookup(handle);
		glGenFramebuffers(1, &framebuffer.m_id);
        
		BindFramebuffer(handle);

		for (int i = 0; i < _Count; i++)
		{
            Texture2D& texture = m_Texture2DPool.lookup(_RenderTargets[i]);
            
			BindTexture2D(_RenderTargets[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.m_id, 0);
			UnbindTexture2D();
		}

        Texture2D& depth = m_Texture2DPool.lookup(_DepthTarget);
        
		BindTexture2D(_DepthTarget);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.m_id, 0);
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
        
		VertexBuffer& buffer = m_VertexBufferPool.lookup(handle);
		glGenBuffers(1, &buffer.m_id);

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
        
		IndexBuffer& buffer = m_IndexBufferPool.lookup(handle);
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

        return handle;
	}

	ResourceHandle CreateUniformBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType)
	{
        ResourceHandle handle = m_UniformBufferPool.add();
        
		UniformBuffer& buffer = m_UniformBufferPool.lookup(handle);
		glGenBuffers(1, &buffer.m_id);

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
        
		VertexArray& vertexArray = m_VertexArrayPool.lookup(handle);

		glGenVertexArrays(1, &vertexArray.m_id);
		glBindVertexArray(vertexArray.m_id);

        VertexBuffer& vertexBuffer = m_VertexBufferPool.lookup(_vertexBuffer);
        
		BindVertexBuffer(_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.Size, vertexBuffer.Data, vertexBuffer.UsageType);
		UnbindVertexBuffer();
        
        IndexBuffer& indexBuffer = m_IndexBufferPool.lookup(_indexBuffer);

		BindIndexBuffer(_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.Size, indexBuffer.Data, indexBuffer.UsageType);
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

				GLenum dataType = 0;

				switch (_layout->m_Elements[i].m_type)
				{
                    case BUFFER_FLOAT:
                        dataType = GL_FLOAT;
                        break;

                    case BUFFER_INT:
                        dataType = GL_INT;
                        break;
                        
                    case BUFFER_UINT:
                        break;
				}

				glVertexAttribPointer(i, _layout->m_Elements[i].m_numSubElements, dataType, _layout->m_Elements[i].m_isNormalized, _layout->m_vertexSize, (GLvoid*)0);
			}
		}
		
        return handle;
	}
    
    ResourceHandle CreateRasterizerState(CullMode _cullMode,
                                                           FillMode _fillMode,
                                                           bool _frontWindingCCW,
                                                           bool _multisample,
                                                           bool _scissor)
    {
        ResourceHandle handle = m_RasterizerStatePool.add();
        RasterizerState& rasterizerState = m_RasterizerStatePool.lookup(handle);
        
        switch (_cullMode)
        {
            case CULL_FRONT:
                rasterizerState.m_cullFace = GL_FRONT;
                rasterizerState.m_enableCullFace = true;
                break;
                
            case CULL_BACK:
                rasterizerState.m_cullFace = GL_BACK;
                rasterizerState.m_enableCullFace = true;
                break;
                
            case CULL_NONE:
                rasterizerState.m_cullFace = GL_BACK;
                rasterizerState.m_enableCullFace = false;
                break;
                
            default:
                rasterizerState.m_cullFace = GL_BACK;
                rasterizerState.m_enableCullFace = true;
                break;
        }
        
        switch (_fillMode)
        {
            case FILL_SOLID:
                rasterizerState.m_polygonMode = GL_FILL;
                break;
                
            case FILL_WIREFRAME:
                rasterizerState.m_polygonMode = GL_LINE;
                break;
                
            default:
                rasterizerState.m_polygonMode = GL_FILL;
                break;
        }
        
        if (_multisample)
            rasterizerState.m_enableMultisample = true;
        else
            rasterizerState.m_enableMultisample = false;
        
        if (_scissor)
            rasterizerState.m_enableScissor = true;
        else
            rasterizerState.m_enableScissor = false;
        
        if (_frontWindingCCW)
            rasterizerState.m_enableFrontFaceCCW = true;
        else
            rasterizerState.m_enableFrontFaceCCW = false;
        
        return handle;
    }
    
    ResourceHandle CreateDepthStencilState(bool _enableDepthTest,
                                           bool _enableStencilTest,
                                           bool _depthMask,
                                           ComparisonFunction _depthComparisonFunction,
                                           StencilOperation _frontStencilFail,
                                           StencilOperation _frontStencilPassDepthFail,
                                           StencilOperation _frontStencilPassDepthPass,
                                           ComparisonFunction _frontStencilComparisonFunction,
                                           StencilOperation _backStencilFail,
                                           StencilOperation _backStencilPassDepthFail,
                                           StencilOperation _backStencilPassDepthPass,
                                           ComparisonFunction _backStencilComparisonFunction,
                                           unsigned int _stencilMask)
    {
        ResourceHandle handle = m_DepthStencilStatePool.add();
        DepthStencilState* depthStencilState = &m_DepthStencilStatePool.lookup(handle);
        
        // Set Depth Options
        
        if (_enableDepthTest)
            depthStencilState->m_enableDepth = true;
        else
            depthStencilState->m_enableDepth = false;
        
        switch (_depthComparisonFunction)
        {
            case FUNC_NEVER:
                depthStencilState->m_depthFunc = GL_NEVER;
                break;
                
            case FUNC_LESS:
                depthStencilState->m_depthFunc = GL_LESS;
                break;
                
            case FUNC_EQUAL:
                depthStencilState->m_depthFunc = GL_EQUAL;
                break;
                
            case FUNC_LESS_EQUAL:
                depthStencilState->m_depthFunc = GL_LEQUAL;
                break;
                
            case FUNC_GREATER:
                depthStencilState->m_depthFunc = GL_GREATER;
                break;
                
            case FUNC_NOT_EQUAL:
                depthStencilState->m_depthFunc = GL_NOTEQUAL;
                break;
                
            case FUNC_GREATER_EQUAL:
                depthStencilState->m_depthFunc = GL_GEQUAL;
                break;
                
            case FUNC_ALWAYS:
                depthStencilState->m_depthFunc = GL_ALWAYS;
                break;
                
            default:
                break;
        }
        
        depthStencilState->m_depthMask = (_depthMask) ? GL_TRUE : GL_FALSE;
        
        
        // Set Stencil Options
        
        if (_enableStencilTest)
            depthStencilState->m_enableStencil = true;
        else
            depthStencilState->m_enableStencil = false;
        
        switch (_frontStencilComparisonFunction)
        {
            case FUNC_NEVER:
                depthStencilState->m_frontStencilComparison = GL_NEVER;
                break;
                
            case FUNC_LESS:
                depthStencilState->m_frontStencilComparison = GL_LESS;
                break;
                
            case FUNC_EQUAL:
                depthStencilState->m_frontStencilComparison = GL_EQUAL;
                break;
                
            case FUNC_LESS_EQUAL:
                depthStencilState->m_frontStencilComparison = GL_LEQUAL;
                break;
                
            case FUNC_GREATER:
                depthStencilState->m_frontStencilComparison = GL_GREATER;
                break;
                
            case FUNC_NOT_EQUAL:
                depthStencilState->m_frontStencilComparison = GL_NOTEQUAL;
                break;
                
            case FUNC_GREATER_EQUAL:
                depthStencilState->m_frontStencilComparison = GL_GEQUAL;
                break;
                
            case FUNC_ALWAYS:
                depthStencilState->m_frontStencilComparison = GL_ALWAYS;
                break;
                
            default:
                depthStencilState->m_frontStencilComparison = GL_LESS;
                break;
        }
        
        switch (_backStencilComparisonFunction)
        {
            case FUNC_NEVER:
                depthStencilState->m_backStencilComparison = GL_NEVER;
                break;
                
            case FUNC_LESS:
                depthStencilState->m_backStencilComparison = GL_LESS;
                break;
                
            case FUNC_EQUAL:
                depthStencilState->m_backStencilComparison = GL_EQUAL;
                break;
                
            case FUNC_LESS_EQUAL:
                depthStencilState->m_backStencilComparison = GL_LEQUAL;
                break;
                
            case FUNC_GREATER:
                depthStencilState->m_backStencilComparison = GL_GREATER;
                break;
                
            case FUNC_NOT_EQUAL:
                depthStencilState->m_backStencilComparison = GL_NOTEQUAL;
                break;
                
            case FUNC_GREATER_EQUAL:
                depthStencilState->m_backStencilComparison = GL_GEQUAL;
                break;
                
            case FUNC_ALWAYS:
                depthStencilState->m_backStencilComparison = GL_ALWAYS;
                break;
                
            default:
                depthStencilState->m_backStencilComparison = GL_LESS;
                break;
        }
        
        // Front Stencil Operation
        
        switch (_frontStencilFail)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_frontStencilFail = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_frontStencilFail = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_frontStencilFail = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_frontStencilFail = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_frontStencilFail = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_frontStencilFail = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_frontStencilFail = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_frontStencilFail = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_frontStencilFail = GL_KEEP;
                break;
        }
        
        switch (_frontStencilPassDepthPass)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_frontStencilPassDepthPass = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_frontStencilPassDepthPass = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_frontStencilPassDepthPass = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_frontStencilPassDepthPass = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_frontStencilPassDepthPass = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_frontStencilPassDepthPass = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_frontStencilPassDepthPass = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_frontStencilPassDepthPass = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_frontStencilPassDepthPass = GL_KEEP;
                break;
        }
        
        switch (_frontStencilPassDepthFail)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_frontStencilPassDepthFail = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_frontStencilPassDepthFail = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_frontStencilPassDepthFail = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_frontStencilPassDepthFail = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_frontStencilPassDepthFail = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_frontStencilPassDepthFail = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_frontStencilPassDepthFail = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_frontStencilPassDepthFail = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_frontStencilPassDepthFail = GL_KEEP;
                break;
        }
        
        // Back Stencil Operation
        
        switch (_backStencilFail)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_backStencilFail = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_backStencilFail = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_backStencilFail = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_backStencilFail = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_backStencilFail = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_backStencilFail = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_backStencilFail = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_backStencilFail = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_backStencilFail = GL_KEEP;
                break;
        }
        
        switch (_backStencilPassDepthPass)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_backStencilPassDepthPass = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_backStencilPassDepthPass = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_backStencilPassDepthPass = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_backStencilPassDepthPass = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_backStencilPassDepthPass = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_backStencilPassDepthPass = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_backStencilPassDepthPass = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_backStencilPassDepthPass = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_backStencilPassDepthPass = GL_KEEP;
                break;
        }
        
        switch (_backStencilPassDepthFail)
        {
            case STENCIL_OP_KEEP:
                depthStencilState->m_backStencilPassDepthFail = GL_KEEP;
                break;
                
            case STENCIL_OP_ZERO:
                depthStencilState->m_backStencilPassDepthFail = GL_ZERO;
                break;
                
            case STENCIL_OP_REPLACE:
                depthStencilState->m_backStencilPassDepthFail = GL_REPLACE;
                break;
                
            case STENCIL_OP_INCR_SAT:
                depthStencilState->m_backStencilPassDepthFail = GL_INCR;
                break;
                
            case STENCIL_OP_DECR_SAT:
                depthStencilState->m_backStencilPassDepthFail = GL_DECR;
                break;
                
            case STENCIL_OP_INVERT:
                depthStencilState->m_backStencilPassDepthFail = GL_INVERT;
                break;
                
            case STENCIL_OP_INCR:
                depthStencilState->m_backStencilPassDepthFail = GL_INCR_WRAP;
                break;
                
            case STENCIL_OP_DECR:
                depthStencilState->m_backStencilPassDepthFail = GL_DECR_WRAP;
                break;
                
            default:
                depthStencilState->m_backStencilPassDepthFail = GL_KEEP;
                break;
        }
        
        return handle;
    }
    
    ResourceHandle CreateSamplerState(TextureFilteringMode _minFilter,
                                      TextureFilteringMode _magFilter,
                                      TextureWrapMode _wrapModeU,
                                      TextureWrapMode _wrapModeV,
                                      TextureWrapMode _wrapModeW,
                                      float _maxAnisotropy,
                                      float _borderRed,
                                      float _borderGreen,
                                      float _borderBlue,
                                      float _borderAlpha)
    {
        ResourceHandle handle = m_SamplerStatePool.add();
        SamplerState& samplerState = m_SamplerStatePool.lookup(handle);
        
        glGenSamplers(1, &samplerState.m_id);
        
        switch(_wrapModeU)
        {
            case REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
                break;
                
            case MIRRORED_REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                break;
                
            case CLAMP_TO_EDGE:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                break;
                
            case CLAMP_TO_BORDER:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                GLfloat borderColor[] = { _borderRed, _borderGreen, _borderBlue, _borderAlpha };
                glSamplerParameterfv(samplerState.m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
                break;
        }
        
        switch(_wrapModeV)
        {
            case REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
                
            case MIRRORED_REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
                
            case CLAMP_TO_EDGE:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
                
            case CLAMP_TO_BORDER:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                GLfloat borderColor[] = { _borderRed, _borderGreen, _borderBlue, _borderAlpha };
                glSamplerParameterfv(samplerState.m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
                break;
        }
        
        switch(_wrapModeW)
        {
            case REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
                break;
                
            case MIRRORED_REPEAT:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
                break;
                
            case CLAMP_TO_EDGE:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                break;
                
            case CLAMP_TO_BORDER:
                glSamplerParameteri(samplerState.m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
                GLfloat borderColor[] = { _borderRed, _borderGreen, _borderBlue, _borderAlpha };
                glSamplerParameterfv(samplerState.m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
                break;
        }
        
        if(_minFilter == LINEAR_ALL && _magFilter == LINEAR_ALL)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else if(_minFilter == LINEAR_ALL && _magFilter == NEAREST_ALL)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else if(_minFilter == NEAREST_ALL && _magFilter == LINEAR_ALL)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
        else if(_minFilter == NEAREST_ALL && _magFilter == NEAREST_ALL)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
        else if(_minFilter == LINEAR_MIPMAP_NEAREST && _magFilter == LINEAR_MIPMAP_NEAREST)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }
        else if(_minFilter == NEAREST_MIPMAP_LINEAR && _magFilter == NEAREST_MIPMAP_LINEAR)
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        }
        else
        {
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(samplerState.m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        
        if(GLEW_EXT_texture_filter_anisotropic)
        {
            GLfloat maxAnisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            
            if (_maxAnisotropy <= maxAnisotropy)
                glSamplerParameterf(samplerState.m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, _maxAnisotropy);
            else
                glSamplerParameterf(samplerState.m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        }
        
        return handle;
    }
    
    ResourceHandle CreateShaderBase(char* _Data, GLenum _shaderType)
    {
        ResourceHandle handle = m_ShaderPool.add();
        
        Shader& shader = m_ShaderPool.lookup(handle);
        shader.m_id = glCreateShader(_shaderType);
        shader.m_source = std::string(_Data);
        
        GLint success;
        GLchar infoLog[512];
        
        const GLchar* source = (GLchar*)_Data;
        
        glShaderSource(shader.m_id, 1, &source, NULL);
        glCompileShader(shader.m_id);
        glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);
        
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(shader.m_id, 512, NULL, infoLog);
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
        
        ShaderProgram& program = m_ShaderProgramPool.lookup(handle);
        
        glAttachShader(program.m_id, m_ShaderPool.lookup(_Vertex).m_id);
        glAttachShader(program.m_id, m_ShaderPool.lookup(_Pixel).m_id);
        
        if(_Geometry != USHRT_MAX)
        {
            glAttachShader(program.m_id, m_ShaderPool.lookup(_Geometry).m_id);
        }
        if(_TessellationControl != USHRT_MAX)
        {
            glAttachShader(program.m_id, m_ShaderPool.lookup(_TessellationControl).m_id);
        }
        if(_TessellationEvalution != USHRT_MAX)
        {
            glAttachShader(program.m_id, m_ShaderPool.lookup(_TessellationEvalution).m_id);
        }
        
        glLinkProgram(program.m_id);
        
        GLint success;
        char infoLog[512];
        
        glGetProgramiv(program.m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program.m_id, 512, NULL, infoLog);
            return USHRT_MAX;
        }
        
        Shader* shader_list[5];
        int count = 0;
        
        program.m_shader_count = -1;
        
        if(HANDLE_VALID(_Vertex))
        {
            shader_list[count] = &m_ShaderPool.lookup(_Vertex);
            count++;
            
            program.m_shader_count++;
            program.m_shaders[program.m_shader_count] = _Vertex;
        }
        if(HANDLE_VALID(_Pixel))
        {
            shader_list[count] = &m_ShaderPool.lookup(_Pixel);
            count++;
            
            program.m_shader_count++;
            program.m_shaders[program.m_shader_count] = _Pixel;
        }
        if(HANDLE_VALID(_Geometry))
        {
            shader_list[count] = &m_ShaderPool.lookup(_Geometry);
            count++;
            
            program.m_shader_count++;
            program.m_shaders[program.m_shader_count] = _Geometry;
        }
        if(HANDLE_VALID(_TessellationControl))
        {
            shader_list[count] = &m_ShaderPool.lookup(_TessellationControl);
            count++;
            
            program.m_shader_count++;
            program.m_shaders[program.m_shader_count] = _TessellationControl;
        }
        if(HANDLE_VALID(_TessellationEvalution))
        {
            shader_list[count] = &m_ShaderPool.lookup(_TessellationEvalution);
            count++;
            
            program.m_shader_count++;
            program.m_shaders[program.m_shader_count] = _TessellationEvalution;
        }

        // Bind Uniform Buffers
        for (int i = 0; i < count; i++)
        {
            Terminus::StringVector uboList = Terminus::StringUtility::find_line("#binding", shader_list[i]->m_source);
            
            for (int i = 0; i < uboList.size(); i++)
            {
                Terminus::StringVector tokens = Terminus::StringUtility::delimit(" ", uboList[i]);
                std::string uniformName = tokens[3];
                GLuint binding = (GLuint)atoi(tokens[5].c_str());
                const GLchar* uniformNameChar = uniformName.c_str();
               
                GLuint uboIndex = glGetUniformBlockIndex(program.m_id, uniformNameChar);
                glUniformBlockBinding(program.m_id, uboIndex, binding);
            }
        }
        
        // Bind Uniform Samplers
        for (int i = 0; i < count; i++)
        {
            Terminus::StringVector samplerList = Terminus::StringUtility::find_line("uniform sampler", shader_list[i]->m_source);
            
            for (int i = 0; i < samplerList.size(); i++)
            {
                Terminus::StringVector tokens = Terminus::StringUtility::delimit(" ", samplerList[i]);
                std::string uniformName = tokens[2];
                uniformName.erase(uniformName.end() - 1, uniformName.end());
                
                GLuint binding = (GLuint)atoi(tokens[4].c_str());
                const GLchar* uniformNameChar = uniformName.c_str();
                GLuint location = glGetUniformLocation(program.m_id, uniformNameChar);
                shader_list[i]->m_sampler_bindings[binding] = location;
            }
        }
        
        return handle;
	}

	void SetTexture2DMipmap(ResourceHandle _Texture2D, int _Level, void* _Data)
	{
        assert(_Texture2D != USHRT_MAX);
        
        Texture2D& texture = m_Texture2DPool.lookup(_Texture2D);
        
		glBindTexture(GL_TEXTURE_2D, texture.m_id);
		glTexImage2D(GL_TEXTURE_2D, _Level, GL_RGBA, texture.m_Width, texture.m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SetTextureCubeMipmap(ResourceHandle _TextureCube, int _Level, void* _Data)
	{
        assert(_TextureCube != USHRT_MAX);
        
        TextureCube& texture = m_TextureCubePool.lookup(_TextureCube);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.m_id);
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
