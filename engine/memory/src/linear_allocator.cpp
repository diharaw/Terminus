#include <memory/src/linear_allocator.hpp>
#include <memory/src/memory_common.hpp>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

LinearAllocator::LinearAllocator()
{

}

LinearAllocator::~LinearAllocator()
{

}

void LinearAllocator::initialize(void* memory, size_t max_size)
{
	// Only init once.
	assert(m_memory == nullptr);
	assert(memory);
	assert(max_size > 0);

	m_size	   = max_size;
	m_memory   = memory;
	m_position = m_memory;
}

void* LinearAllocator::allocate(size_t size, size_t align)
{
	assert(size != 0);

	uint8_t adjustment = memory::align_backward_adjustment(m_position, align);

	if (m_used_size + adjustment + size > m_size)
		return nullptr;

	uintptr_t aligned_address = (uintptr_t)m_position + adjustment;
	m_position = (void*)(aligned_address + size);
	m_num_allocations++;
	m_used_size += adjustment + size;

	return (void*)aligned_address;
}

void LinearAllocator::deallocate(void* ptr)
{
	assert(false);
}

void LinearAllocator::clear()
{
	m_num_allocations = 0;
	m_used_size		  = 0;
	m_position		  = m_memory;
}

TE_END_TERMINUS_NAMESPACE
