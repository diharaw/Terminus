#include <Graphics/render_device_d3d11.h>
#include <Core/context.h>
#include <global.h>
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

	void RenderDevice::Initialize()
	{
		//ThreadPool* thread_pool = Global::GetDefaultThreadPool();
		//// Create RenderDevice initializationg task
		//RenderDeviceInitData* data = Global::GetPerFrameAllocator()->NewPerFrame<RenderDeviceInitData>();
	
		//TaskData* task = thread_pool->CreateTask();
		//task->data = data;
		//task->function.Bind<RenderDevice, &RenderDevice::InitializeTask>(this);

		//thread_pool->SubmitAndWait();

		InitializeTask(nullptr);
	}

	void RenderDevice::InitializeTask(void* data)
	{
		m_window = context::get_platform().get_window();
		assert(InitializeAPI());
	}

	void RenderDevice::Shutdown()
	{
		if (m_swap_chain)
		{
			m_swap_chain->SetFullscreenState(false, NULL);
		}

		DestroyFramebuffer(m_default_framebuffer);
		DestroyTexture2D(m_default_render_target);
		DestroyTexture2D(m_default_depth_target);

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
			if (displayModeList[i].Width == (unsigned int)context::get_platform().get_width())
			{
				if (displayModeList[i].Width == (unsigned int)context::get_platform().get_height())
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

		swapChainDesc.BufferDesc.Width = context::get_platform().get_width();
		swapChainDesc.BufferDesc.Height = context::get_platform().get_height();

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
		swapChainDesc.OutputWindow = context::get_platform().get_handle_win32();
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

		depthBufferDesc.Width = context::get_platform().get_width();
		depthBufferDesc.Height = context::get_platform().get_height();
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

		return true;
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
											 bool createRenderTargetView,
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

		int rowPitch = (width * 4) * sizeof(unsigned char);
		m_device_context->UpdateSubresource(texture->m_textureD3D, 0, NULL, data, rowPitch, 0);

		// Create ShaderResourceView

		Result = m_device->CreateShaderResourceView(texture->m_textureD3D, &texture->m_srvDesc, &texture->m_textureView);
		if (FAILED(Result))
		{
			//T_LOG_ERROR("Failed to Create Texture2D!");
			return nullptr;
		}

		if (createRenderTargetView)
		{
			// Create DepthStencilView

			if (format == TextureFormat::D32_FLOAT_S8_UINT || format == TextureFormat::D24_FLOAT_S8_UINT || format == TextureFormat::D16_FLOAT)
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
		IndexBuffer* indexBuffer = new IndexBuffer();

		indexBuffer->m_type = BufferType::VERTEX;
		indexBuffer->m_usageType = usageType;

		switch (usageType)
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

		indexBuffer->m_BufferDescD3D.ByteWidth = size;
		indexBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_INDEX_BUFFER;

		indexBuffer->m_subresourceDataD3D.pSysMem = data;
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

	UniformBuffer* RenderDevice::CreateUniformBuffer(void* data,
													 uint size,
													 BufferUsageType usageType)
	{
		UniformBuffer* uniformBuffer = new UniformBuffer();

		uniformBuffer->m_BufferDescD3D.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		uniformBuffer->m_BufferDescD3D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		uniformBuffer->m_type = BufferType::VERTEX;
		uniformBuffer->m_usageType = usageType;

		switch (usageType)
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
		uniformBuffer->m_BufferDescD3D.ByteWidth = size;

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

	VertexArray* RenderDevice::CreateVertexArray(VertexBuffer* vertexBuffer,
												 IndexBuffer* indexBuffer,
												 InputLayoutType layoutType,
												 InputLayout* inputLayout)
	{
		VertexArray* vertexArray = new VertexArray();

		vertexArray->m_vertexBuffer = vertexBuffer;
		vertexArray->m_indexBuffer = indexBuffer;

		if (layoutType == InputLayoutType::STANDARD_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC inputElement[4];
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

			ID3D10Blob* bytecode = CreateStubShaderByteCodeFromLayout(layout);

			HRESULT result = m_device->CreateInputLayout(inputElement, 4, bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vertexArray->m_inputLayoutD3D);

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
		else if (layoutType == InputLayoutType::STANDARD_SKINNED_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC inputElement[6];
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

			ID3D10Blob* bytecode = CreateStubShaderByteCodeFromLayout(layout);

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
		else if (layoutType == InputLayoutType::CUSTOM_VERTEX)
		{
			D3D11_INPUT_ELEMENT_DESC* inputElement = new D3D11_INPUT_ELEMENT_DESC[inputLayout->m_Elements.size()];
			
			vertexArray->m_vertexBuffer->m_offset = 0;
			vertexArray->m_vertexBuffer->m_stride = inputLayout->m_vertexSize;

			for (int i = 0; i < inputLayout->m_Elements.size(); i++)
			{
				inputElement[i].SemanticName = inputLayout->m_Elements[i].m_semanticName.c_str();
				inputElement[i].SemanticIndex = 0;
				inputElement[i].InputSlot = 0;
				inputElement[i].AlignedByteOffset = inputLayout->m_Elements[i].m_offset;
				inputElement[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				inputElement[i].InstanceDataStepRate = 0;

				if (inputLayout->m_Elements[i].m_numSubElements == 3 && inputLayout->m_Elements[i].m_type == BufferDataType::FLOAT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (inputLayout->m_Elements[i].m_numSubElements == 4 && inputLayout->m_Elements[i].m_type == BufferDataType::FLOAT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				else if (inputLayout->m_Elements[i].m_numSubElements == 3 && inputLayout->m_Elements[i].m_type == BufferDataType::INT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_SINT;
				}
				else if (inputLayout->m_Elements[i].m_numSubElements == 4 && inputLayout->m_Elements[i].m_type == BufferDataType::INT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;
				}
				else if (inputLayout->m_Elements[i].m_numSubElements == 3 && inputLayout->m_Elements[i].m_type == BufferDataType::UINT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32_UINT;
				}
				else if (inputLayout->m_Elements[i].m_numSubElements == 4 && inputLayout->m_Elements[i].m_type == BufferDataType::UINT)
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
				}
				else
				{
					inputElement[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			}

			ID3D10Blob* bytecode = CreateStubShaderByteCodeFromLayout(*inputLayout);

			HRESULT result = m_device->CreateInputLayout(&inputElement[0], inputLayout->m_Elements.size(), bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vertexArray->m_inputLayoutD3D);

			delete[] inputElement;
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
		else
		{
			//T_LOG_ERROR("Vertex Array Creation : Unknown layout type");
			return nullptr;
		}
	}

	RasterizerState* RenderDevice::CreateRasterizerState(CullMode cullMode,
														 FillMode fillMode,
														 bool frontWindingCCW,
														 bool multisample,
														 bool scissor)
	{
		RasterizerState* rasterizerState = new RasterizerState();

		D3D11_RASTERIZER_DESC rasterizerDesc;

		rasterizerDesc.AntialiasedLineEnable = false;

		if (cullMode == CullMode::BACK)
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
		else if (cullMode == CullMode::FRONT)
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		else
			rasterizerDesc.CullMode = D3D11_CULL_NONE;

		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;

		if (fillMode == FillMode::SOLID)
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		else
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

		rasterizerDesc.FrontCounterClockwise = frontWindingCCW;
		rasterizerDesc.MultisampleEnable = multisample;
		rasterizerDesc.ScissorEnable = scissor;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		HRESULT result = m_device->CreateRasterizerState(&rasterizerDesc, &rasterizerState->m_RasterizerStateD3D);
		if (FAILED(result))
		{
			//T_LOG_ERROR("Failed to Create Rasterizer State");
			T_SAFE_DELETE(rasterizerState);
			return nullptr;
		}

		//T_LOG_INFO("Successfully Created Rasterizer State");

		return rasterizerState;
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
		SamplerState* samplerState = new SamplerState();
		
		switch (wrapModeU)
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
			samplerState->m_samplerDesc.BorderColor[0] = borderRed;
			samplerState->m_samplerDesc.BorderColor[1] = borderGreen;
			samplerState->m_samplerDesc.BorderColor[2] = borderBlue;
			samplerState->m_samplerDesc.BorderColor[3] = borderAlpha;
			break;
		}

		switch (wrapModeV)
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
			samplerState->m_samplerDesc.BorderColor[0] = borderRed;
			samplerState->m_samplerDesc.BorderColor[1] = borderGreen;
			samplerState->m_samplerDesc.BorderColor[2] = borderBlue;
			samplerState->m_samplerDesc.BorderColor[3] = borderAlpha;
			break;
		}

		switch (wrapModeW)
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
			samplerState->m_samplerDesc.BorderColor[0] = borderRed;
			samplerState->m_samplerDesc.BorderColor[1] = borderGreen;
			samplerState->m_samplerDesc.BorderColor[2] = borderBlue;
			samplerState->m_samplerDesc.BorderColor[3] = borderAlpha;
			break;
		}

		if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		else if (minFilter == TextureFilteringMode::LINEAR_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::LINEAR_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		else if (minFilter == TextureFilteringMode::NEAREST_ALL && magFilter == TextureFilteringMode::NEAREST_ALL)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		else if (minFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST && magFilter == TextureFilteringMode::LINEAR_MIPMAP_NEAREST)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		else if (minFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR && magFilter == TextureFilteringMode::NEAREST_MIPMAP_LINEAR)
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		else
			samplerState->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		if (maxAnisotropy < 1.0)
			samplerState->m_samplerDesc.MaxAnisotropy = 1.0;
		else
			samplerState->m_samplerDesc.MaxAnisotropy = maxAnisotropy;

		HRESULT result;
		result = m_device->CreateSamplerState(&samplerState->m_samplerDesc, &samplerState->m_D3D11SamplerState);

		if (FAILED(result))
		{
			T_SAFE_DELETE(samplerState);
			//T_LOG_ERROR("Failed to Create Sampler State!");
			return nullptr;
		}

		//T_LOG_INFO("Successfully Created Sampler State!");
		return samplerState;
	}

	Framebuffer* RenderDevice::CreateFramebuffer()
	{
		Framebuffer* framebuffer = new Framebuffer();
		return framebuffer;
	}

	Shader* RenderDevice::CreateShader(ShaderType type,
									   const char* shaderSource)
	{
		Shader* shader = new Shader();
		shader->m_type = type;
		shader->m_shaderSource = shaderSource;

		ID3D10Blob* shaderBytecode;
		ID3D10Blob* errorMessage;

		std::string entryPoint;
		std::string target;

		// TODO: handle different feature levels
		switch (type)
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

		size_t size = strlen(shaderSource);

		HRESULT result = D3DCompile(shaderSource, size, NULL, NULL, NULL, entryPoint.c_str(), target.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBytecode, &errorMessage);

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

		switch (type)
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
			//T_LOG_ERROR("Failed to create Shader");

			return nullptr;
		}

		return shader;
	}

	ShaderProgram* RenderDevice::CreateShaderProgram(Shader* vertexShader,
													 Shader* pixelShader,
													 Shader* geometryShader,
													 Shader* controlShader,
													 Shader* evaluationShader)
	{
		ShaderProgram* shaderProgram = new ShaderProgram();

		shaderProgram->m_shaderMap[vertexShader->m_type] = vertexShader;
		shaderProgram->m_shaderMap[pixelShader->m_type] = pixelShader;

		if (geometryShader)
			shaderProgram->m_shaderMap[geometryShader->m_type] = geometryShader;

		if (controlShader && evaluationShader)
		{
			shaderProgram->m_shaderMap[controlShader->m_type] = controlShader;
			shaderProgram->m_shaderMap[evaluationShader->m_type] = evaluationShader;
		}

		//T_LOG_INFO("Successfully Created Shader Program");

		return shaderProgram;
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
		DepthStencilState* depthStencilState = new DepthStencilState();

		if (enableDepthTest)
			depthStencilState->m_depthStencilStateDesc.DepthEnable = true;
		else
			depthStencilState->m_depthStencilStateDesc.DepthEnable = false;

		if (depthMask)
			depthStencilState->m_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			depthStencilState->m_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		switch (depthComparisonFunction)
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

		if (enableStencilTest)
			depthStencilState->m_depthStencilStateDesc.StencilEnable = true;
		else
			depthStencilState->m_depthStencilStateDesc.StencilEnable = false;


		depthStencilState->m_depthStencilStateDesc.StencilReadMask = stencilMask;
		depthStencilState->m_depthStencilStateDesc.StencilWriteMask = stencilMask;

		// Front Stencil Comparison Function

		switch (frontStencilComparisonFunction)
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

		switch (frontStencilFail)
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

		switch (frontStencilPassDepthPass)
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

		switch (frontStencilPassDepthFail)
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

		switch (backStencilComparisonFunction)
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

		switch (backStencilFail)
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

		switch (backStencilPassDepthPass)
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

		switch (backStencilPassDepthFail)
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
			//T_LOG_ERROR("Failed to Create Depth Stencil State");
			T_SAFE_DELETE(depthStencilState);
			return nullptr;
		}

		//T_LOG_INFO("Successfully Created Depth Stencil State");

		return depthStencilState;
	}

	void RenderDevice::AttachRenderTarget(Framebuffer* framebuffer, Texture* renderTarget)
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
			//T_LOG_ERROR("Render Target binding Failed : Invalid Texture Target");
			return;
			break;
		}
		}

		//_framebuffer->attachRenderTarget(_renderTarget);
		framebuffer->m_renderTargets.push_back(renderTarget);
		framebuffer->m_renderTargetViews.push_back(renderTarget->m_renderTargetView);
	}

	void RenderDevice::AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{
		ID3D11DepthStencilView* depthStencilView;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		// Check Format and set DS format accordingly
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		switch (renderTarget->m_type)
		{
			case TextureTarget::TEXTURE2D:
			{
				Texture2D* texture = (Texture2D*)renderTarget;
				m_device->CreateDepthStencilView(texture->m_textureD3D, NULL, &depthStencilView);
				break;
			}
			default:
			{
				//T_LOG_ERROR("Render Target binding Failed : Invalid Texture Target");
				return;
				break;
			}
		}

		framebuffer->m_depthStencilTarget = renderTarget;
		framebuffer->m_depthStecilView = renderTarget->m_depthView;
	}

	void RenderDevice::DestroyTexture1D(Texture1D* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		delete texture;
	}

	void RenderDevice::DestroyTexture2D(Texture2D* texture)
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

	void RenderDevice::DestroyTexture3D(Texture3D* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		delete texture;
	}

	void RenderDevice::DestroyTextureCube(TextureCube* texture)
	{
		if (texture->m_renderTargetView)
			texture->m_renderTargetView->Release();

		if (texture->m_textureView)
			texture->m_textureView->Release();

		if (texture->m_depthView)
			texture->m_depthView->Release();

		delete texture;
	}

	void RenderDevice::DestroyVertexBuffer(VertexBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::DestroyIndexBuffer(IndexBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::DestroyUniformBuffer(UniformBuffer* buffer)
	{
		if (buffer->m_BufferD3D)
			buffer->m_BufferD3D->Release();

		delete buffer;
	}

	void RenderDevice::DestroyVertexArray(VertexArray* vertexArray)
	{
		if (vertexArray)
		{
			if (vertexArray->m_vertexBuffer)
				DestroyVertexBuffer(vertexArray->m_vertexBuffer);

			if (vertexArray->m_indexBuffer)
				DestroyIndexBuffer(vertexArray->m_indexBuffer);
			
			T_SAFE_DELETE(vertexArray);
		}
	}

	void RenderDevice::DestroyRasterizerState(RasterizerState* state)
	{
		if (state->m_RasterizerStateD3D)
			state->m_RasterizerStateD3D->Release();

		delete state;
	}

	void RenderDevice::DestroySamplerState(SamplerState* state)
	{
		if (state->m_D3D11SamplerState)
			state->m_D3D11SamplerState->Release();

		delete state;
	}

	void RenderDevice::DestroyDepthStencilState(DepthStencilState* state)
	{
		if (state->m_depthStencilStateD3D)
			state->m_depthStencilStateD3D->Release();

		delete state;
	}

	void RenderDevice::DestroyFramebuffer(Framebuffer* framebuffer)
	{
		T_SAFE_DELETE(framebuffer);
	}

	void RenderDevice::DestroyShader(Shader* shader)
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

	void RenderDevice::DestoryShaderProgram(ShaderProgram* program)
	{
		for (auto it : program->m_shaderMap)
		{
			T_SAFE_DELETE(it.second);
		}
	}

	void RenderDevice::BindTexture(Texture* texture,
								   ShaderType shaderStage,
								   uint bufferSlot)
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

	void RenderDevice::BindUniformBuffer(UniformBuffer* uniformBuffer,
										 ShaderType shaderStage,
										 uint bufferSlot)
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

	void RenderDevice::BindRasterizerState(RasterizerState* state)
	{
		m_device_context->RSSetState(state->m_RasterizerStateD3D);
	}

	void RenderDevice::BindSamplerState(SamplerState* state,
										ShaderType shaderStage,
										uint slot)
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

	void RenderDevice::BindVertexArray(VertexArray* vertexArray)
	{
		m_device_context->IASetInputLayout(vertexArray->m_inputLayoutD3D);
		m_device_context->IASetVertexBuffers(0, 1, &vertexArray->m_vertexBuffer->m_BufferD3D,
												   &vertexArray->m_vertexBuffer->m_stride,
												   &vertexArray->m_vertexBuffer->m_offset);
		m_device_context->IASetIndexBuffer(vertexArray->m_indexBuffer->m_BufferD3D, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderDevice::BindFramebuffer(Framebuffer* framebuffer)
	{
		if (framebuffer)
			m_current_framebuffer = framebuffer;
		else
			m_current_framebuffer = m_default_framebuffer;
		
		m_device_context->OMSetRenderTargets(m_current_framebuffer->m_renderTargetViews.size(), &m_current_framebuffer->m_renderTargetViews[0], m_current_framebuffer->m_depthStecilView);
	}

	void RenderDevice::BindDepthStencilState(DepthStencilState* state)
	{
		m_device_context->OMSetDepthStencilState(state->m_depthStencilStateD3D, 1);
	}

	void RenderDevice::BindShaderProgram(ShaderProgram* program)
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

	void* RenderDevice::MapBuffer(Buffer* buffer, BufferMapType type)
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

	void RenderDevice::UnmapBuffer(Buffer* buffer)
	{
		m_device_context->Unmap(buffer->m_BufferD3D, 0);
	}

	void RenderDevice::SetPrimitiveType(DrawPrimitive primitive)
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

	void RenderDevice::ClearFramebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor)
	{
		switch (clearTarget)
		{
		case FramebufferClearTarget::DEPTH:
			m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			break;

		case FramebufferClearTarget::ALL:
			{
				for (auto it : m_current_framebuffer->m_renderTargetViews)
					m_device_context->ClearRenderTargetView(it, &clearColor.x);

				if (m_current_framebuffer->m_depthStecilView)
				{
					m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
					m_device_context->ClearDepthStencilView(m_current_framebuffer->m_depthStecilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
				}	
			}
			break;
		}
	}

	void RenderDevice::SetViewport(int width, int height, int topLeftX, int topLeftY)
	{
		D3D11_VIEWPORT viewport;

		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = topLeftX;
		viewport.TopLeftY = topLeftY;

		m_device_context->RSSetViewports(1, &viewport);
	}

	void RenderDevice::SwapBuffers()
	{
		m_swap_chain->Present(0, 0);
	}

	void RenderDevice::Draw(int firstIndex,
							int count)
	{
		m_device_context->Draw(count, firstIndex);
	}

	void RenderDevice::DrawIndexed(int indexCount)
	{
		m_device_context->DrawIndexed(indexCount, 0, 0);
	}

	void RenderDevice::DrawIndexedBaseVertex(int indexCount,
											 unsigned int baseIndex,
											 unsigned int baseVertex)
	{
		m_device_context->DrawIndexed(indexCount, baseIndex, baseVertex);
	}

	void RenderDevice::DrawInstanced()
	{


	}

	void RenderDevice::DrawIndexedInstanced()
	{

	}

	ID3D10Blob* RenderDevice::CreateStubShaderByteCodeFromLayout(InputLayout _layout)
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
}

#endif
