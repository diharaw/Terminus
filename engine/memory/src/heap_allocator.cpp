#include <memory/src/heap_allocator.hpp>
#include <memory/src/memory_common.hpp>
#include <assert.h>
#include <stdlib.h>

TE_BEGIN_TERMINUS_NAMESPACE

const static uint32_t HEADER_PAD_VALUE = 0xffffffffu;

struct AllocHeader
{
	size_t size;
};

AllocHeader* header_pointer(void* data)
{
	uint32_t* p = (uint32_t*)data;
	while (p[-1] == HEADER_PAD_VALUE)
		--p;

	return ((AllocHeader*)p) - 1;
}

void* data_pointer(AllocHeader* header, const size_t& alignment)
{
	void* p = header + 1;
	return memory::align_forward(p, alignment);
}

void initialize_header(AllocHeader* header, void* data, const size_t& size)
{
	header->size = size;
	uint32_t* p = (uint32_t*)(header + 1);
	while (p < data)
		*p++ = HEADER_PAD_VALUE;
}

HeapAllocator::HeapAllocator() : m_num_alloc(0), m_alloc_size(0)
{

}

HeapAllocator::~HeapAllocator()
{
	m_num_alloc = 0;
	m_alloc_size = 0;
}

void* HeapAllocator::allocate(size_t size, size_t align)
{
	return malloc(size);
}

void HeapAllocator::deallocate(void* ptr)
{
    return ::free(ptr);
}

size_t HeapAllocator::size_with_header(size_t size, uint8_t alignment)
{
	return size + sizeof(AllocHeader) + alignment;
}

TE_END_TERMINUS_NAMESPACE
