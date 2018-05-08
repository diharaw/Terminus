#pragma once

#include <core/terminus_macros.hpp>
#include <io/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T, size_t N>
class Array
{
public:
    Array() : m_size(0) {}
    ~Array() {}

	void clear()
	{
		m_size = 0;
	}

	void resize(const size_t& size)
	{
		m_size = size;
	}

	size_t size()
	{
		return m_size;
	}

	void push_back(T obj)
	{
		m_elements[m_size++] = obj;
	}
    
    T& operator [](const uint32_t& idx)
    {
        return m_elements[idx];
    }
    
    T operator [](const uint32_t& idx) const
    {
        return m_elements[idx];
    }

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
		return{ m_elements, 0, N };
	}

	Iterator end()
	{
		return{ m_elements, N, N };
	}

	ConstIterator begin() const
	{
		{ m_elements, 0, N };
	}

	ConstIterator end() const
	{
		return{ m_elements, N, N };
	}
    
private:
    T m_elements[N];
	size_t m_size;
};

TE_END_TERMINUS_NAMESPACE
