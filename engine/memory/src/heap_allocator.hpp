#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class HeapAllocator : public IAllocator
{
public:
	HeapAllocator();
	~HeapAllocator();
	void* allocate(size_t size, size_t count, size_t align) override;
	void free(void* ptr) override;
};

TE_END_TERMINUS_NAMESPACE
