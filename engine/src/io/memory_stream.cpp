#include <io/memory_stream.hpp>
#include <core/engine_core.hpp>
#include <iostream>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

MemoryStream::MemoryStream(void* initial, const size_t& size)
{
	m_pointer = 0;

	// If initial data available, heap-allocate enough memory and memcpy initial data into it.
	if (size > 0)
	{
		m_buffer = TE_HEAP_ALLOC(size);
		m_size = size;
		memcpy(m_buffer, initial, m_size);
	}
	else
	{
		m_buffer = nullptr;
		m_size = 0;
	}
}

MemoryStream::~MemoryStream()
{
	if (m_buffer)
		TE_HEAP_DEALLOC(m_buffer);
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
    void* new_data = TE_HEAP_ALLOC(capacity);
    
	if (m_buffer)
	{
		memcpy(new_data, m_buffer, m_size);
		TE_HEAP_DEALLOC(m_buffer);
	}

    m_buffer = new_data;
	m_size = capacity;
}

size_t MemoryStream::size()
{
	return m_size;
}

void* MemoryStream::data()
{
	return m_buffer;
}

TE_END_TERMINUS_NAMESPACE