#ifndef GRAPHICSQUEUE_H
#define GRAPHICSQUEUE_H

#include <graphics/command_buffer.h>
#include <core/global.h>

#define MAX_COMMAND_BUFFERS 32
#define PER_FRAME_UNIFORM_MEMORY 1
#define PER_FRAME_UNIFORM_MEMORY_BYTES PER_FRAME_UNIFORM_MEMORY*MB_IN_BYTES

namespace terminus
{
    struct GraphicsQueue
    {
        CommandBuffer m_cmd_buf[MAX_COMMAND_BUFFERS];
        uint32 m_num_cmd_buf;
        LinearAllocator* m_allocator;
        
        GraphicsQueue()
        {
            m_num_cmd_buf = 0;
            void* allocator_memory = global::get_default_allocator()->Allocate(PER_FRAME_UNIFORM_MEMORY_BYTES, 8);
            m_allocator = T_NEW LinearAllocator(PER_FRAME_UNIFORM_MEMORY_BYTES, allocator_memory);
        }
        
        ~GraphicsQueue()
        {
            
        }
        
        inline uint32 CreateCommandBuffer()
        {
            uint32 cmd_buf_idx = m_num_cmd_buf;
         
            // TODO : Use pool allocator;
            void* mem = global::get_default_allocator()->Allocate(COMMAND_BUFFER_SIZE_BYTES, 8);
            m_cmd_buf[cmd_buf_idx].m_memory = mem;
            m_cmd_buf[cmd_buf_idx].m_pos = mem;
            m_cmd_buf[cmd_buf_idx].WriteEnd();
            m_num_cmd_buf++;
            
            return cmd_buf_idx;
        }
        
    };
}

#endif
