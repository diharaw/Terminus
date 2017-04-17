#include <graphics/render_device_d3d11.h>
#include <core/context.h>
#include <core/global.h>
#include <algorithm>

#if defined(TERMINUS_DIRECT3D11) && defined(WIN32)

namespace terminus
{
	RenderDevice::RenderDevice()
	{

	}
	RenderDevice::~RenderDevice()
	{

	}

	void RenderDevice::initialize()
	{
		initialize_api();
	}

	void RenderDevice::shutdown()
	{
		if (m_swap_chain)
		{
			m_swap_chain->SetFullscreenState(false, NULL);
		}

		destroy_framebuffer(m_default_framebuffer);
		destroy_texture_2d(m_default_render_target);
		destroy_texture_2d(m_default_depth_target);

		if (m_device_context)
		{
			m_device_context->Release();
			m_device_context = 0;
		}

		if (m_device)
		{
			m_device->Release();
			m_device = 0;
		}

		if (m_swap_chain)
		{
			m_swap_chain->Release();
			m_swap_chain = 0;
		}
	}

	bool RenderDevice::initialize_api()
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
			if (displayModeList[i].Width == (unsigned int)context::get_platform()->get_width())
			{
				if (displayModeList[i].Width == (unsigned int)context::get_platform()->get_height())
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

		error = wcstombs_s(&stringLength, m_video_card_desc, 128, adapterDesc.Description, 128);
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

		swapChainDesc.BufferDesc.Width = context::get_platform()->get_width();
		swapChainDesc.BufferDesc.Height = context::get_platform()->get_height();

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
		swapChainDesc.OutputWindow = context::get_platform()->get_handle_win32();
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

		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swap_chain, &m_device, NULL, &m_device_context);

		if (FAILED(result))
		{
			return false;
		}

		// Create RenderTargetView of Default Framebuffer using the Swap Chain Back Buffer

		result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

		if (FAILED(result))
		{
			return false;
		}

		m_default_render_target = new Texture2D();
		m_default_depth_target = new Texture2D();
		m_default_framebuffer = new Framebuffer();
		m_default_framebuffer->m_renderTargets.push_back(m_default_render_target);

		ID3D11RenderTargetView* renderTargetView;

		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		m_default_framebuffer->m_renderTargetViews.push_back(renderTargetView);
		m_default_render_target->m_renderTargetView = renderTargetView;

		if (FAILED(result))
		{
			return false;
		}

		backBufferPtr->Release();
		backBufferPtr = 0;

