#pragma once

#ifndef GLTYPES_H
#define GLTYPES_H

#include "Config.h"
#include <unordered_map>

#ifdef TERMINUS_OPENGL

#include "../../Third Party/glew/include/GL/glew.h"

#define CHECK_ERROR(x)																					  \
x; int line = __LINE__;																					  \
{																										  \
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
		std::cerr << "GL_" << error.c_str() << " - FILE:" << __FILE__ << " , LINE:" << line << std::endl; \
		err = glGetError();																				  \
	}																									  \
}																										  \

namespace Terminus { namespace Graphics {
    
	using BindingMap = std::unordered_map<GLuint, GLuint>;
	using RenderTargetList = std::vector<Texture*>;

    struct Texture
    {
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
		GLuint m_id;
		GLenum UsageType;
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
		GLuint m_id;
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
		GLuint m_id;
		ResourceHandle m_shaders[5];
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
    };
    
    struct Framebuffer
    {
		GLuint m_id;
		RenderTargetList m_render_targets;
		Texture* m_depth_target;
    };
    
} }

#endif

#endif