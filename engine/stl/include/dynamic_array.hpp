#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <memory/src/heap_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T>
class DynamicArray
{
public:
    DynamicArray() : m_num_elements(0), m_elements(nullptr) 
	{
		m_allocator = &m_default_allocator;
	}

    DynamicArray(const size_t& size, IAllocator* allocator)
    {
		if (allocator)
			m_allocator = allocator;
		else
			m_allocator = &m_default_allocator;

        resize(size);
    }
    ~DynamicArray()
    {
        clear();
    }
    
    void resize(const size_t& size)
    {
        if (m_elements)
			m_allocator->free(m_elements);
        
        m_num_elements = size;
        m_elements = (T*)m_allocator->allocate(m_num_elements * sizeof(T), 1, 8);
    }
    
    void clear()
    {
        if (m_elements)
			m_allocator->free(m_elements);
        
        m_num_elements = 0;
        m_elements = (T*)m_allocator->allocate(m_num_elements * sizeof(T), 1, 8);
    }
    
    T& operator [](uint32_t idx)
    {
        return m_elements[idx];
    }
    
    T operator [](uint32_t idx) const
    {
        return m_elements[idx];
    }
    
private:
    size_t		  m_num_elements;
    T*			  m_elements;
	IAllocator*	  m_allocator;
	HeapAllocator m_default_allocator;
};

TE_END_TERMINUS_NAMESPACE
