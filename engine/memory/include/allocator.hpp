#pragma once

#include <memory/include/memory_macros.hpp>
#include <core/include/terminus_macros.hpp>
#include <stddef.h>
#include <stdint.h>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

// Overloaded operators.

template <typename OBJECT_TYPE, typename ALLOCATOR_TYPE, typename ...ARGS>
OBJECT_TYPE* custom_new(ALLOCATOR_TYPE* allocator, ARGS&&... params)
{
	void* p = allocator->allocate(sizeof(OBJECT_TYPE), 8);
	return new(p) OBJECT_TYPE(std::forward<ARGS>(params)...);
}

template <typename OBJECT_TYPE, typename ALLOCATOR_TYPE>
void custom_delete(ALLOCATOR_TYPE* allocator, OBJECT_TYPE* p)
{
	p->~OBJECT_TYPE();
	allocator->deallocate(p);
}

template <typename OBJECT_TYPE, typename ...ARGS>
OBJECT_TYPE* heap_new(ARGS&&... params)
{
	void* p = malloc(sizeof(OBJECT_TYPE));
	return new (p) (std::forward<Args>(params)...);
}

template <typename OBJECT_TYPE>
void heap_delete(OBJECT_TYPE* p)
{
	p->~OBJECT_TYPE();
	free(p);
}

inline void* heap_alloc(const size_t& size)
{
	return malloc(size);
}

inline void heap_free(void* p)
{
	free(p);
}

class IAllocator
{
public:
	IAllocator() : m_size(0), m_memory(nullptr), m_used_size(0), m_num_allocations(0) {}
	virtual ~IAllocator() {}
	virtual void* allocate(size_t size, size_t align) = 0;
	virtual void deallocate(void* ptr) = 0;

	inline size_t num_allocations() { return m_num_allocations; }
	inline size_t allocated_size() { return m_used_size; }
	
protected:
	size_t		 m_size;
	size_t		 m_used_size;
	size_t		 m_num_allocations;
	void*		 m_memory;
};

TE_END_TERMINUS_NAMESPACE
