#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <vector>
#include "../Types.h"

#ifdef TERMINUS_OPENGL
#include "GLTypes.h"
#include "../../Third Party/glew/include/GL/glew.h"
#elif TERMINUS_DIRECT3D11
#include "D3D11Types.h"
#endif

namespace Terminus { namespace Graphics {
    
    class RenderDevice
    {
        
    private:
        
        // Object Pools
        std::vector<Texture1D>   	  m_Texture1DPool;
        std::vector<Texture2D>   	  m_Texture2DPool;
        std::vector<Texture3D>   	  m_Texture3DPool;
        std::vector<TextureCube> 	  m_TextureCubePool;
        std::vector<VertexBuffer>     m_VertexBufferPool;
        std::vector<IndexBuffer>      m_IndexBufferPool;
        std::vector<UniformBuffer>    m_UniformBufferPool;
        std::vector<VertexArray>      m_VertexArrayPool;
        std::vector<SamplerState>     m_SamplerStatePool;
        std::vector<RasterizerState>  m_RasterizerStatePool;
        std::vector<Framebuffer> 	  m_FramebufferPool;
        std::vector<Shader> 	      m_ShaderPool;
        std::vector<ShaderProgram>    m_ShaderProgramPool;
        
    public:
        
        RenderDevice();
        ~RenderDevice();
        
        // Object Creation
        auto  CreateTexture1D();
        auto  CreateTexture2D();
        auto  CreateTexture3D();
        auto  CreateTextureCube();
        auto  CreateVertexBuffer();
        auto  CreateIndexBuffer();
        auto  CreateUniformBuffer();
        auto  CreateRasterizerState();
        auto  CreateSamplerState();
        auto  CreateFramebuffer();
        auto  CreateShader();
        auto  CreateShaderProgram();
        
        // Object Destruction
        auto  DestroyTexture1D();
        void  DestroyTexture2D();
        void  DestroyTexture3D();
        void  DestroyTextureCube();
        void  DestroyVertexBuffer();
        void  DestroyIndexBuffer();
        void  DestroyUniformBuffer();
        void  DestroyRasterizerState();
        void  DestroySamplerState();
        void  DestroyFramebuffer();
        void  DestroyShader();
        void  DestoryShaderProgram();
        
        // Object Use
        void  BindTexture1D();
        void  BindTexture2D();
        void  BindTexture3D();
        void  BindTextureCube();
        void  BindUniformBuffer();
        void  BindRasterizerState();
        void  BindSamplerState();
        void  BindFramebuffer();
        void  BindShaderProgram();
        void* MapBuffer();
        void  UnmapBuffer();
        
        // Stateless Methods
        void  Draw();
        void  DrawIndexed();
        void  DrawIndexedBaseVertex();
        void  DrawInstanced();
        void  DrawIndexedInstanced();
        
    private:
        
        // Backend Specifics
#ifdef TERMINUS_OPENGL
        GLenum m_primitive_type;
        ShaderProgram m_current_program;
#elif TERMINUS_DIRECT3D11
        
#endif
        
    };
    
} }

#endif
