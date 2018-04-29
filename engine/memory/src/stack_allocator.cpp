#include <memory/src/stack_allocator.hpp>
#include <memory/src/memory_common.hpp>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

StackAllocator::StackAllocator()
{

}

StackAllocator::~StackAllocator()
{
#ifdef _DEBUG
    m_previous_position = nullptr;
#endif
    
    m_current_position = nullptr;
}

void StackAllocator::initialize_internal()
{
    assert(m_size > 0);
    
#ifdef _DEBUG
    m_previous_position = nullptr;
#endif
}

void StackAllocator::initialize(void* memory, size_t max_size)
{
	// Only init once.
	assert(m_memory == nullptr);
	assert(memory);
	assert(max_size > 0);

	m_size = max_size;
	m_memory = memory;
    
    initialize_internal();
}

void* StackAllocator::allocate(size_t size, size_t count, size_t align)
{
    assert(size != 0);
    uint8_t adjustment = memory_common::align_foward_adjustment_with_header(m_current_position, align, sizeof(AllocationHeader));
    
    if(m_used_size + adjustment + size > m_size)
        return nullptr;
    
    void* alignedAddress = memory_common::add(m_current_position, adjustment);
    AllocationHeader* header = (AllocationHeader*)(memory_common::subtract(alignedAddress, sizeof(AllocationHeader)));
    
    header->m_adjustment = adjustment;
    
#ifdef _DEBUG
    header->m_previous_address = m_previous_position;
    m_previous_position = alignedAddress;
#endif
    
    m_current_position = memory_common::add(alignedAddress, size);
    m_used_size += size + adjustment;
    m_num_allocations++;
    
    return alignedAddress;
}

void StackAllocator::free(void* ptr)
{
    AllocationHeader* header = (AllocationHeader*)(memory_common::subtract(ptr, sizeof(AllocationHeader)));
    m_used_size -= (uintptr_t)m_current_position - (uintptr_t)ptr + header->m_adjustment;
    
    m_current_position = memory_common::subtract(ptr, header->m_adjustment);
    
#ifdef _DEBUG
    m_previous_position = header->m_previous_address;
#endif
    
    m_num_allocations--;
}

TE_END_TERMINUS_NAMESPACE
