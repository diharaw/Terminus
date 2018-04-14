#pragma once

#include <stdint.h>
#include <assert.h>
#include <core/include/terminus_macros.hpp>
#include <memory/src/pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<size_t SIZE>
class BufferPoolAllocator : public PoolAllocator
{
public:
	void initialize(size_t object_size, uint8_t alignment)
	{
        PoolAllocator::initialize(&m_buffer[0], SIZE, object_size, alignment);
	}

private:
	uint8_t m_buffer[SIZE];
};

TE_END_TERMINUS_NAMESPACE
