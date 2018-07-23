#pragma once

#if defined(TE_GFX_BACKEND_D3D12)

#include <dxgi1_6.h>
#include <d3d12.h>
#include <core/terminus_macros.hpp>
#include <gfx/direct3d12/d3dx12.h>
#include <gfx/gfx_enums.hpp>
#include <stl/string_buffer.hpp>
#include <stl/vector.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

#define MAX_COLOR_ATTACHMENTS 8
#define MAX_VERTEX_ATTRIBUTES 8

struct Buffer
{
	ComPtr<ID3D12Resource>   d3d12_buffer;
	D3D12_VERTEX_BUFFER_VIEW d3d12_vertex_buffer_view;
};

struct Texture
{
	ComPtr<ID3D12Resource>        d3d12_texture;
	CD3DX12_CPU_DESCRIPTOR_HANDLE d3d12_srv;
};

struct Framebuffer
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE d3d12_rtv[MAX_COLOR_ATTACHMENTS];
	CD3DX12_CPU_DESCRIPTOR_HANDLE d3d12_dsv;
};

struct CommandPool
{
	ComPtr<ID3D12CommandAllocator> d3d12_cmd_alloc;
};

struct CommandBuffer
{
	ComPtr<ID3D12GraphicsCommandList> d3d12_cmd_list;
};

struct Shader
{
	ShaderStage	     stage;
	ComPtr<ID3DBlob> d3d12_blob;
	StringBuffer32   entry_point;
};

struct PipelineState
{
	PipelineType				type;
	ComPtr<ID3D12PipelineState> d3d12_pipeline_state;
};

struct Fence
{
	ComPtr<ID3D12Fence> d3d12_fence;
};

// TODO: Namespace the GFX stuff!!!
struct SemaphoreGPU
{
	ComPtr<ID3D12Fence> d3d12_fence;
};

struct InputLayout
{
	D3D12_INPUT_ELEMENT_DESC d3d12_input_element_descs[MAX_VERTEX_ATTRIBUTES];
};

struct VertexArray
{
	Buffer*      vertex_buffer;
	Buffer*      index_buffer;
	InputLayout* input_layout;
};

struct PipelineLayout
{
	ComPtr<ID3D12RootSignature> d3d12_root_sig;
};

struct DescriptorSet
{
	// Indicates whether this Descriptor Table only contains CBV's, 
	// in which case it can be bound as a root constant buffer view.
	bool                          only_cbv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE d3d12_gpu_address;
};

TE_END_TERMINUS_NAMESPACE

#endif