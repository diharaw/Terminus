#include <io/include/memory_stream.hpp>
#include <iostream>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

MemoryStream::MemoryStream(IAllocator* allocator, void* initial, const size_t& size) : m_allocator(allocator)
{
	assert(allocator != nullptr);

	m_pointer = 0;

	if (size > 0)
	{
		m_buffer = initial;
		m_size = size;
		reserve(m_size);
	}
	else
	{
		m_buffer = nullptr;
		m_size = 0;
	}
}

MemoryStream::~MemoryStream()
{
	m_allocator->free(m_buffer);
}

void MemoryStream::seek(const size_t& offset)
{
	m_pointer += offset;
}

void MemoryStream::write(void* src, const size_t& size)
{
    m_size += size;
    reserve(m_size);
    memcpy(m_buffer, src, size);

	if (m_auto_move_write)
		seek(size);
}

void MemoryStream::read(void* dst, const size_t& size)
{
	uint8_t* ptr = (uint8_t*)m_buffer;
	memcpy(dst, ptr + m_pointer, size);

	if (m_auto_move_read)
		seek(size);
}

void MemoryStream::reset()
{
	m_pointer = 0;
}

void MemoryStream::reserve(const size_t& capacity)
{
    std::cout << "Reserving..." << std::endl;
    void* new_data = m_allocator->allocate(capacity, 1, 8);
    memcpy(new_data, m_buffer, m_size);
    free(m_buffer);
    m_buffer = new_data;
	m_size = capacity;
}

size_t MemoryStream::size()
{
	return m_size;
}

TE_END_TERMINUS_NAMESPACE