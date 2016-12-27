#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderDevice.h"

namespace Terminus { namespace Graphics {

	namespace Renderer
	{
        inline void Render(RenderDevice& device)
        {
            
        }
        
        inline void SortQueue(GraphicsQueue& queue)
        {
            ThreadPool* thread_pool = Global::GetDefaultThreadPool();
            
            
        }
        
        inline void SortTask(void* data)
        {
            
        }
	};

} }

#endif
