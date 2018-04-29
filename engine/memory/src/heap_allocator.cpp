#include <memory/src/heap_allocator.hpp>
#include <memory/src/memory_common.hpp>
#include <assert.h>
#include <stdlib.h>

TE_BEGIN_TERMINUS_NAMESPACE

HeapAllocator::HeapAllocator()
{

}

HeapAllocator::~HeapAllocator()
{

}

void* HeapAllocator::allocate(size_t size, size_t count, size_t align)
{
	return malloc(size);
}

void HeapAllocator::free(void* ptr)
{
    return ::free(ptr);
}

TE_END_TERMINUS_NAMESPACE
