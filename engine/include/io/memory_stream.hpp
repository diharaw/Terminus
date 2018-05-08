#pragma once

#include <io/stream.hpp>
#include <memory/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// A stream that writes into and reads from a heap-allocated buffer which will continue growing as needed by write operations.
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
	void* data();

private:
    void reserve(const size_t& capacity);
    
private:
    void*		m_buffer;
    size_t		m_size;
};

TE_END_TERMINUS_NAMESPACE
