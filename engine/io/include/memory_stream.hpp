#pragma once

#include <io/include/stream.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class MemoryStream : public IStream
{
public:
    MemoryStream(void* initial = nullptr, const size_t& size = 0);
    ~MemoryStream();
    void seek(const size_t& offset) override;
    void write(void* src, const size_t& size) override;
    void read(void* dst, const size_t& size) override;
    void reset() override;
	size_t size() override;

private:
    void reserve(const size_t& capacity);
    
private:
    void*		m_buffer;
    size_t		m_size;
};

TE_END_TERMINUS_NAMESPACE
