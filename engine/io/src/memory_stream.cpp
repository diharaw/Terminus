#include "memory_stream.hpp"
#include <iostream>

MemoryStream::MemoryStream(const size_t& capacity) : m_buffer(nullptr), m_capacity(capacity), m_size(0)
{
    m_buffer = malloc(m_capacity);
}

MemoryStream::~MemoryStream()
{
    free(m_buffer);
}

void MemoryStream::seek(const size_t& offset)
{
    
}

void MemoryStream::write(void* src, const size_t& size)
{
    m_size += size;
    
    if (m_size > ((m_capacity/100.0f) * 70.0f))
        reserve(m_size * 2);
    
    memcpy(m_buffer, src, size);
}

void MemoryStream::read(void* dst, const size_t& size)
{
    
}

void MemoryStream::clear()
{
    m_size = 0;
}

void MemoryStream::reserve(size_t capacity)
{
    std::cout << "Reserving..." << std::endl;
    void* new_data = malloc(capacity);
    memcpy(new_data, m_buffer, m_capacity);
    free(m_buffer);
    m_buffer = new_data;
    m_capacity = capacity;
}
