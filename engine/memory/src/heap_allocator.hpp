#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class HeapAllocator : public IAllocator
{
public:
	HeapAllocator();
	~HeapAllocator();
	void* allocate(size_t size, size_t align) override;
	void deallocate(void* ptr) override;

private:
	static size_t size_with_header(size_t size, uint8_t alignment);
};

TE_END_TERMINUS_NAMESPACE
