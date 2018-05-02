#pragma once

#include <memory/include/memory_macros.hpp>
#include <core/include/terminus_macros.hpp>
#include <stddef.h>
#include <stdint.h>

TE_BEGIN_TERMINUS_NAMESPACE

// Overloaded operators.

template <typename ALLOCATOR_TYPE, typename OBJECT_TYPE, typename ...ARGS>
OBJECT_TYPE* custom_new(ALLOCATOR_TYPE* allocator, ARGS&&... params, int line, const char* file)
{
	void* p = allocator->allocate(sizeof(OBJECT_TYPE), 8);
	return new(p) (std::forward<Args>(params)...);
}

template <typename ALLOCATOR_TYPE, typename OBJECT_TYPE>
void custom_delete(ALLOCATOR_TYPE* allocator, OBJECT_TYPE* p, int line, const char* file)
{
	p->~OBJECT();
	allocator->free(p);
}

class IAllocator
{
public:
	IAllocator() : m_size(0), m_memory(nullptr), m_used_size(0), m_num_allocations(0) {}
	virtual ~IAllocator() {}
	virtual void* allocate(size_t size, size_t align) = 0;
	virtual void deallocate(void* ptr) = 0;
	
protected:
	size_t		 m_size;
	size_t		 m_used_size;
	size_t		 m_num_allocations;
	void*		 m_memory;
};

TE_END_TERMINUS_NAMESPACE
