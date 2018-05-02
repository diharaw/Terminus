#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class HeapAllocator : public IAllocator
{
private:
	uint32_t m_num_alloc;
	size_t   m_alloc_size;

public:
	HeapAllocator();
	~HeapAllocator();
	void* allocate(size_t size, size_t align) override;
	void deallocate(void* ptr) override;

private:
	inline size_t num_allocations() { return m_num_alloc;  }
	inline size_t allocated_size()  { return m_alloc_size; }
	static size_t size_with_header(size_t size, uint8_t alignment);
};

TE_END_TERMINUS_NAMESPACE
