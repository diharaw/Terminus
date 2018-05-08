#pragma once

#include <stdint.h>
#include <assert.h>
#include <core/terminus_macros.hpp>
#include <memory/pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<size_t MAX_SIZE, size_t OBJECT_SIZE, uint8_t ALIGNMENT = 8>
class BufferPoolAllocator : public PoolAllocator
{
public:
	BufferPoolAllocator() : PoolAllocator(&m_buffer[0], MAX_SIZE, OBJECT_SIZE, ALIGNMENT) {}

private:
	uint8_t m_buffer[MAX_SIZE];
};

TE_END_TERMINUS_NAMESPACE
