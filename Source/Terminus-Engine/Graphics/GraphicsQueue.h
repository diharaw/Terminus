#ifndef GRAPHICSQUEUE_H
#define GRAPHICSQUEUE_H

#include "CommandBuffer.h"
#include "../Global.h"

#define MAX_COMMAND_BUFFERS 32

namespace Terminus { namespace Graphics {
    
    struct GraphicsQueue
    {
        CommandBuffer m_cmd_buf[MAX_COMMAND_BUFFERS];
        uint32 m_num_cmd_buf;
        
        GraphicsQueue()
        {
            m_num_cmd_buf = 0;
        }
        
        ~GraphicsQueue()
        {
            
        }
        
        inline uint32 CreateCommandBuffer()
        {
            uint32 cmd_buf_idx = m_num_cmd_buf;
         
            // TODO : Use pool allocator;
            void* mem = Global::GetDefaultAllocator()->Allocate(COMMAND_BUFFER_SIZE_BYTES, 8);
            m_cmd_buf[cmd_buf_idx].m_memory = mem;
            m_cmd_buf[cmd_buf_idx].m_pos = mem;
            m_cmd_buf[cmd_buf_idx].WriteEnd();
            m_num_cmd_buf++;
            
            return cmd_buf_idx;
        }
        
    };
    
} }

#endif
