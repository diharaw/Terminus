#include <graphics/graphics_queue.h>
#include <memory/linear_allocator.h>

namespace terminus
{
	GraphicsQueue::GraphicsQueue()
	{
		_num_cmd_buf = 0;
		void* allocator_memory = global::get_default_allocator()->Allocate(PER_FRAME_UNIFORM_MEMORY_BYTES, 8);
		_allocator = T_NEW LinearAllocator(PER_FRAME_UNIFORM_MEMORY_BYTES, allocator_memory);
	}

	GraphicsQueue::~GraphicsQueue()
	{

	}

	uint32 GraphicsQueue::create_command_buffer()
	{
		uint32 cmd_buf_idx = _num_cmd_buf;
		_num_cmd_buf++;
		return cmd_buf_idx;
	}
}