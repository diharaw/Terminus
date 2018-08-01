#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/direct3d12/descriptor_heap_manager.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// -----------------------------------------------------------------------------------------------------------------------------------

bool DescriptorHeapManager::initialize(ID3D12Device* device, UINT descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAG flags)
{
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
	heap_desc.NumDescriptors = descriptor_count;
	heap_desc.Type = type;
	heap_desc.Flags = flags;
	ThrowIfFailed(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_heap)));
    m_descriptor_size = device->GetDescriptorHandleIncrementSize(type);
    m_cpu_root_handle = m_heap->GetCPUDescriptorHandleForHeapStart();
    m_gpu_root_handle = m_heap->GetGPUDescriptorHandleForHeapStart();
    m_cpu_current_handle = m_cpu_root_handle;
    m_gpu_current_handle = m_gpu_root_handle;
    m_total = descriptor_count;
    m_allocated = 0;
}

// -----------------------------------------------------------------------------------------------------------------------------------

DescriptorHeapAlloc DescriptorHeapManager::allocate_dynamic(uint32_t count)
{
    m_allocated += count;
    if (m_allocated > m_total)
    {
        // Reset current handle to the beginning of heap.
        m_allocated = 0;
        m_cpu_current_handle = m_cpu_root_handle;
        m_gpu_current_handle = m_gpu_root_handle;
    }
    DescriptorHeapAlloc alloc;
    alloc.alloc_idx = -1;
    alloc.cpu_handle = m_cpu_current_handle;
    alloc.gpu_handle = m_gpu_current_handle;
    m_cpu_current_handle.Offset(count, m_descriptor_size);
    m_gpu_current_handle.Offset(count, m_descriptor_size);
    m_allocated += count;
    return alloc;
}

// -----------------------------------------------------------------------------------------------------------------------------------

TE_END_TERMINUS_NAMESPACE

#endif