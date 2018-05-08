#pragma once

#include <stdint.h>
#include <assert.h>
#include <core/terminus_macros.hpp>
#include <memory/linear_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

template<size_t SIZE>
class BufferLinearAllocator : public LinearAllocator
{
public:
	BufferLinearAllocator() : LinearAllocator(&m_buffer[0], SIZE) {}

private:
	uint8_t m_buffer[SIZE];
};

TE_END_TERMINUS_NAMESPACE
