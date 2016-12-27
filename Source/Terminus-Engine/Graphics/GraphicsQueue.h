#ifndef GRAPHICSQUEUE_H
#define GRAPHICSQUEUE_H

#include "CommandBuffer.h"

#define MAX_COMMAND_BUFFERS 32

namespace Terminus { namespace Graphics {
    
    struct GraphicsQueue
    {
        CommandBuffer m_command_buffers[MAX_COMMAND_BUFFERS];
        uint32 m_command_buffer_count;
        
        GraphicsQueue()
        {
            m_command_buffer_count = 0;
            
        }
        
        ~GraphicsQueue()
        {
            
        }
        
    };
    
} }

#endif
