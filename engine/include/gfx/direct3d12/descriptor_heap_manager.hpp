#pragma once

#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/gfx_types.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct DescriptorHeapAlloc
{
    size_t                        alloc_idx;
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle;
};

class DescriptorHeapManager
{
public:
    bool initialize(ID3D12Device* device, UINT descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAG flags);
    DescriptorHeapAlloc allocate_dynamic(uint32_t count);

private:
    UINT                          m_descriptor_size;
    UINT                          m_total;
    UINT                          m_allocated;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpu_root_handle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpu_current_handle;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpu_root_handle;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpu_current_handle;
    ComPtr<ID3D12DescriptorHeap>  m_heap;
};

TE_END_TERMINUS_NAMESPACE

#endif