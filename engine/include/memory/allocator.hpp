#pragma once

#include <memory/memory_macros.hpp>
#include <core/terminus_macros.hpp>
#include <stddef.h>
#include <stdint.h>
#include <iostream>

// Overloaded operators.

template <typename ALLOCATOR>
void* operator new(size_t size, ALLOCATOR* a, int line, const char* file)
{
	std::cout << "Performing allocation of size " << size << " at line " << line << " of " << file << std::endl;
	return a->allocate(size, 8);
}

template <typename ALLOCATOR, typename OBJECT>
void custom_delete(OBJECT* p, ALLOCATOR* a, int line, const char* file)
{
	std::cout << "Performing deallocation of size " << sizeof(OBJECT) << " at line " << line << " of " << file << std::endl;
	p->~OBJECT();
	a->deallocate(p);
}

#define TE_NEW(ALLOCATOR) new(ALLOCATOR, __LINE__, __FILE__)
#define TE_DELETE(OBJECT, ALLOCATOR) custom_delete(OBJECT, ALLOCATOR, __LINE__, __FILE__)
#define TE_HEAP_NEW new(&te::global::default_allocator(), __LINE__, __FILE__)
#define TE_HEAP_DELETE(OBJECT) custom_delete(OBJECT, &te::global::default_allocator(), __LINE__, __FILE__)
#define TE_HEAP_ALLOC(SIZE) te::global::default_allocator().allocate(SIZE, 8)
#define TE_HEAP_DEALLOC(OBJECT) te::global::default_allocator().deallocate(OBJECT)

TE_BEGIN_TERMINUS_NAMESPACE

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
	size_t m_size;
	size_t m_used_size;
	size_t m_num_allocations;
	void*  m_memory;
};

TE_END_TERMINUS_NAMESPACE