		// Create DepthBuffer of Default Framebuffer

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = context::get_platform()->get_width();
		depthBufferDesc.Height = context::get_platform()->get_height();
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_default_depth_target->m_textureD3D);
		if (FAILED(result))
		{
			return false;
		}

		// Create DepthStencilView of Default Framebuffer

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* depthStencilView;

		result = m_device->CreateDepthStencilView(m_default_depth_target->m_textureD3D, &depthStencilViewDesc, &depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		m_default_depth_target->m_depthView = depthStencilView;
		m_default_framebuffer->m_depthStencilTarget = m_default_depth_target;
		m_default_framebuffer->m_depthStecilView = depthStencilView;

		
		_window_width = (float)context::get_platform()->get_width();
		_window_height = (float)context::get_platform()->get_height();

		return true;
	}

	PipelineStateObject* RenderDevice::create_pipeline_state_object(PipelineStateObjectCreateDesc desc)
	{
		PipelineStateObject* pso = new PipelineStateObject();

		pso->depth_stencil_state = create_depth_stencil_state(desc.depth_stencil_state);
		pso->rasterizer_state = create_rasterizer_state(desc.rasterizer_state);
		pso->primitive = desc.primitive;

		return pso;
	}

	Texture1D* RenderDevice::create_texture_1d(Texture1DCreateDesc desc)
	{
		return nullptr;
	}

	Texture2D* RenderDevice::create_texture_2d(Texture2DCreateDesc desc)
	{
		Texture2D* texture = new Texture2D();
		texture->m_resource_id = m_texture_res_id++;

		ZeroMemory(&texture->m_textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		ZeroMemory(&texture->m_srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		ZeroMemory(&texture->m_dsDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		ZeroMemory(&texture->m_rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		if (desc.generate_mipmaps)
		{
			if (desc.format == TextureFormat::D32_FLOAT_S8_UINT || desc.format == TextureFormat::D24_FLOAT_S8_UINT || desc.format == TextureFormat::D16_FLOAT)
			{
				texture->m_textureDesc.MipLevels = 0;
				texture->m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2D.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2D.MipLevels = -1;
			}
			else
			{
				texture->m_textureDesc.MipLevels = desc.mipmap_levels;
				texture->m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2D.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2D.MipLevels = (desc.mipmap_levels == 0) ? -1 : desc.mipmap_levels;
			}
		}

		texture->m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		switch (desc.format)
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
			texture->m_textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texture->m_textureDesc.MipLevels = 1;
			texture->m_textureDesc.MiscFlags = 0;
			break;

		case TextureFormat::D24_FLOAT_S8_UINT:
			texture->m_textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture->m_textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
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
		texture->m_textureDesc.Height = desc.height;
		texture->m_textureDesc.Width = desc.width;
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

		if (desc.format == TextureFormat::D24_FLOAT_S8_UINT)
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

		if (!desc.create_render_target_view)
		{
			int rowPitch = (desc.width * 4) * sizeof(unsigned char);
			m_device_context->UpdateSubresource(texture->m_textureD3D, 0, NULL, desc.data, rowPitch, 0);
		}

		// Create ShaderResourceView

		Result = m_device->CreateShaderResourceView(texture->m_textureD3D, &texture->m_srvDesc, &texture->m_textureView);
		if (FAILED(Result))
		{
			//T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		if (desc.create_render_target_view)
		{
			// Create DepthStencilView

			if (desc.format == TextureFormat::D32_FLOAT_S8_UINT || desc.format == TextureFormat::D24_FLOAT_S8_UINT || desc.format == TextureFormat::D16_FLOAT)
			{
				texture->m_renderTargetView = nullptr;

				Result = m_device->CreateDepthStencilView(texture->m_textureD3D, &texture->m_dsDesc, &texture->m_depthView);
				if (FAILED(Result))
				{
					//T_LOG_ERROR("Failed to Create Texture2D!");
					return nullptr;
				}
			}
			else // Create RenderTargetView
			{
				texture->m_depthView = nullptr;

				Result = m_device->CreateRenderTargetView(texture->m_textureD3D, &texture->m_rtvDesc, &texture->m_renderTargetView);
				if (FAILED(Result))
				{
					//T_LOG_ERROR("Failed to Create Texture2D!");
					return nullptr;
				}
			}
		}

		return texture;
	}

	Texture3D* RenderDevice::create_texture_3d(Texture3DCreateDesc desc)
	{
		return nullptr;
	}

	TextureCube* RenderDevice::create_texture_cube(TextureCubeCreateDesc desc)
	{
		//-------------------------------------------------------------------------------------------------------------------------------------
		// Create engine objects
		//-------------------------------------------------------------------------------------------------------------------------------------

		TextureCube* texture = new TextureCube();
		texture->m_resource_id = m_texture_res_id++;

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Intiailize desc structs.
		//-------------------------------------------------------------------------------------------------------------------------------------

		ZeroMemory(&texture->_desc, sizeof(D3D11_TEXTURE2D_DESC));
		ZeroMemory(&texture->m_srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		ZeroMemory(&texture->m_dsDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		ZeroMemory(&texture->m_rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Setup mipmapping options.
		//-------------------------------------------------------------------------------------------------------------------------------------

		if (desc.generate_mipmaps)
		{
			if (desc.format == TextureFormat::D32_FLOAT_S8_UINT || desc.format == TextureFormat::D24_FLOAT_S8_UINT || desc.format == TextureFormat::D16_FLOAT)
			{
				texture->_desc.MipLevels = 0;
				texture->_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2DArray.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2DArray.MipLevels = -1;
			}
			else
			{
				texture->_desc.MipLevels = desc.mipmap_levels;
				texture->_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				texture->m_srvDesc.Texture2DArray.MostDetailedMip = 0;
				texture->m_srvDesc.Texture2DArray.MipLevels = (desc.mipmap_levels == 0) ? -1 : desc.mipmap_levels;
			}
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Find DXGI texture format.
		//-------------------------------------------------------------------------------------------------------------------------------------

		texture->_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		texture->_desc.Format = get_dxgi_format(desc.format);

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Setup depth target specific options.
		//-------------------------------------------------------------------------------------------------------------------------------------

		switch (desc.format)
		{
		case TextureFormat::D32_FLOAT_S8_UINT:
			texture->_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texture->_desc.MipLevels = 1;
			texture->_desc.MiscFlags = 0;
			break;

		case TextureFormat::D24_FLOAT_S8_UINT:
			texture->_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texture->_desc.MipLevels = 1;
			texture->_desc.MiscFlags = 0;
			break;

		case TextureFormat::D16_FLOAT:
			texture->_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;;
			texture->_desc.MipLevels = 1;
			texture->_desc.MiscFlags = 0;
			break;

		default:
			assert(false);
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Fill out Texture2D desc.
		//-------------------------------------------------------------------------------------------------------------------------------------

		// Set data type depending on bpp property of extraData
		texture->_desc.Height = desc.height;
		texture->_desc.Width = desc.width;
		texture->_desc.ArraySize = 6;
		texture->_desc.SampleDesc.Count = 1;
		texture->_desc.SampleDesc.Quality = 0;
		texture->_desc.Usage = D3D11_USAGE_DEFAULT;
		texture->_desc.CPUAccessFlags = 0;
		texture->_desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Fill out shader resource view desc.
		//-------------------------------------------------------------------------------------------------------------------------------------

		texture->m_srvDesc.Format = texture->_desc.Format;
		texture->m_srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Fill out depth stencil view desc.
		//-------------------------------------------------------------------------------------------------------------------------------------

		texture->m_dsDesc.Format = texture->_desc.Format;
		texture->m_dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		texture->m_dsDesc.Texture2DArray.FirstArraySlice = 0;
		texture->m_dsDesc.Texture2DArray.MipSlice = 0;
		texture->m_dsDesc.Texture2DArray.ArraySize = 6;

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Fill out render target view desc.
		//-------------------------------------------------------------------------------------------------------------------------------------

		texture->m_rtvDesc.Format = texture->_desc.Format;
		texture->m_rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		texture->m_rtvDesc.Texture2DArray.FirstArraySlice = 0;
		texture->m_rtvDesc.Texture2DArray.MipSlice = 0;
		texture->m_rtvDesc.Texture2DArray.ArraySize = 6;

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Temporary fallbacks.
		//-------------------------------------------------------------------------------------------------------------------------------------

		if (desc.format == TextureFormat::D24_FLOAT_S8_UINT)
		{
			texture->_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			texture->m_srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			texture->m_srvDesc.Texture2DArray.MipLevels = texture->_desc.MipLevels;
			texture->m_srvDesc.Texture2DArray.MostDetailedMip = 0;

			texture->m_dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture->m_dsDesc.Flags = 0;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Setup intial data.
		//-------------------------------------------------------------------------------------------------------------------------------------

		D3D11_SUBRESOURCE_DATA pData[6];
		int rowPitch = (desc.width * 4) * sizeof(unsigned char);

		for (int i = 0; i < 6; i++)
		{
			//Pointer to the pixel data
			pData[i].pSysMem = desc.data[i];
			//Line width in bytes
			pData[i].SysMemPitch = rowPitch;
			// This is only used for 3d textures.
			pData[i].SysMemSlicePitch = 0;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Create Texture2D.
		//-------------------------------------------------------------------------------------------------------------------------------------

		HRESULT Result;

		if (!desc.create_render_target_view)
			Result = m_device->CreateTexture2D(&texture->_desc, NULL, &texture->_texture);
		else
			Result = m_device->CreateTexture2D(&texture->_desc, &pData[0], &texture->_texture);

		if (FAILED(Result))
		{
			T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Create shader resource view.
		//-------------------------------------------------------------------------------------------------------------------------------------

		Result = m_device->CreateShaderResourceView(texture->_texture, &texture->m_srvDesc, &texture->m_textureView);
		if (FAILED(Result))
		{
			T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		// Render target view specific initialization
		//-------------------------------------------------------------------------------------------------------------------------------------

		if (desc.create_render_target_view)
		{
			//-------------------------------------------------------------------------------------------------------------------------------------
			// Create depth stencil view. (if current TextureCube is of a depth-buffer format and has requested a render target view, a depth 
			// stencil view wil be created for it.)
			//-------------------------------------------------------------------------------------------------------------------------------------

			if (desc.format == TextureFormat::D32_FLOAT_S8_UINT || desc.format == TextureFormat::D24_FLOAT_S8_UINT || desc.format == TextureFormat::D16_FLOAT)
			{
				texture->m_depthView = nullptr;

				Result = m_device->CreateDepthStencilView(texture->_texture, &texture->m_dsDesc, &texture->m_depthView);
				if (FAILED(Result))
				{
					T_LOG_ERROR("Failed to Create DepthStencilView!");
					return nullptr;
				}
			}

			//-------------------------------------------------------------------------------------------------------------------------------------
			// Create render target view. (if current TextureCube is NOT of a depth-buffer format and has requested a render target view, a render 
			// target view wil be created for it.)
			//-------------------------------------------------------------------------------------------------------------------------------------

			else
			{
				texture->m_renderTargetView = nullptr;

				Result = m_device->CreateRenderTargetView(texture->_texture, &texture->m_rtvDesc, &texture->m_renderTargetView);
				if (FAILED(Result))
				{
					T_LOG_ERROR("Failed to Create RenderTargetView!");
					return nullptr;
				}
			}
		}

		return texture;
	}

	VertexBuffer* RenderDevice::create_vertex_buffer(BufferCreateDesc desc)
	{
		VertexBuffer* vertexBuffer = new VertexBuffer();
		vertexBuffer->m_resource_id = m_buffer_res_id++;

        ZeroMemory(&vertexBuffer->m_BufferDescD3D, sizeof(D3D11_BUFFER_DESC));
        
		vertexBuffer->m_type = BufferType::VERTEX;
		vertexBuffer->m_usageType = desc.usage_type;

		switch (desc.usage_type)
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

		vertexBuffer->m_BufferDescD3D.ByteWidth = desc.size;
		vertexBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		vertexBuffer->m_subresourceDataD3D.pSysMem = desc.data;
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

	IndexBuffer* RenderDevice::create_index_buffer(BufferCreateDesc desc)
	{
		IndexBuffer* indexBuffer = new IndexBuffer();
		indexBuffer->m_resource_id = m_buffer_res_id++;

        ZeroMemory(&indexBuffer->m_BufferDescD3D, sizeof(D3D11_BUFFER_DESC));
        
		indexBuffer->m_type = BufferType::VERTEX;
		indexBuffer->m_usageType = desc.usage_type;

		switch (desc.usage_type)
		{
		case BufferUsageType::STATIC:
			indexBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DEFAULT;
			break;

		case BufferUsageType::DYNAMIC:
			indexBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DYNAMIC;
			break;
		}

		if (indexBuffer->m_type != BufferType::UNIFORM)
			indexBuffer->m_BufferDescD3D.CPUAccessFlags = 0;

		indexBuffer->m_BufferDescD3D.MiscFlags = 0;
		indexBuffer->m_BufferDescD3D.StructureByteStride = 0;

		indexBuffer->m_BufferDescD3D.ByteWidth = desc.size;
		indexBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_INDEX_BUFFER;

		indexBuffer->m_subresourceDataD3D.pSysMem = desc.data;
		indexBuffer->m_subresourceDataD3D.SysMemPitch = 0;
		indexBuffer->m_subresourceDataD3D.SysMemSlicePitch = 0;

		HRESULT result = m_device->CreateBuffer(&indexBuffer->m_BufferDescD3D, &indexBuffer->m_subresourceDataD3D, &indexBuffer->m_BufferD3D);

		if (FAILED(result))
		{
			T_SAFE_DELETE(indexBuffer);
			//T_LOG_ERROR("Failed to Create Index Buffer!");
			return nullptr;
		}

		//T_LOG_INFO("Successfully Created Index Buffer");

		return indexBuffer;
	}

	UniformBuffer* RenderDevice::create_uniform_buffer(BufferCreateDesc desc)
	{
		UniformBuffer* uniformBuffer = new UniformBuffer();
		uniformBuffer->m_resource_id = m_buffer_res_id++;

        ZeroMemory(&uniformBuffer->m_BufferDescD3D, sizeof(D3D11_BUFFER_DESC));
        
		uniformBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		uniformBuffer->m_BufferDescD3D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		uniformBuffer->m_type = BufferType::VERTEX;
		uniformBuffer->m_usageType = desc.usage_type;

		switch (desc.usage_type)
		{
		case BufferUsageType::STATIC:
			uniformBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DEFAULT;
			break;

		case BufferUsageType::DYNAMIC:
			uniformBuffer->m_BufferDescD3D.Usage = D3D11_USAGE_DYNAMIC;
			break;
		}

		uniformBuffer->m_BufferDescD3D.MiscFlags = 0;
		uniformBuffer->m_BufferDescD3D.StructureByteStride = 0;
		uniformBuffer->m_BufferDescD3D.ByteWidth = desc.size;

		HRESULT result = m_device->CreateBuffer(&uniformBuffer->m_BufferDescD3D, NULL, &uniformBuffer->m_BufferD3D);

		if (FAILED(result))
		{
			T_SAFE_DELETE(uniformBuffer);
			//T_LOG_ERROR("Failed to Create Uniform Buffer!");
			return nullptr;
		}
		//T_LOG_INFO("Successfully Created Uniform Buffer");

		return uniformBuffer;
	}

	VertexArray* RenderDevice::create_vertex_array(VertexArrayCreateDesc desc)
	{
		VertexArray* vertexArray = new VertexArray();
		vertexArray->m_resource_id = m_vertex_array_res_id++;

		vertexArray->m_vertexBuffer = desc.vertex_buffer;
		vertexArray->m_indexBuffer = desc.index_buffer;

		if (desc.layout_type == InputLayoutType::STANDARD_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC inputElement[4];
            
            ZeroMemory(&inputElement[0], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[1], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[2], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[3], sizeof(D3D11_INPUT_ELEMENT_DESC));
            
			InputLayout layout;
			InputElement element;

			vertexArray->m_vertexBuffer->m_offset = 0;
			vertexArray->m_vertexBuffer->m_stride = sizeof(Vertex);

			// Vertices

			inputElement[0].SemanticName = "POSITION";
			inputElement[0].SemanticIndex = 0;
			inputElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElement[0].InputSlot = 0;
			inputElement[0].AlignedByteOffset = 0;
			inputElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[0].InstanceDataStepRate = 0;

			element.m_numSubElements = 4;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "POSITION";
			layout.m_Elements.push_back(element);

			// Tex Coords

			inputElement[1].SemanticName = "TEXCOORD";
			inputElement[1].SemanticIndex = 0;
			inputElement[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElement[1].InputSlot = 0;
			inputElement[1].AlignedByteOffset = offsetof(Vertex, m_TexCoord);
			inputElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[1].InstanceDataStepRate = 0;

			element.m_numSubElements = 2;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "TEXCOORD";
			layout.m_Elements.push_back(element);

			// Normals

			inputElement[2].SemanticName = "NORMAL";
			inputElement[2].SemanticIndex = 0;
			inputElement[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElement[2].InputSlot = 0;
			inputElement[2].AlignedByteOffset = offsetof(Vertex, m_Normal);
			inputElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[2].InstanceDataStepRate = 0;

			element.m_numSubElements = 3;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "NORMAL";
			layout.m_Elements.push_back(element);

			// Tangents

			inputElement[3].SemanticName = "TANGENT";
			inputElement[3].SemanticIndex = 0;
			inputElement[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElement[3].InputSlot = 0;
			inputElement[3].AlignedByteOffset = offsetof(Vertex, m_Tangent);
			inputElement[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[3].InstanceDataStepRate = 0;

			element.m_numSubElements = 4;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "TANGENT";
			layout.m_Elements.push_back(element);

			ID3D10Blob* bytecode = create_stub_shader(layout);

			HRESULT result = m_device->CreateInputLayout(inputElement, 4, bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vertexArray->m_inputLayoutD3D);

			bytecode->Release();
			bytecode = 0;

			if (FAILED(result))
			{
				T_LOG_ERROR("Failed to create Input Layout");
				return nullptr;
			}

			T_LOG_INFO("Successfully Created Vertex Array");

			return vertexArray;
		}
		else if (desc.layout_type == InputLayoutType::STANDARD_SKINNED_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC inputElement[6];
            
            ZeroMemory(&inputElement[0], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[1], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[2], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[3], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[4], sizeof(D3D11_INPUT_ELEMENT_DESC));
            ZeroMemory(&inputElement[5], sizeof(D3D11_INPUT_ELEMENT_DESC));
            
			InputLayout layout;
			InputElement element;

			vertexArray->m_vertexBuffer->m_offset = 0;
			vertexArray->m_vertexBuffer->m_stride = sizeof(SkeletalVertex);

			// Vertices

			inputElement[0].SemanticName = "POSITION";
			inputElement[0].SemanticIndex = 0;
			inputElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElement[0].InputSlot = 0;
			inputElement[0].AlignedByteOffset = 0;
			inputElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[0].InstanceDataStepRate = 0;

			element.m_numSubElements = 3;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "POSITION";
			layout.m_Elements.push_back(element);

			// Tex Coords

			inputElement[1].SemanticName = "TEXCOORD";
			inputElement[1].SemanticIndex = 0;
			inputElement[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElement[1].InputSlot = 0;
			inputElement[1].AlignedByteOffset = offsetof(Vertex, m_TexCoord);
			inputElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[1].InstanceDataStepRate = 0;

			element.m_numSubElements = 2;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "TEXCOORD";
			layout.m_Elements.push_back(element);

			// Normals

			inputElement[2].SemanticName = "NORMAL";
			inputElement[2].SemanticIndex = 0;
			inputElement[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElement[2].InputSlot = 0;
			inputElement[2].AlignedByteOffset = offsetof(Vertex, m_Normal);
			inputElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[2].InstanceDataStepRate = 0;

			element.m_numSubElements = 3;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "NORMAL";
			layout.m_Elements.push_back(element);

			// Tangents

			inputElement[3].SemanticName = "TANGENT";
			inputElement[3].SemanticIndex = 0;
			inputElement[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElement[3].InputSlot = 0;
			inputElement[3].AlignedByteOffset = offsetof(Vertex, m_Tangent);
			inputElement[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[3].InstanceDataStepRate = 0;

			element.m_numSubElements = 4;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "TANGENT";
			layout.m_Elements.push_back(element);


			// Bone Indices

			inputElement[4].SemanticName = "BLENDINDICES";
			inputElement[4].SemanticIndex = 0;
			inputElement[4].Format = DXGI_FORMAT_R32G32B32A32_UINT;
			inputElement[4].InputSlot = 0;
			inputElement[4].AlignedByteOffset = offsetof(Vertex, m_Tangent);
			inputElement[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[4].InstanceDataStepRate = 0;

			element.m_numSubElements = 4;
			element.m_type = BufferDataType::UINT;
			element.m_semanticName = "BLENDINDICES";
			layout.m_Elements.push_back(element);

			// Bone Weights

			inputElement[5].SemanticName = "BLENDWEIGHT";
			inputElement[5].SemanticIndex = 0;
			inputElement[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElement[5].InputSlot = 0;
			inputElement[5].AlignedByteOffset = offsetof(Vertex, m_Tangent);
			inputElement[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElement[5].InstanceDataStepRate = 0;

			element.m_numSubElements = 4;
			element.m_type = BufferDataType::FLOAT;
			element.m_semanticName = "BLENDWEIGHT";
			layout.m_Elements.push_back(element);

			ID3D10Blob* bytecode = create_stub_shader(layout);

			HRESULT result = m_device->CreateInputLayout(inputElement, 6, bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vertexArray->m_inputLayoutD3D);

			bytecode->Release();
			bytecode = 0;

			if (FAILED(result))
			{
				//T_LOG_ERROR("Failed to create Input Layout");
				return nullptr;
			}

			//T_LOG_INFO("Successfully Created Vertex Array");

			return vertexArray;
		}
		else if (desc.layout_type == InputLayoutType::CUSTOM_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC* inputElement = new D3D11_INPUT_ELEMENT_DESC[desc.layout->m_Elements.size()];
			
            for(int k = 0; k < desc.layout->m_Elements.size(); k++)
            {
                ZeroMemory(&inputElement[k], sizeof(D3D11_INPUT_ELEMENT_DESC));
            }
            
			vertexArray->m_vertexBuffer->m_offset = 0;
			vertexArray->m_vertexBuffer->m_stride = desc.layout->m_vertexSize;

			for (int i = 0; i < desc.layout->m_Elements.size(); i++)
			{
				inputElement[i].SemanticName = desc.layout->m_Elements[i].m_semanticName.c_str();
				inputElement[i].SemanticIndex = 0;
				inputElement[i].InputSlot = 0;
				inputElement[i].AlignedByteOffset = desc.layout->m_Elements[i].m_offset;
				inputElement[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				inputElement[i].InstanceDataStepRate = 0;

				if (desc.layout->m_Elements[i].m_numSubElements == 3 && desc.layout->m_Elements[i].m_type == BufferDataType::FLOAT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (desc.layout->m_Elements[i].m_numSubElements == 4 && desc.layout->m_Elements[i].m_type == BufferDataType::FLOAT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				else if (desc.layout->m_Elements[i].m_numSubElements == 3 && desc.layout->m_Elements[i].m_type == BufferDataType::INT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_SINT;
				}
				else if (desc.layout->m_Elements[i].m_numSubElements == 4 && desc.layout->m_Elements[i].m_type == BufferDataType::INT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;
				}
				else if (desc.layout->m_Elements[i].m_numSubElements == 3 && desc.layout->m_Elements[i].m_type == BufferDataType::UINT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_UINT;
				}
				else if (desc.layout->m_Elements[i].m_numSubElements == 4 && desc.layout->m_Elements[i].m_type == BufferDataType::UINT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
				}
				else
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			}

			ID3D10Blob* bytecode = create_stub_shader(*desc.layout);

			HRESULT result = m_device->CreateInputLayout(&inputElement[0], desc.layout->m_Elements.size(), bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vertexArray->m_inputLayoutD3D);

			delete[] inputElement;
			bytecode->Release();
			bytecode = 0;

			if (FAILED(result))
			{
				T_LOG_ERROR("Failed to create Input Layout");
				return nullptr;
			}

			T_LOG_INFO("Successfully Created Vertex Array");

			return vertexArray;
		}
		else
		{
			T_LOG_ERROR("Vertex Array Creation : Unknown layout type");
			return nullptr;
		}
	}

	RasterizerState* RenderDevice::create_rasterizer_state(RasterizerStateCreateDesc desc)
	{
		RasterizerState* rasterizerState = new RasterizerState();

		D3D11_RASTERIZER_DESC rasterizerDesc;
        
        ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = false;

		if (desc.cull_mode == CullMode::BACK)
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
		else if (desc.cull_mode == CullMode::FRONT)
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		else
			rasterizerDesc.CullMode = D3D11_CULL_NONE;

		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;

		if (desc.fill_mode == FillMode::SOLID)
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		else
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

		rasterizerDesc.FrontCounterClockwise = desc.front_winding_ccw;
		rasterizerDesc.MultisampleEnable = desc.multisample;
		rasterizerDesc.ScissorEnable = desc.scissor;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		HRESULT result = m_device->CreateRasterizerState(&rasterizerDesc, &rasterizerState->m_RasterizerStateD3D);
		if (FAILED(result))
		{
			T_LOG_ERROR("Failed to Create Rasterizer State");
			T_SAFE_DELETE(rasterizerState);
			return nullptr;
		}

		T_LOG_INFO("Successfully Created Rasterizer State");

		return rasterizerState;
	}

	SamplerState* RenderDevice::create_sampler_state(SamplerStateCreateDesc desc)
	{
		SamplerState* samplerState = new SamplerState();
		samplerState->m_resource_id = m_sampler_res_id++;
        
        ZeroMemory(&samplerState->m_samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		
		switch (desc.wrap_mode_u)
		{
		case TextureWrapMode::REPEAT:
			samplerState->m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			samplerState->m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			samplerState->m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			samplerState->m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerState->m_samplerDesc.BorderColor[0] = desc.border_color.x;
			samplerState->m_samplerDesc.BorderColor[1] = desc.border_color.y;
			samplerState->m_samplerDesc.BorderColor[2] = desc.border_color.z;
			samplerState->m_samplerDesc.BorderColor[3] = desc.border_color.w;
			break;
		}

		switch (desc.wrap_mode_v)
		{
		case TextureWrapMode::REPEAT:
			samplerState->m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			samplerState->m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			samplerState->m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			samplerState->m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerState->m_samplerDesc.BorderColor[0] = desc.border_color.x;
			samplerState->m_samplerDesc.BorderColor[1] = desc.border_color.y;
			samplerState->m_samplerDesc.BorderColor[2] = desc.border_color.z;
			samplerState->m_samplerDesc.BorderColor[3] = desc.border_color.w;
			break;
		}

		switch (desc.wrap_mode_w)
		{
		case TextureWrapMode::REPEAT:
			samplerState->m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			break;

		case TextureWrapMode::MIRRORED_REPEAT:
			samplerState->m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;

		case TextureWrapMode::CLAMP_TO_EDGE:
			samplerState->m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;

		case TextureWrapMode::CLAMP_TO_BORDER:
			samplerState->m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerState->m_samplerDesc.BorderColor[0] = desc.border_color.x;
			samplerState->m_samplerDesc.BorderColor[1] = desc.border_color.y;
			samplerState->m_samplerDesc.BorderColor[2] = desc.border_color.z;
			samplerState->m_samplerDesc.BorderColor[3] = desc.border_color.w;
			break;
		}

		if (desc.min_filter == TextureFilteringMode::LINEAR_ALL && desc.mag_filter == TextureFilteringMode::LINEAR_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		else if (desc.min_filter == TextureFilteringMode::LINEAR_ALL && desc.mag_filter == TextureFilteringMode::NEAREST_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		else if (desc.min_filter == TextureFilteringMode::NEAREST_ALL && desc.mag_filter == TextureFilteringMode::LINEAR_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		else if (desc.min_filter == TextureFilteringMode::NEAREST_ALL && desc.mag_filter == TextureFilteringMode::NEAREST_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		else if (desc.min_filter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST && desc.mag_filter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		else if (desc.min_filter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR && desc.mag_filter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		else
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		if (desc.max_anisotropy < 1.0)
			samplerState->m_samplerDesc.MaxAnisotropy = 1.0;
		else
			samplerState->m_samplerDesc.MaxAnisotropy = desc.max_anisotropy;

		HRESULT result;
		result = m_device->CreateSamplerState(&samplerState->m_samplerDesc, &samplerState->m_D3D11SamplerState);

		if (FAILED(result))
		{
			T_SAFE_DELETE(samplerState);
			T_LOG_ERROR("Failed to Create Sampler State!");
			return nullptr;
		}

		T_LOG_INFO("Successfully Created Sampler State!");
		return samplerState;
	}

	Framebuffer* RenderDevice::create_framebuffer(FramebufferCreateDesc desc)
	{
		Framebuffer* framebuffer = new Framebuffer();
		framebuffer->m_resource_id = m_framebuffer_res_id++;

		for (int i = 0; i < desc.num_render_targets; i++)
			attach_render_target(framebuffer, desc.render_targets[i]);

		if (desc.depth_target)
			attach_depth_stencil_target(framebuffer, desc.depth_target);

		return framebuffer;
	}

	Shader* RenderDevice::create_shader(ShaderCreateDesc desc)
	{
		Shader* shader = new Shader();
		shader->m_type = desc.type;
		shader->m_shaderSource = desc.shader_source;

		ID3D10Blob* shaderBytecode;
		ID3D10Blob* errorMessage;

		std::string entryPoint;
		std::string target;

		// TODO: handle different feature levels
		switch (desc.type)
		{
		case ShaderType::VERTEX:
			entryPoint = "VertexMain";
			target = "vs_5_0";
			break;

		case ShaderType::GEOMETRY:
			entryPoint = "GeometryMain";
			target = "gs_5_0";
			break;

		case ShaderType::TESSELLATION_CONTROL:
			entryPoint = "HullMain";
			target = "hs_5_0";
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			entryPoint = "DomainMain";
			target = "ds_5_0";
			break;

		case ShaderType::PIXEL:
			entryPoint = "PixelMain";
			target = "ps_5_0";
			break;
		}

		size_t size = strlen(desc.shader_source);

		HRESULT result = D3DCompile(desc.shader_source, size, NULL, NULL, NULL, entryPoint.c_str(), target.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBytecode, &errorMessage);

		if (FAILED(result))
		{
			T_SAFE_DELETE(shader);

			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			long bufferSize = errorMessage->GetBufferSize();

			std::string error = "";

			for (int i = 0; i < bufferSize; i++)
				error += compileErrors[i];

			//T_LOG_ERROR("Shader Compilation Failed : " + error);

			return nullptr;
		}

		switch (desc.type)
		{
		case ShaderType::VERTEX:
			result = m_device->CreateVertexShader(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize(), NULL, &shader->m_vertexShader);
			break;

		case ShaderType::GEOMETRY:
			result = m_device->CreateGeometryShader(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize(), NULL, &shader->m_geometryShader);
			break;

		case ShaderType::TESSELLATION_CONTROL:
			result = m_device->CreateHullShader(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize(), NULL, &shader->m_hullShader);
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			result = m_device->CreateDomainShader(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize(), NULL, &shader->m_domainShader);
			break;

		case ShaderType::PIXEL:
			result = m_device->CreatePixelShader(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize(), NULL, &shader->m_pixelShader);
			break;

		default:
			break;
		}

		if (FAILED(result))
		{
			T_SAFE_DELETE(shader);
			T_LOG_ERROR("Failed to create Shader");

			return nullptr;
		}

		return shader;
	}

	ShaderProgram* RenderDevice::create_shader_program(ShaderProgramCreateDesc desc)
	{
		ShaderProgram* shaderProgram = new ShaderProgram();
		shaderProgram->m_resource_id = m_shader_program_res_id++;

		shaderProgram->m_shaderMap[desc.vertex->m_type] = desc.vertex;
		shaderProgram->m_shaderMap[desc.pixel->m_type] = desc.pixel;

		if (desc.geometry)
			shaderProgram->m_shaderMap[desc.geometry->m_type] = desc.geometry;

		if (desc.tessellation_control && desc.tessellation_evaluation)
		{
			shaderProgram->m_shaderMap[desc.tessellation_control->m_type] = desc.tessellation_control;
			shaderProgram->m_shaderMap[desc.tessellation_evaluation->m_type] = desc.tessellation_evaluation;
		}

		T_LOG_INFO("Successfully Created Shader Program");

		return shaderProgram;
	}

	DepthStencilState* RenderDevice::create_depth_stencil_state(DepthStencilStateCreateDesc desc)
	{
		DepthStencilState* depthStencilState = new DepthStencilState();

        ZeroMemory(&depthStencilState->m_depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
        
		if (desc.enable_depth_test)
			depthStencilState->m_depthStencilStateDesc.DepthEnable = true;
		else
			depthStencilState->m_depthStencilStateDesc.DepthEnable = false;

		if (desc.depth_mask)
			depthStencilState->m_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			depthStencilState->m_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		switch (desc.depth_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
			break;
		}

		// Stencil

		if (desc.enable_stencil_test)
			depthStencilState->m_depthStencilStateDesc.StencilEnable = true;
		else
			depthStencilState->m_depthStencilStateDesc.StencilEnable = false;


		depthStencilState->m_depthStencilStateDesc.StencilReadMask = desc.stencil_mask;
		depthStencilState->m_depthStencilStateDesc.StencilWriteMask = desc.stencil_mask;

		// Front Stencil Comparison Function

		switch (desc.front_stencil_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
			break;
		}

		// Front Stencil Operation

		switch (desc.front_stencil_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		switch (desc.front_stencil_pass_depth_pass)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		switch (desc.front_stencil_pass_depth_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		// Back Stencil Comparison Function

		switch (desc.back_stencil_cmp_func)
		{
		case ComparisonFunction::NEVER:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			break;

		case ComparisonFunction::LESS:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
			break;

		case ComparisonFunction::EQUAL:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			break;

		case ComparisonFunction::LESS_EQUAL:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;

		case ComparisonFunction::GREATER:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
			break;

		case ComparisonFunction::NOT_EQUAL:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;

		case ComparisonFunction::GREATER_EQUAL:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;

		case ComparisonFunction::ALWAYS:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
			break;
		}

		// Back Stencil Operation

		switch (desc.back_stencil_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		switch (desc.back_stencil_pass_depth_pass)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		switch (desc.back_stencil_pass_depth_fail)
		{
		case StencilOperation::KEEP:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			break;

		case StencilOperation::ZERO:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			break;

		case StencilOperation::REPLACE:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			break;

		case StencilOperation::INCR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
			break;

		case StencilOperation::DECR_SAT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
			break;

		case StencilOperation::INVERT:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
			break;

		case StencilOperation::INCR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			break;

		case StencilOperation::DECR:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			break;

		default:
			depthStencilState->m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;
		}

		HRESULT result = m_device->CreateDepthStencilState(&depthStencilState->m_depthStencilStateDesc, &depthStencilState->m_depthStencilStateD3D);
		if (FAILED(result))
		{
			T_LOG_ERROR("Failed to Create Depth Stencil State");
			T_SAFE_DELETE(depthStencilState);
			return nullptr;
		}

		T_LOG_INFO("Successfully Created Depth Stencil State");

		return depthStencilState;
	}

	void RenderDevice::attach_render_target(Framebuffer* framebuffer, Texture* renderTarget)
	{
		ID3D11RenderTargetView* renderTargetView;

		switch (renderTarget->m_type)
		{
			case TextureTarget::TEXTURE2D:
			{
				Texture2D* texture = (Texture2D*)renderTarget;
				m_device->CreateRenderTargetView(texture->m_textureD3D, NULL, &renderTargetView);
				break;
			}
			default:
			{
				T_LOG_ERROR("Render Target binding Failed : Invalid Texture Target");
				return;
				break;
			}
		}

		framebuffer->m_renderTargets.push_back(renderTarget);
		framebuffer->m_renderTargetViews.push_back(((Texture2D*)renderTarget)->m_renderTargetView);
	}

	void RenderDevice::attach_depth_stencil_target(Framebuffer* framebuffer, Texture* renderTarget)
	{
		ID3D11DepthStencilView* depthStencilView;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        
        ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		// Check Format and set DS format accordingly
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		switch (renderTarget->m_type)
		{
			case TextureTarget::TEXTURE2D:
			{
				Texture2D* texture = (Texture2D*)renderTarget;
				m_device->CreateDepthStencilView(texture->m_textureD3D, &depthStencilViewDesc, &depthStencilView);
				break;
			}
			default:
			{
				T_LOG_ERROR("Render Target binding Failed : Invalid Texture Target");
				return;
				break;
			}
		}

		framebuffer->m_depthStencilTarget = renderTarget;
		framebuffer->m_depthStecilView = ((Texture2D*)renderTarget)->m_depthView;
	}

	void RenderDevice::destroy_pipeline_state_object(PipelineStateObject* pso)
	{
		destroy_depth_stencil_state(pso->depth_stencil_state);
		destroy_rasterizer_state(pso->rasterizer_state);

		delete pso;
	}

	void RenderDevice::destroy_texture_1d(Texture1D* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		delete texture;
	}

	void RenderDevice::destroy_texture_2d(Texture2D* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		if (texture->m_textureD3D)
			texture->m_textureD3D->Release();

		delete texture;
	}

	void RenderDevice::destroy_texture_3d(Texture3D* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		delete texture;
	}

	void RenderDevice::destroy_texture_cube(TextureCube* texture)
	{
		/*if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();*/

		delete texture;
	}

	void RenderDevice::destroy_vertex_buffer(VertexBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::destroy_index_buffer(IndexBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::destroy_uniform_buffer(UniformBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::destroy_vertex_array(VertexArray* vertexArray)
	{
		if (vertexArray)
		{
			if (vertexArray->m_vertexBuffer)
				destroy_vertex_buffer(vertexArray->m_vertexBuffer);

			if (vertexArray->m_indexBuffer)
				destroy_index_buffer(vertexArray->m_indexBuffer);
			
			T_SAFE_DELETE(vertexArray);
		}
	}

	void RenderDevice::destroy_rasterizer_state(RasterizerState* state)
	{
		if (state->m_RasterizerStateD3D)
			state->m_RasterizerStateD3D->Release();

		delete state;
	}

	void RenderDevice::destroy_sampler_state(SamplerState* state)
	{
		if (state->m_D3D11SamplerState)
			state->m_D3D11SamplerState->Release();

		delete state;
	}

	void RenderDevice::destroy_depth_stencil_state(DepthStencilState* state)
	{
		if (state->m_depthStencilStateD3D)
			state->m_depthStencilStateD3D->Release();

		delete state;
	}

	void RenderDevice::destroy_framebuffer(Framebuffer* framebuffer)
	{
		T_SAFE_DELETE(framebuffer);
	}

	void RenderDevice::destroy_shader(Shader* shader)
	{
		if (shader->m_computeShader)
			shader->m_computeShader->Release();

		if (shader->m_domainShader)
			shader->m_domainShader->Release();

		if (shader->m_geometryShader)
			shader->m_geometryShader->Release();

		if (shader->m_hullShader)
			shader->m_hullShader->Release();

		if (shader->m_pixelShader)
			shader->m_pixelShader->Release();

		if (shader->m_vertexShader)
			shader->m_vertexShader->Release();

		T_SAFE_DELETE(shader);
	}

	void RenderDevice::destory_shader_program(ShaderProgram* program)
	{
		for (auto it : program->m_shaderMap)
		{
			T_SAFE_DELETE(it.second);
		}
	}

	void RenderDevice::bind_pipeline_state_object(PipelineStateObject* pso)
	{
		bind_depth_stencil_state(pso->depth_stencil_state);
		bind_rasterizer_state(pso->rasterizer_state);
		set_primitive_type(pso->primitive);
	}

	void RenderDevice::bind_texture(Texture* texture, ShaderType shaderStage, uint32_t bufferSlot)
	{
		switch (shaderStage)
		{

		case ShaderType::VERTEX:
			m_device_context->VSSetShaderResources(bufferSlot, 1, &texture->m_textureView);
			break;

		case ShaderType::PIXEL:
			m_device_context->PSSetShaderResources(bufferSlot, 1, &texture->m_textureView);
			break;

		case ShaderType::GEOMETRY:
			m_device_context->GSSetShaderResources(bufferSlot, 1, &texture->m_textureView);
			break;

		case ShaderType::TESSELLATION_CONTROL:
			m_device_context->HSSetShaderResources(bufferSlot, 1, &texture->m_textureView);
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			m_device_context->DSSetShaderResources(bufferSlot, 1, &texture->m_textureView);
			break;

		default:
			break;
		}
	}

	void RenderDevice::bind_uniform_buffer(UniformBuffer* uniformBuffer, ShaderType shaderStage, uint32_t bufferSlot)
	{
		switch (shaderStage)
		{

		case ShaderType::VERTEX:
			m_device_context->VSSetConstantBuffers(bufferSlot, 1, &uniformBuffer->m_BufferD3D);
			break;

		case ShaderType::PIXEL:
			m_device_context->PSSetConstantBuffers(bufferSlot, 1, &uniformBuffer->m_BufferD3D);
			break;

		case ShaderType::GEOMETRY:
			m_device_context->GSSetConstantBuffers(bufferSlot, 1, &uniformBuffer->m_BufferD3D);
			break;

		case ShaderType::TESSELLATION_CONTROL:
			m_device_context->HSSetConstantBuffers(bufferSlot, 1, &uniformBuffer->m_BufferD3D);
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			m_device_context->DSSetConstantBuffers(bufferSlot, 1, &uniformBuffer->m_BufferD3D);
			break;

		default:
			break;
		}
	}

	void RenderDevice::bind_rasterizer_state(RasterizerState* state)
	{
		m_device_context->RSSetState(state->m_RasterizerStateD3D);
	}

	void RenderDevice::bind_sampler_state(SamplerState* state, ShaderType shaderStage, uint32_t slot)
	{
		switch (shaderStage)
		{

		case ShaderType::VERTEX:
			m_device_context->VSSetSamplers(slot, 1, &state->m_D3D11SamplerState);
			break;

		case ShaderType::PIXEL:
			m_device_context->PSSetSamplers(slot, 1, &state->m_D3D11SamplerState);
			break;

		case ShaderType::GEOMETRY:
			m_device_context->GSSetSamplers(slot, 1, &state->m_D3D11SamplerState);
			break;

		case ShaderType::TESSELLATION_CONTROL:
			m_device_context->HSSetSamplers(slot, 1, &state->m_D3D11SamplerState);
			break;

		case ShaderType::TESSELLATION_EVALUATION:
			m_device_context->DSSetSamplers(slot, 1, &state->m_D3D11SamplerState);
			break;

		default:
			break;
		}
	}

	void RenderDevice::bind_vertex_array(VertexArray* vertexArray)
	{
		m_device_context->IASetInputLayout(vertexArray->m_inputLayoutD3D);
		m_device_context->IASetVertexBuffers(0, 1, &vertexArray->m_vertexBuffer->m_BufferD3D,
												   &vertexArray->m_vertexBuffer->m_stride,
												   &vertexArray->m_vertexBuffer->m_offset);
		m_device_context->IASetIndexBuffer(vertexArray->m_indexBuffer->m_BufferD3D, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderDevice::bind_framebuffer(Framebuffer* framebuffer)
	{
		if (framebuffer)
			m_current_framebuffer = framebuffer;
		else
			m_current_framebuffer = m_default_framebuffer;
		
		m_device_context->OMSetRenderTargets(m_current_framebuffer->m_renderTargetViews.size(), &m_current_framebuffer->m_renderTargetViews[0], m_current_framebuffer->m_depthStecilView);
	}

	void RenderDevice::bind_depth_stencil_state(DepthStencilState* state)
	{
		m_device_context->OMSetDepthStencilState(state->m_depthStencilStateD3D, 1);
	}

	void RenderDevice::bind_shader_program(ShaderProgram* program)
	{
		m_device_context->VSSetShader(program->m_shaderMap[ShaderType::VERTEX]->m_vertexShader, NULL, 0);
		m_device_context->PSSetShader(program->m_shaderMap[ShaderType::PIXEL]->m_pixelShader, NULL, 0);

		if (program->m_shaderMap[ShaderType::GEOMETRY])
			m_device_context->GSSetShader(program->m_shaderMap[ShaderType::GEOMETRY]->m_geometryShader, NULL, 0);

		if (program->m_shaderMap[ShaderType::TESSELLATION_CONTROL] && program->m_shaderMap[ShaderType::TESSELLATION_EVALUATION])
		{
			m_device_context->HSSetShader(program->m_shaderMap[ShaderType::TESSELLATION_CONTROL]->m_hullShader, NULL, 0);
			m_device_context->DSSetShader(program->m_shaderMap[ShaderType::TESSELLATION_EVALUATION]->m_domainShader, NULL, 0);
		}
	}

	void* RenderDevice::map_buffer(Buffer* buffer, BufferMapType type)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		switch (type)
		{

		case BufferMapType::READ:
			result = m_device_context->Map(buffer->m_BufferD3D, 0, D3D11_MAP_READ, 0, &mappedResource);
			break;

		case BufferMapType::WRITE:
			result = m_device_context->Map(buffer->m_BufferD3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			break;

		case BufferMapType::READ_WRITE:
			result = m_device_context->Map(buffer->m_BufferD3D, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
			break;

		default:
			result = -1;
			break;
		}

		if (FAILED(result))
			return nullptr;
		else
			return (void*)mappedResource.pData;
	}

	void RenderDevice::unmap_buffer(Buffer* buffer)
	{
		m_device_context->Unmap(buffer->m_BufferD3D, 0);
	}

	void RenderDevice::set_primitive_type(DrawPrimitive primitive)
	{
		switch (primitive)
		{
		case DrawPrimitive::POINTS:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;

		case DrawPrimitive::TRIANGLES:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;

		case DrawPrimitive::TRIANGLE_STRIP:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;

		case DrawPrimitive::LINES:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;

		case DrawPrimitive::LINE_STRIP:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;

		default:
			m_primitive_type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		}

		m_device_context->IASetPrimitiveTopology(m_primitive_type);
	}

	void RenderDevice::clear_framebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor)
	{
		switch (clearTarget)
		{
		case FramebufferClearTarget::DEPTH:
			m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			break;

		case FramebufferClearTarget::ALL:
			{
				if (m_current_framebuffer)
				{
					for (auto it : m_current_framebuffer->m_renderTargetViews)
						m_device_context->ClearRenderTargetView(it, &clearColor.x);

					if (m_current_framebuffer->m_depthStecilView)
					{
						m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
						m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
					}
				}
				else
				{

				}
				
			}
			break;
		}
	}

	void RenderDevice::set_viewport(uint32_t width, uint32_t height, uint32_t top_left_x, uint32_t top_left_y)
	{
		D3D11_VIEWPORT viewport;

		if (width == 0 && height == 0)
		{
			viewport.Width = (float)_window_width;
			viewport.Height = (float)_window_height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = top_left_x;
			viewport.TopLeftY = top_left_y;
		}
		else
		{
			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = top_left_x;
			viewport.TopLeftY = top_left_y;
		}

		m_device_context->RSSetViewports(1, &viewport);
	}

	void RenderDevice::swap_buffers()
	{
		m_swap_chain->Present(0, 0);
	}

	void RenderDevice::draw(uint32_t first_index, uint32_t count)
	{
		m_device_context->Draw(count, first_index);
	}

	void RenderDevice::draw_indexed(uint32_t index_count)
	{
		m_device_context->DrawIndexed(index_count, 0, 0);
	}

	void RenderDevice::draw_indexed_base_vertex(uint32_t index_count, uint32_t base_index, uint32_t base_vertex)
	{
		m_device_context->DrawIndexed(index_count, base_index, base_vertex);
	}

	void RenderDevice::draw_instanced()
	{


	}

	void RenderDevice::draw_indexed_instanced()
	{

	}

	ID3D10Blob* RenderDevice::create_stub_shader(InputLayout _layout)
	{
		ID3D10Blob* vertexShaderByteCode;
		ID3D10Blob* errorMessage;

		std::string shaderSource = "";

		std::string inputStructureName = "struct VertexInputType \n ";

		std::string pixelInputAndMain = "struct PixelInputType\n"
			"{\n"
			"float4 position : SV_POSITION;\n"
			"float4 color : COLOR;\n"
			"};\n"
			"\n"
			"PixelInputType ColorVertexShader(VertexInputType input)\n"
			"{\n"
			"PixelInputType output; \n"
			"float4 pos = float4(0,0,2,1);"
			"output.position = pos;"
			"output.color = pos;"
			"return output;\n"
			"}\n";

		shaderSource = "struct VertexInputType\n"
			"{\n";

		for (auto element : _layout.m_Elements)
		{
			std::string inputVarType;
			std::string lowercaseSemantic;

			lowercaseSemantic.resize(element.m_semanticName.size());

			std::transform(element.m_semanticName.begin(), element.m_semanticName.end(), lowercaseSemantic.begin(), ::tolower);

			switch (element.m_type)
			{
			case BufferDataType::FLOAT:
				inputVarType = "float" + std::to_string(element.m_numSubElements);
				break;

			case BufferDataType::INT:
				inputVarType = "int" + std::to_string(element.m_numSubElements);
				break;

			case BufferDataType::UINT:
				inputVarType = "uint" + std::to_string(element.m_numSubElements);
				break;

			default:
				break;
			}

			shaderSource += inputVarType + " " + lowercaseSemantic + " : " + element.m_semanticName + ";\n";
		}
		shaderSource += "};\n";
		shaderSource += "\n";
		shaderSource += pixelInputAndMain;

		HRESULT result = D3DCompile(shaderSource.c_str(), shaderSource.size(), NULL, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderByteCode, &errorMessage);

		if (FAILED(result))
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			long bufferSize = errorMessage->GetBufferSize();

			std::string error = "";

			for (int i = 0; i < bufferSize; i++)
				error += compileErrors[i];

			//T_LOG_ERROR("Stub Shader Compilation Failed : " + error);
			return nullptr;
		}

		return vertexShaderByteCode;
	}

	DXGI_FORMAT RenderDevice::get_dxgi_format(TextureFormat format)
	{
		DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN;

		switch (format)
		{
		case TextureFormat::R32G32B32_FLOAT:
			fmt = DXGI_FORMAT_R32G32B32_FLOAT;
			break;

		case TextureFormat::R32G32B32A32_FLOAT:
			fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;

		case TextureFormat::R32G32B32_UINT:
			fmt = DXGI_FORMAT_R32G32B32_UINT;
			break;

		case TextureFormat::R32G32B32A32_UINT:
			fmt = DXGI_FORMAT_R32G32B32A32_UINT;
			break;

		case TextureFormat::R32G32B32_INT:
			fmt = DXGI_FORMAT_R32G32B32_SINT;
			break;

		case TextureFormat::R32G32B32A32_INT:
			fmt = DXGI_FORMAT_R32G32B32A32_SINT;
			break;

		case TextureFormat::R16G16B16A16_FLOAT:
			fmt = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;

		case TextureFormat::R16G16B16A16_UINT:
			fmt = DXGI_FORMAT_R16G16B16A16_UINT;
			break;

		case TextureFormat::R16G16B16A16_INT:
			fmt = DXGI_FORMAT_R16G16B16A16_SINT;
			break;

		case TextureFormat::R8G8B8A8_UNORM:
			fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;

		case TextureFormat::R8G8B8A8_UINT:
			fmt = DXGI_FORMAT_R8G8B8A8_UINT;
			break;

		case TextureFormat::D32_FLOAT_S8_UINT:
			fmt = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;

		case TextureFormat::D24_FLOAT_S8_UINT:
			fmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;

		case TextureFormat::D16_FLOAT:
			fmt = DXGI_FORMAT_D16_UNORM;
			break;

		default:
			fmt = DXGI_FORMAT_UNKNOWN;
			break;
		}

		return fmt;
	}
}

#endif
