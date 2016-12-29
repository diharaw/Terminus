#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderDevice.h"
#include "GraphicsQueue.h"

namespace Terminus { namespace Graphics {

	class Renderer
	{
    private:
        bool          m_front_queue_index;
        GraphicsQueue m_graphics_queues[2];
        
    public:
        Renderer();
        ~Renderer();
        void Submit();
        void Sort();
        GraphicsQueue& GetGraphicsQueueFront();
        GraphicsQueue& GetGraphicsQueueBack();
        
    private:
        void SortTask(void* data);
        
	};

} }

#endif
