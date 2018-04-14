#pragma once

#include <core/include/terminus_macros.hpp>
#include <stddef.h>
#include <stdint.h>

// Overloaded operators.

template <class T>
void* operator new(size_t bytes, T* allocator, int line, const char* file)
{
	return allocator->allocate(bytes, 1, 8);
}

template <class T>
void operator delete(void* ptr, T* allocator, int line, const char* file)
{
	allocator->free(ptr);
}

TE_BEGIN_TERMINUS_NAMESPACE

class Allocator
{
public:
	Allocator() : m_size(0), m_memory(nullptr), m_used_size(0), m_backing(nullptr), m_num_allocations(0) {}
	virtual ~Allocator() {}
	virtual void* allocate(size_t size, size_t count, size_t align) = 0;
	virtual void free(void* ptr) = 0;
	
protected:
	size_t		m_size;
	size_t		m_used_size;
	size_t		m_num_allocations;
	void*		m_memory;
	Allocator*  m_backing;
};

TE_END_TERMINUS_NAMESPACE
