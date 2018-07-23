#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/direct3d12/gfx_device_d3d12.hpp>
#include <core/engine_core.hpp>
#include <io/logger.hpp>

#define RTV_DESCRIPTOR_COUNT 1000
#define DSV_DESCRIPTOR_COUNT 1000
#define CBV_SRV_UAV_DESCRIPTOR_COUNT 1000000
#define SAMPLER_DESCRIPTOR_COUNT 2048

TE_BEGIN_TERMINUS_NAMESPACE

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::GfxDevice()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

GfxDevice::~GfxDevice()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

bool GfxDevice::initialize()
{
#if defined(_DEBUG)
	enable_debug_layer();
#endif
	create_device();
	create_command_queues();
	create_swap_chain();
	create_descriptor_heaps();
	query_descriptor_sizes();
	create_framebuffers();

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::shutdown()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::enable_debug_layer()
{
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debug_controller;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
		{
			debug_controller->EnableDebugLayer();
			// Enable additional debug layers.
			m_dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::get_hardware_adapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		ThrowIfFailed(adapter->GetDesc1(&desc));

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::create_device()
{
	ThrowIfFailed(CreateDXGIFactory2(m_dxgi_factory_flags, IID_PPV_ARGS(&m_dxgi_factory)));

	ComPtr<IDXGIAdapter1> hardware_adapter;
	get_hardware_adapter(m_dxgi_factory.Get(), &hardware_adapter);

	ThrowIfFailed(D3D12CreateDevice(hardware_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::create_command_queues()
{
	// Create graphics command queue
	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_graphics_cmd_queue)));
	NAME_D3D12_OBJECT(m_graphics_cmd_queue);

	// Create compute command queue
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;

	ThrowIfFailed(m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_compute_cmd_queue)));
	NAME_D3D12_OBJECT(m_compute_cmd_queue);

	// Create copy command queue
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

	ThrowIfFailed(m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_copy_cmd_queue)));
	NAME_D3D12_OBJECT(m_copy_cmd_queue);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::create_swap_chain()
{
	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	sswap_chain_desc.BufferCount = FRAME_COUNT;
	sswap_chain_desc.Width = m_width;
	sswap_chain_desc.Height = m_height;
	sswap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sswap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sswap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sswap_chain_desc.SampleDesc.Count = 1;

	HWND w32_hwnd = (HWND)global::application()->native_handle();

	ComPtr<IDXGISwapChain1> swap_chain;

	// Swap chain needs the queue so that it can force a flush on it.
	ThrowIfFailed(m_factory->CreateSwapChainForHwnd(m_graphics_cmd_queue.Get(),
													w32_hwnd,
													&swap_chain_desc,
													nullptr,
													nullptr,
													&swap_chain));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(m_factory->MakeWindowAssociation(w32_hwnd, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swap_chain.As(&m_swap_chain));
	m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::create_descriptor_heaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
	heap_desc.NumDescriptors = RTV_DESCRIPTOR_COUNT;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_rtv_heap)));

	heap_desc.NumDescriptors = DSV_DESCRIPTOR_COUNT;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_dsv_heap)));

	heap_desc.NumDescriptors = CBV_SRV_UAV_DESCRIPTOR_COUNT * 10;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_cbv_srv_uav_heap_cpu)));

	// TODO: Use larger CBV/SRV/UAV descriptor cout for Tier 3 devices.
	heap_desc.NumDescriptors = CBV_SRV_UAV_DESCRIPTOR_COUNT;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_cbv_srv_uav_heap_gpu)));

	heap_desc.NumDescriptors = SAMPLER_DESCRIPTOR_COUNT * 10;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_sampler_heap_cpu)));

	heap_desc.NumDescriptors = SAMPLER_DESCRIPTOR_COUNT;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_sampler_heap_gpu)));
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::query_descriptor_sizes()
{
	m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbv_srv_uav_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_sampler_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void GfxDevice::create_framebuffers()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each frame.
	for (UINT n = 0; n < FRAME_COUNT; n++)
	{
		m_back_buffer_rt[n] = TE_HEAP_NEW Texture();
		m_back_buffer_fbo[n] = TE_HEAP_NEW Framebuffer();

		ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_back_buffer_rt[n]->d3d12_texture)));
		m_device->CreateRenderTargetView(m_back_buffer_rt[n]->d3d12_texture.Get(), nullptr, rtv_handle);
		rtv_handle.Offset(1, m_rtv_descriptor_size);

		m_back_buffer_fbo[n]->d3d12_rtv.resize(1);
		m_back_buffer_fbo[n]->d3d12_rtv[0] = rtv_handle;

		// TODO: Create depth stencil views
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

TE_END_TERMINUS_NAMESPACE

#endif