#ifndef GRAPHICSQUEUE_H
#define GRAPHICSQUEUE_H

#include <graphics/command_buffer.h>

#define MAX_COMMAND_BUFFERS 32
#define PER_FRAME_UNIFORM_MEMORY 2
#define PER_FRAME_UNIFORM_MEMORY_BYTES PER_FRAME_UNIFORM_MEMORY*MB_IN_BYTES

namespace terminus
{
	class LinearAllocator;

    class GraphicsQueue
    {
	public:
		GraphicsQueue();
		~GraphicsQueue();
		uint32 create_command_buffer();
        
	public:
		CommandBuffer _cmd_buf[MAX_COMMAND_BUFFERS];
		uint32 _num_cmd_buf;
		LinearAllocator* _allocator;
    };
}

#endif
