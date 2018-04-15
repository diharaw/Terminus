#include <memory/src/heap_allocator.hpp>
#include <memory/src/memory_common.hpp>
#include <memory/src/dlmalloc.h>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

HeapAllocator::HeapAllocator()
{

}

HeapAllocator::~HeapAllocator()
{

}

void HeapAllocator::initialize(IAllocator* backing, size_t max_size)
{
	// Prevent initialize from being called.
	assert(false);
}

void HeapAllocator::initialize(void* memory, size_t max_size)
{
	// Prevent initialize from being called.
	assert(false);
}

void* HeapAllocator::allocate(size_t size, size_t count, size_t align)
{
	return dlmalloc(size);
}

void HeapAllocator::free(void* ptr)
{
    return dlfree(ptr);
}

TE_END_TERMINUS_NAMESPACE
