#include "D3D11RenderDevice.h"

#if defined(TERMINUS_DIRECT3D11) && defined(WIN32)

namespace Terminus { namespace Graphics {

	RenderDevice::RenderDevice()
	{

	}
	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::Initialize(void* memory, size_t size)
	{
		m_window = PlatformBackend::GetWindow();
		assert(InitializeAPI());
	}

	bool RenderDevice::InitializeAPI()
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator = 0, denominator = 0;
		size_t stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		float fieldOfView, screenAspect;

		m_vsync = false;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)PlatformBackend::GetWidth())
			{
				if (displayModeList[i].Width == (unsigned int)PlatformBackend::GetHeight())
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		m_video_card_memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		error = wcstombs_s(&stringLength, m_VideoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = 0;

		adapter->Release();
		adapter = 0;

		factory->Release();
		factory = 0;

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;

		swapChainDesc.BufferDesc.Width = PlatformBackend::GetWidth();
		swapChainDesc.BufferDesc.Height = PlatformBackend::GetHeight();

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (m_vsync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = PlatformBackend::GetHandleWin32();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		if (false)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_device, NULL, &m_device_context);

		if (FAILED(result))
		{
			return false;
		}

		result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

		if (FAILED(result))
		{
			return false;
		}

		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView);

		if (FAILED(result))
		{
			return false;
		}

		backBufferPtr->Release();
		backBufferPtr = 0;
	}

	Texture1D* RenderDevice::CreateTexture1D(uint16 width,
										     void* data,
										     TextureFormat format,
										     bool generateMipmaps,
										     uint mipMapLevels)
	{
		return nullptr;
	}

	Texture2D* RenderDevice::CreateTexture2D(uint16 width,
											 uint16 height,
											 void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{
		Texture2D* texture = new Texture2D();

		if (generateMipmaps)
		{
			if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
			{
				texture->m_textureDesc.MipLevels = 0;
				texture->m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2D.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2D.MipLevels = -1;
			}
			else
			{
				texture->m_textureDesc.MipLevels = mipMapLevels;
				texture->m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2D.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2D.MipLevels = (mipMapLevels == 0) ? -1 : mipMapLevels;
			}
		}

		texture->m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		switch (format)
		{
		case TextureFormat::R32G32B32_FLOAT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;

		case TextureFormat::R32G32B32A32_FLOAT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;

		case TextureFormat::R32G32B32_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			break;

		case TextureFormat::R32G32B32A32_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			break;

		case TextureFormat::R32G32B32_INT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			break;

		case TextureFormat::R32G32B32A32_INT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			break;

		case TextureFormat::R16G16B16A16_FLOAT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;

		case TextureFormat::R16G16B16A16_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
			break;

		case TextureFormat::R16G16B16A16_INT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R16G16B16A16_SINT;
			break;

		case TextureFormat::R8G8B8A8_UNORM:
			texture->m_textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;

		case TextureFormat::R8G8B8A8_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
			break;

		case TextureFormat::D32_FLOAT_S8_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			texture->m_textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;;
			texture->m_textureDesc.MipLevels = 1;
			texture->m_textureDesc.MiscFlags = 0;
			break;

		case TextureFormat::D24_FLOAT_S8_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture->m_textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;;
			texture->m_textureDesc.MipLevels = 1;
			texture->m_textureDesc.MiscFlags = 0;
			break;

		case TextureFormat::D16_FLOAT:
			texture->m_textureDesc.Format = DXGI_FORMAT_D16_UNORM;
			texture->m_textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;;
			texture->m_textureDesc.MipLevels = 1;
			texture->m_textureDesc.MiscFlags = 0;
			break;

		default:
			assert(false);
			break;
		}


		// Set data type depending on bpp property of extraData
		texture->m_textureDesc.Height = height;
		texture->m_textureDesc.Width = width;
		texture->m_textureDesc.ArraySize = 1;
		texture->m_textureDesc.SampleDesc.Count = 1;
		texture->m_textureDesc.SampleDesc.Quality = 0;
		texture->m_textureDesc.Usage = D3D11_USAGE_DEFAULT;
		texture->m_textureDesc.CPUAccessFlags = 0;

		texture->m_srvDesc.Format = texture->m_textureDesc.Format;
		texture->m_srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		texture->m_dsDesc.Format = texture->m_textureDesc.Format;
		texture->m_dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		texture->m_dsDesc.Texture2D.MipSlice = 0;

		texture->m_rtvDesc.Format = texture->m_textureDesc.Format;
		texture->m_rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		texture->m_rtvDesc.Texture2D.MipSlice = 0;

		if (format == TextureFormat::D24_FLOAT_S8_UINT)
		{
			texture->m_textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			texture->m_srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			texture->m_srvDesc.Texture2D.MipLevels = texture->m_textureDesc.MipLevels;
			texture->m_srvDesc.Texture2D.MostDetailedMip = 0;

			texture->m_dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture->m_dsDesc.Flags = 0;
		}

		HRESULT Result = m_device->CreateTexture2D(&texture->m_textureDesc, NULL, &texture->m_textureD3D);
		if (FAILED(Result))
		{
			//T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		Result = m_device->CreateShaderResourceView(texture->m_textureD3D, &texture->m_srvDesc, &texture->m_textureView);
		if (FAILED(Result))
		{
			//T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
		{
			Result = m_device->CreateDepthStencilView(texture->m_textureD3D, &texture->m_dsDesc, &texture->m_depthView);
			if (FAILED(Result))
			{
				//T_LOG_ERROR("Failed to Create Texture2D!");
				return nullptr;
			}
		}
		else
		{
			Result = m_device->CreateRenderTargetView(texture->m_textureD3D, &texture->m_rtvDesc, &texture->m_renderTargetView);
			if (FAILED(Result))
			{
				//T_LOG_ERROR("Failed to Create Texture2D!");
				return nullptr;
			}
		}

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
		return nullptr;
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
		VertexBuffer* vertexBuffer = new VertexBuffer();

		vertexBuffer->m_type = BufferType::VERTEX;
		vertexBuffer->m_usageType = usageType;

		switch (usageType)
		{
		case BufferUsageType::STATIC:
			vertexBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DEFAULT;
			break;

		case BufferUsageType::DYNAMIC:
			vertexBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DYNAMIC;
			break;
		}

		if (vertexBuffer->m_type != BufferType::UNIFORM)
			vertexBuffer->m_BufferDescD3D.CPUAccessFlags = 0;

		vertexBuffer->m_BufferDescD3D.MiscFlags = 0;
		vertexBuffer->m_BufferDescD3D.StructureByteStride = 0;

		vertexBuffer->m_BufferDescD3D.ByteWidth = size;
		vertexBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		vertexBuffer->m_subresourceDataD3D.pSysMem = data;
		vertexBuffer->m_subresourceDataD3D.SysMemPitch = 0;
		vertexBuffer->m_subresourceDataD3D.SysMemSlicePitch = 0;

		HRESULT result = m_device->CreateBuffer(&vertexBuffer->m_BufferDescD3D, &vertexBuffer->m_subresourceDataD3D, &vertexBuffer->m_BufferD3D);

		if (FAILED(result))
		{
			T_SAFE_DELETE(vertexBuffer);
			//T_LOG_ERROR("Failed to Create Vertex Buffer!");
			return nullptr;
		}

		//T_LOG_INFO("Successfully Created Vertex Buffer");

		return vertexBuffer;
	}

	IndexBuffer* RenderDevice::CreateIndexBuffer(void* data,
												 uint size,
												 BufferUsageType usageType)
	{

	}

	UniformBuffer* RenderDevice::CreateUniformBuffer(void* data,
													 uint size,
													 BufferUsageType usageType)
	{

	}

	VertexArray* RenderDevice::CreateVertexArray(VertexBuffer* vertexBuffer,
												 IndexBuffer* indexBuffer,
												 InputLayoutType layoutType,
												 InputLayout* layout)
	{

	}

	RasterizerState* RenderDevice::CreateRasterizerState(CullMode cullMode,
														 FillMode fillMode,
														 bool frontWindingCCW,
														 bool multisample,
														 bool scissor)
	{

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

	}

	Framebuffer* RenderDevice::CreateFramebuffer()
	{

	}

	Shader* RenderDevice::CreateShader(ShaderType type,
									   const char* shaderSource)
	{

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

	}

	void RenderDevice::AttachRenderTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{

	}

	void RenderDevice::AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{

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

	void RenderDevice::SetPrimitiveType(DrawPrimitive primitive)
	{

	}

	void RenderDevice::ClearFramebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor)
	{

	}

	void RenderDevice::SetViewport(int width, int height, int topLeftX, int topLeftY)
	{

	}

	void RenderDevice::SwapBuffers()
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