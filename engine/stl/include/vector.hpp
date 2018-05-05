#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/engine_core.hpp>
#include <io/include/io_macros.hpp>
#include <memory/src/heap_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T>
class Vector
{
public:
	Vector(const size_t& capacity = 10) : m_size(0), m_capacity(0), m_elements(nullptr)
    {
        reserve(capacity);
    }
    
    ~Vector()
    {
		TE_HEAP_DEALLOC(m_elements);
    }

    void clear()
    {
		m_size = 0;
    }
    
    // Reserves space for 'size' number of elements.
    void reserve(const size_t& size)
    {
        std::cout << "Reserving..." << std::endl;
        
        size_t old_capacity = m_capacity;
        m_capacity = size;
		T* new_data = (T*)TE_HEAP_ALLOC(m_capacity * sizeof(T));
        
        if (m_elements)
        {
            T* old_data = m_elements;

			for (int i = 0; i < m_size; i++)
				new_data[i] = old_data[i];

			TE_HEAP_DEALLOC(old_data);
        }
    
		m_elements = new_data;
    }
    
    // Sets the capacity of the array to 'size'. The actual number of elements is not equal to 'size'.
    void resize(const size_t& size)
    {
        reserve(size);
		m_size = size;
    }
    
    void push_back(T obj)
    {
        if (m_size > ((m_capacity/100.0f) * 70.0f))
            reserve(m_capacity * 2);
        
		m_elements[m_size++] = obj;
    }
    
    size_t capacity()
    {
        return m_capacity;
    }
    
    size_t size()
    {
        return m_size;
    }
    
    T& operator [](const uint32_t& idx)
    {
        return m_elements[idx];
    }
    
    T operator [](const uint32_t& idx) const
    {
        return m_elements[idx];
    }

public:
	// Iterators
	class Iterator
	{
		T*	   data;
		size_t	   position;
		size_t num_elements;
	public:
		Iterator(T* _data, size_t _position, size_t _num_elements) : data(_data), position(_position), num_elements(_num_elements)
		{
		}

		T& operator*()
		{
			return data[position];
		}

		Iterator& operator++()
		{
			position++;
			return *this;
		}

		bool operator!=(const Iterator& it) const
		{
			return position != it.position;
		}
	};

	class ConstIterator
	{
		T*		   data;
		size_t	   position;
		size_t	   num_elements;
	public:
		ConstIterator(T* _data, size_t _position, size_t _num_elements) : data(_data), position(_position), num_elements(_num_elements)
		{
		}

		const T& operator*() const
		{
			return data[position];
		}

		ConstIterator& operator++()
		{
			position++; return *this;
		}

		bool operator!=(const ConstIterator& it) const
		{
			return position != it.position;
		}
	};

	Iterator begin()
	{
		return { m_elements, 0, m_size };
	}

	Iterator end()
	{
		return { m_elements, m_size, m_size };
	}

	ConstIterator begin() const
	{
		{ m_elements, 0, m_size };
	}

	ConstIterator end() const
	{
		return { m_elements, m_size, m_size };
	}
    
private:
    size_t		  m_size;
    size_t		  m_capacity;
    T*			  m_elements;
};

TE_END_TERMINUS_NAMESPACE
