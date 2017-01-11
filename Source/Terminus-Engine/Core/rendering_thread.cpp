#include <Core/rendering_thread.h>
#include <Core/context.h>

namespace terminus
{
    RenderingThread::RenderingThread()
    {
        _thread = std::thread(&RenderingThread::render_loop, this);
    }
    
    RenderingThread::~RenderingThread()
    {
        _thread.join();
    }
    
    void RenderingThread::render_loop()
    {
        Context& context = Global::get_context();
        
        context._main_ready_sema.wait();
        
        context._render_device.Initialize();
        context._renderer.initialize();
        
        context._render_ready_sema.notify();
        
        while (!context._shutdown)
        {
            // submit api calls
            context._renderer.submit();
            
            // do resource loading
            
            
            // notify done
            context._render_done_sema.notify();
            
            // wait for swap done
            context._swap_done_sema.wait();
            
        }
    }
}
