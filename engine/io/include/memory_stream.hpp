#pragma once

#include "stream.hpp"

class MemoryStream : public Stream
{
public:
    MemoryStream(const size_t& capacity = 1024);
    ~MemoryStream();
    void seek(const size_t& offset) override;
    void write(void* src, const size_t& size) override;
    void read(void* dst, const size_t& size) override;
    void clear() override;
    void reserve(size_t capacity) override;
    
private:
    void*  m_buffer;
    size_t m_capacity;
    size_t m_size;
};
