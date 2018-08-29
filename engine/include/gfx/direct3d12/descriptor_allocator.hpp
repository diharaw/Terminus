#pragma once

#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/gfx_types.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct DescriptorBlock
{
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle;

    int size;
	bool allocated;

	// Heap List
	DescriptorBlock* prev_heap;
	DescriptorBlock* next_heap;

	// Free List
	DescriptorBlock* prev_free;
	DescriptorBlock* next_free;

	DescriptorBlock(int _size) : size(_size), prev_heap(nullptr), next_heap(nullptr), prev_free(nullptr), next_free(nullptr), allocated(false) {}
};

class DescriptorAllocater
{
public:
    bool initialize(ID3D12Device* device, UINT descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAG flags);
    DescriptorBlock* allocate_static(uint32_t size);
    void deallocate_static(DescriptorBlock* block);

    DescriptorBlock* allocate_dynamic(uint32_t count);
    void deallocate_dynamic(DescriptorBlock* block);

private:
    void add_to_free_list(DescriptorBlock* node);
    DescriptorBlock* subdivide(int size, DescriptorBlock* block);
    void merge(DescriptorBlock* block);

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