#pragma once

#include <stdint.h>
#include <assert.h>
#include <core/include/terminus_macros.hpp>
#include <memory/src/stack_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<size_t SIZE>
class BufferStackAllocator : public StackAllocator
{
public:
	BufferStackAllocator()
	{
        StackAllocator::initialize(&m_buffer[0], SIZE);
	}

private:
	uint8_t m_buffer[SIZE];
};

TE_END_TERMINUS_NAMESPACE
