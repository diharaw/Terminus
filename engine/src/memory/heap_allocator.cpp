#include <memory/heap_allocator.hpp>
#include <memory/memory_common.hpp>
#include <concurrency/atomic.hpp>
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

HeapAllocator::HeapAllocator() : IAllocator()
{

}

HeapAllocator::~HeapAllocator()
{

}

void* HeapAllocator::allocate(size_t size, size_t align)
{
	size_t total_size = size_with_header(size, align);
	AllocHeader* h = (AllocHeader*)malloc(total_size);
	void* p = data_pointer(h, align);
	initialize_header(h, p, total_size);

	atomic::add64((int64_t*)&m_used_size, total_size);
	atomic::increment64((int64_t*)&m_num_allocations);

	return p;
}

void HeapAllocator::deallocate(void* ptr)
{
	if (!ptr)
		return;

	AllocHeader* h = header_pointer(ptr);

	atomic::sub64((int64_t*)&m_used_size, h->size);
	atomic::decrement64((int64_t*)&m_num_allocations);

	free(h);
}

size_t HeapAllocator::size_with_header(size_t size, uint8_t alignment)
{
	return size + sizeof(AllocHeader) + alignment;
}

TE_END_TERMINUS_NAMESPACE
