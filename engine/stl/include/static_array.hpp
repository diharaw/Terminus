#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<typename T, size_t N>
class StaticArray
{
public:
    StaticArray() {}
    ~StaticArray() {}
    
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
};

TE_END_TERMINUS_NAMESPACE
