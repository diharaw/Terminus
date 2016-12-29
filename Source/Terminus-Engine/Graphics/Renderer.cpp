#include "Renderer.h"

namespace Terminus { namespace Graphics {
  
    Renderer::Renderer()
    {
        
    }
    
    Renderer::~Renderer()
    {
        
    }
    
    void Renderer::Submit()
    {
        
    }
    
    void Renderer::Sort()
    {
        ThreadPool* thread_pool = Global::GetDefaultThreadPool();
        
        
    }
    
    void Renderer::SortTask(void* data)
    {
        
    }
    
    GraphicsQueue& Renderer::GetGraphicsQueueFront()
    {
        return m_graphics_queues[m_front_queue_index];
    }
    
    GraphicsQueue& Renderer::GetGraphicsQueueBack()
    {
        return m_graphics_queues[!m_front_queue_index];
    }
    
} }
