#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T>
class DynamicArray
{
public:
    DynamicArray() : m_num_elements(0), m_elements(nullptr) {}
    DynamicArray(const size_t& size)
    {
        resize(size);
    }
    ~DynamicArray()
    {
        clear();
    }
    
    void resize(const size_t& size)
    {
        if (m_elements)
            delete[] m_elements;
        
        m_num_elements = size;
        m_elements = new T[m_num_elements];
    }
    
    void clear()
    {
        if (m_elements)
            delete[] m_elements;
        
        m_num_elements = 0;
        m_elements = new T[m_num_elements];
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
    size_t m_num_elements;
    T*     m_elements;
};

TE_END_TERMINUS_NAMESPACE
