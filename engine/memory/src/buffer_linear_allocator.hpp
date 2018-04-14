#pragma once

#include <stdint.h>
#include <assert.h>
#include <core/include/terminus_macros.hpp>
#include <memory/src/linear_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<size_t SIZE>
class BufferLinearAllocator : public LinearAllocator
{
public:
	BufferLinearAllocator()
	{
		m_size = SIZE;
		m_memory = &m_buffer[0];
		m_position = m_memory;
	}

private:
	uint8_t m_buffer[SIZE];
};

TE_END_TERMINUS_NAMESPACE
