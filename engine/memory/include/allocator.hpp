#pragma once

#include <memory/include/memory_macros.hpp>
#include <core/include/terminus_macros.hpp>
#include <stddef.h>
#include <stdint.h>

// Overloaded operators.

template <typename ALLOCATOR>
void* operator new(size_t bytes, ALLOCATOR* allocator, int line, const char* file)
{
	return allocator->allocate(bytes, 1, 8);
}

template <typename OBJECT, typename ALLOCATOR>
void custom_delete(OBJECT* ptr, ALLOCATOR* allocator, int line, const char* file)
{
	ptr->~OBJECT();
	allocator->free(ptr);
}

TE_BEGIN_TERMINUS_NAMESPACE

class IAllocator
{
public:
	IAllocator() : m_size(0), m_memory(nullptr), m_used_size(0), m_num_allocations(0) {}
	virtual ~IAllocator() {}
	virtual void* allocate(size_t size, size_t count, size_t align) = 0;
	virtual void free(void* ptr) = 0;
	
protected:
	size_t		 m_size;
	size_t		 m_used_size;
	size_t		 m_num_allocations;
	void*		 m_memory;
};

TE_END_TERMINUS_NAMESPACE
