#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <memory/src/heap_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T>
class ResizableArray
{
public:
    ResizableArray(size_t capacity = 10, IAllocator* allocator = nullptr) : m_num_elements(0), m_capacity(0), m_data(nullptr)
    {
		if (allocator)
			m_allocator = allocator;
		else
			m_allocator = &m_default_allocator;

        reserve(capacity);
    }
    
    ~ResizableArray()
    {
		m_allocator->free(m_data);
    }

    void clear()
    {
        m_num_elements = 0;
    }
    
    // Reserves space for 'size' number of elements.
    void reserve(size_t size)
    {
        std::cout << "Reserving..." << std::endl;
        
        size_t old_capacity = m_capacity;
        m_capacity = size;
        T* new_data = (T*)m_allocator->allocate(m_capacity * sizeof(T), 1, 8);
        
        if (m_data)
        {
            T* old_data = m_data;
            memcpy(new_data, old_data, sizeof(T) * old_capacity);
			m_allocator->free(old_data);
        }
    
        m_data = new_data;
    }
    
    // Sets the capacity of the array to 'size'. The actual number of elements is not equal to 'size'.
    void resize(const size_t& size)
    {
        reserve(size);
        m_num_elements = size;
    }
    
    void push_back(T obj)
    {
        if (m_num_elements > ((m_capacity/100.0f) * 70.0f))
            reserve(m_capacity * 2);
        
        m_data[m_num_elements++] = obj;
    }
    
    size_t capacity()
    {
        return m_capacity;
    }
    
    size_t num_elements()
    {
        return m_num_elements;
    }
    
    T& operator [](uint32_t idx)
    {
        return m_data[idx];
    }
    
    T operator [](uint32_t idx) const
    {
        return m_data[idx];
    }
    
private:
    size_t		  m_num_elements;
    size_t		  m_capacity;
    T*			  m_data;
	IAllocator*	  m_allocator;
	HeapAllocator m_default_allocator;
};

TE_END_TERMINUS_NAMESPACE
