#pragma once

#include <core/config.h>

#ifdef TERMINUS_DIRECT3D11

#include <d3d11.h>
#include <d3dcompiler.h>
#include <unordered_map>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef TERMINUS_ENABLE_ERROR_CHECK
#define D3D11_CHECK_ERROR(x)	x; 
#else
#define D3D11_CHECK_ERROR(x)	x; 
#endif

namespace terminus
{
	struct Texture;

	using RenderTargetViewList = std::vector<ID3D11RenderTargetView*>;
	using RenderTargetList = std::vector<Texture*>;

    struct Texture
    {
		uint16					m_resource_id;
		TextureTarget			 m_type;
		ID3D11ShaderResourceView* m_textureView;
		ID3D11DepthStencilView* m_depthView;
		ID3D11RenderTargetView* m_renderTargetView;
		D3D11_SHADER_RESOURCE_VIEW_DESC m_srvDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC m_dsDesc;
		D3D11_RENDER_TARGET_VIEW_DESC m_rtvDesc;
    };
    
    struct Texture1D : Texture
    {
		int m_width;
    };
    
    struct Texture2D : Texture
    {
		int m_width;
		int m_height;
		D3D11_TEXTURE2D_DESC m_textureDesc;
		ID3D11Texture2D* m_textureD3D;
    };
    
    struct Texture3D : Texture
    {
		int m_width;
		int m_height;
		int m_depth;
    };
    
    struct TextureCube : Texture
    {
        
    };
    
    struct Buffer
    {
		uint16 m_resource_id;
		ID3D11Buffer* m_BufferD3D;
		D3D11_BUFFER_DESC m_BufferDescD3D;
		BufferType m_type;
		BufferUsageType m_usageType;
    };
    
    struct VertexBuffer : Buffer
    {
		D3D11_SUBRESOURCE_DATA m_subresourceDataD3D;
		unsigned int m_stride;
		unsigned int m_offset;
    };
    
    struct IndexBuffer : Buffer
    {
		D3D11_SUBRESOURCE_DATA m_subresourceDataD3D;
    };
    
    struct UniformBuffer : Buffer
    {
        
    };
    
    struct VertexArray
    {
		uint16 m_resource_id;
		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
		ID3D11InputLayout* m_inputLayoutD3D;
    };
    
    struct Shader
    {
		ID3D11VertexShader* m_vertexShader;
		ID3D11GeometryShader* m_geometryShader;
		ID3D11DomainShader* m_domainShader;
		ID3D11HullShader* m_hullShader;
		ID3D11ComputeShader* m_computeShader;
		ID3D11PixelShader* m_pixelShader;
		std::string m_shaderSource;
		ShaderType m_type;
    };
    
    struct ShaderProgram
    {
		uint16 m_resource_id;
		std::unordered_map<ShaderType, Shader*> m_shaderMap;
    };
    
    struct RasterizerState
    {
		ID3D11RasterizerState* m_RasterizerStateD3D;
    };

	struct DepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC m_depthStencilStateDesc;
		ID3D11DepthStencilState* m_depthStencilStateD3D;
	};
    
    struct SamplerState
    {
		uint16 m_resource_id;
		ID3D11SamplerState* m_D3D11SamplerState;
		D3D11_SAMPLER_DESC m_samplerDesc;
    };
    
    struct Framebuffer
    {
		uint16 m_resource_id;
		RenderTargetViewList m_renderTargetViews;
		RenderTargetList m_renderTargets;
		ID3D11DepthStencilView* m_depthStecilView;
		Texture* m_depthStencilTarget;
    };
}

#endif
