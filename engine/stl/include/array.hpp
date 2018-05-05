#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>

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
    
private:
    T m_elements[N];
	size_t m_size;
};

TE_END_TERMINUS_NAMESPACE
