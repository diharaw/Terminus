#pragma once

#include <memory/memory_macros.hpp>
#include <core/terminus_macros.hpp>
#include <io/logger.hpp>
#include <stddef.h>
#include <stdint.h>
#include <iostream>

// Overloaded operators.
// https://blog.molecular-matters.com/2011/07/07/memory-system-part-2/

template <typename ALLOCATOR>
void* operator new(size_t size, ALLOCATOR* a, int line, const char* file)
{
#if defined(TE_TRACK_ALLOCATIONS)
	TE_LOG_INFO("Performing allocation of size " + size + " at line " + line + " of " + file);
#endif
	return a->allocate(size, 8);
}

template <typename OBJECT, typename ALLOCATOR>
OBJECT* custom_new_array(size_t size, ALLOCATOR* a, int line, const char* file)
{
#if defined(TE_TRACK_ALLOCATIONS)
	TE_LOG_INFO("Performing array allocation of size " + size + " at line " + line + " of " + file);
#endif
	union
	{
		void*	as_void;
		size_t* as_size_t;
		OBJECT* as_T;
	};

	as_void = a->allocate(sizeof(OBJECT) * size + sizeof(size_t));

	// store number of instances in first size_t bytes
	*as_size_t++ = size;

	// construct instances using placement new
	const OBJECT* const one_past_last = as_T + size;
	while (as_T < one_past_last)
		new (as_T++) OBJECT;

	// hand user the pointer to the first instance
	return (as_T - size);
}

template <typename OBJECT, typename ALLOCATOR>
void custom_delete(OBJECT* p, ALLOCATOR* a, int line, const char* file)
{
#if defined(TE_TRACK_ALLOCATIONS)
	TE_LOG_INFO("Performing deallocation of size " + sizeof(OBJECT) + " at line " + line + " of " + file);
#endif
	p->~OBJECT();
	a->deallocate(p);
}

template <typename OBJECT, typename ALLOCATOR>
void custom_delete_array(OBJECT* p, ALLOCATOR* a)
{
	union
	{
		size_t* as_size_t;
		OBJECT* as_T;
	};

	// user pointer points to first instance...
	as_T = ptr;

	// ...so go back size_t bytes and grab number of instances
	const size_t N = as_size_t[-1];

	// call instances' destructor in reverse order
	for (size_t i = N; i>0; --i)
		as_T[i - 1].~OBJECT();

	a->deallocate(as_size_t - 1);
}

#define TE_NEW(ALLOCATOR) new(ALLOCATOR, __LINE__, __FILE__)
#define TE_DELETE(OBJECT, ALLOCATOR) custom_delete(OBJECT, ALLOCATOR, __LINE__, __FILE__)

#define TE_HEAP_NEW TE_NEW(&te::global::default_allocator())
#define TE_HEAP_DELETE(OBJECT) TE_DELETE(OBJECT, &te::global::default_allocator())

#define TE_NEW_ARRAY(CLASS, N, ALLOCATOR) custom_new_array<CLASS>(N, ALLOCATOR, __LINE__, __FILE__)
#define TE_DELETE_ARRAY(OBJECT, ALLOCATOR) custom_delete_array(OBJECT, ALLOCATOR, __LINE__, __FILE__)

#define TE_HEAP_NEW_ARRAY(CLASS, N) TE_NEW_ARRAY(CLASS, N, &te::global::default_allocator())
#define TE_HEAP_DELETE_ARRAY(OBJECT) TE_DELETE_ARRAY(OBJECT, &te::global::default_allocator())

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
