#pragma once

#include <graphics/command_buffer.h>
#include <container/packed_array.h>
#include <memory/pool_allocator.h>

#define MAX_COMMAND_BUFFERS 32
#define COMMAND_POOL_SIZE MAX_COMMAND_BUFFERS * MAX_COMMAND_BUFFER_SIZE_BYTES

namespace terminus
{
	struct CommandPool
	{
		CommandPool();

		uint32_t _num_allocated;
		PoolAllocator* _allocator;
		PackedArray<CommandBuffer*, MAX_COMMAND_BUFFERS> _allocated_list;
		uint8_t _buffer[COMMAND_POOL_SIZE];
		uint8_t _allocator_mem[sizeof(PoolAllocator)];
	};
}
