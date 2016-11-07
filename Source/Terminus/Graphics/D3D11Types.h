#pragma once

#ifndef D3D11TYPES_H
#define D3D11TYPES_H

#include "Config.h"

#ifdef TERMINUS_DIRECT3D11

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define CHECK_ERROR(x)	x; \

namespace Terminus { namespace Graphics {
    
    struct Texture
    {
        
    };
    
    struct Texture1D : Texture
    {
        
    };
    
    struct Texture2D : Texture
    {
        
    };
    
    struct Texture3D : Texture
    {
        
    };
    
    struct TextureCube : Texture
    {
        
    };
    
    struct Buffer
    {
        
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
        
    };
    
    struct Shader
    {
        
    };
    
    struct ShaderProgram
    {
        
    };
    
    struct RasterizerState
    {
        
    };
    
    struct SamplerState
    {
        
    };
    
    struct Framebuffer
    {
        
    };
    
} }

#endif

#endif