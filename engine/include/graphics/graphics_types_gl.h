#pragma once

#include <core/config.h>

#include <graphics/render_common.h>
#include <io/logger.h>
#include <core/types.h>
#include <unordered_map>

#ifdef TERMINUS_OPENGL

#include <GL/glew.h>

#ifdef TERMINUS_ENABLE_ERROR_CHECK
#define GL_CHECK_ERROR(x)																				  \
x; {                                                                                                      \
	GLenum err(glGetError());																			  \
																										  \
	while (err != GL_NO_ERROR)																			  \
	{																									  \
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
		std::string formatted_error = "OPENGL ERROR : ";                                                  \
        formatted_error += error;                                                                         \
        T_LOG_ERROR(formatted_error);                                                                     \
		err = glGetError();																				  \
	}																									  \
}																										  
#else
#define GL_CHECK_ERROR(x)	x
#endif


namespace std
{
    template<>
    struct hash<ShaderType>
    {
        size_t operator()(const ShaderType& em) const
        {
            return std::hash<int>()((int)em);
        }
    };
}
        
namespace terminus
{
    
	struct Texture;
	struct Shader;
    
	using ShaderMap = std::unordered_map<ShaderType, Shader*>;
	using BindingMap = std::unordered_map<GLuint, GLuint>;
	using RenderTargetList = std::vector<Texture*>;

    struct Texture
    {
        uint16 m_resource_id;
		GLuint m_id;
		GLenum m_glTextureTarget;
    };
    
    struct Texture1D : Texture
    {
        
    };
    
    struct Texture2D : Texture
    {
		uint16_t m_Width;
		uint16_t m_Height;
    };
    
    struct Texture3D : Texture
    {
        
    };
    
    struct TextureCube : Texture
    {
        
    };
    
    struct Buffer
    {
        uint16 m_resource_id;
		GLuint m_id;
		GLenum UsageType;
		GLenum bufferType;
		void* Data;
		uint Size;
    };
    
    struct VertexBuffer : Buffer
    {
        
    };
    
    struct IndexBuffer : Buffer
    {
        
    };
    
    struct UniformBuffer : Buffer
    {
        
    };
    
    struct VertexArray
    {
        uint16 m_resource_id;
		GLuint m_id;
        VertexBuffer* _vb;
        IndexBuffer*  _ib;
    };
    
    struct Shader
    {
		GLuint m_id;
		BindingMap m_sampler_bindings;
		ShaderType m_type;
		std::string m_source;
    };
    
    struct ShaderProgram
    {
        uint16 m_resource_id;
		GLuint m_id;
		ShaderMap m_shader_map;
		int m_shader_count;
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
    
    struct SamplerState
    {
		GLuint m_id;
        uint16 m_resource_id;
    };
    
    struct Framebuffer
    {
        uint16 m_resource_id;
		GLuint m_id;
		RenderTargetList m_render_targets;
		Texture* m_depth_target;
    };
    
    struct GraphicsCommand
    {
        uint64 sort_key;
        GraphicsCommandType type;
        
    };
    
} // namespace terminus

#endif

