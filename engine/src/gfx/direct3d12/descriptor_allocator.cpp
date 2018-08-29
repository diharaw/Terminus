#if defined(TE_GFX_BACKEND_D3D12)

#include <gfx/direct3d12/descriptor_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// -----------------------------------------------------------------------------------------------------------------------------------

DescriptorAllocator::DescriptorAllocator()
{
	m_heap = new DescriptorBlock(1024);
	m_free = m_heap;
}

// -----------------------------------------------------------------------------------------------------------------------------------

DescriptorAllocator::~DescriptorAllocator()
{
	DescriptorBlock* node = m_heap;
	while (node)
	{
		DescriptorBlock* temp = node;
		node = node->next_heap;
		delete temp;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool DescriptorAllocator::initialize(ID3D12Device* device, UINT descriptor_count, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAG flags)
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

DescriptorBlock* DescriptorAllocator::allocate_static(int size)
{
	DescriptorBlock* node = m_free;
	
	while (node->size < size)
	{
		if (!node->next_free)
			return nullptr;
		else
			node = node->next_free;
	}
	node = subdivide(size, node);
	node->allocated = true;
	return node;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void DescriptorAllocator::deallocate_static(DescriptorBlock* block)
{
	DescriptorBlock* node = block;
	
	if (node)
	{
		add_to_free_list(node);
		if (node->prev_heap && !node->prev_heap->allocated)
		{
			node = node->prev_heap;
			merge(node);
		}
		if (node->next_heap && !node->next_heap->allocated)
			merge(node);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void DescriptorAllocator::print_heap()
{
	DescriptorBlock* node = m_heap;
	while (node)
	{
		std::cout << " | " << node->size << ", " << (node->allocated ? "alloc" : "free") << " | ";
		node = node->next_heap;
	}
	std::cout << std::endl;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void DescriptorAllocator::add_to_free_list(DescriptorBlock* node)
{
	node->allocated = false;
	node->next_free = m_free;
	m_free->prev_free = node;
	m_free = node;
	m_free->prev_free = nullptr;
}

// -----------------------------------------------------------------------------------------------------------------------------------

DescriptorBlock* DescriptorAllocator::subdivide(int size, DescriptorBlock* block)
{
	DescriptorBlock* new_block = new DescriptorBlock(size);
	block->size -= size;
	if (block->next_heap)
	{
		block->next_heap->prev_heap = new_block;
		new_block->next_heap = block->next_heap;
	}
	new_block->prev_heap = block;
	block->next_heap = new_block;
	return new_block;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void DescriptorAllocator::merge(DescriptorBlock* block)
{
	DescriptorBlock* node_to_delete = block->next_heap;
	block->size += block->next_heap->size;
	if (node_to_delete->next_heap)
		node_to_delete->next_heap->prev_heap = block;
	if (node_to_delete->prev_free)
		node_to_delete->prev_free->next_free = node_to_delete->next_free;
	if (node_to_delete->next_free)
		node_to_delete->next_free->prev_free = node_to_delete->prev_free;
	DescriptorBlock* temp = node_to_delete->next_heap;
	if (node_to_delete == m_free)
		m_free = node_to_delete->next_free;
	delete block->next_heap;
	block->next_heap = temp;
	block->allocated = false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

DescriptorHeapAlloc DescriptorAllocator::allocate_dynamic(uint32_t count)
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