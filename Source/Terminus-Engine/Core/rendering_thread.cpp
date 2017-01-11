#include <Core/rendering_thread.h>
#include <Core/context.h>

namespace terminus
{
    RenderingThread::RenderingThread()
    {
        
    }
    
    RenderingThread::~RenderingThread()
    {
    
    }
    
    void RenderingThread::run()
    {
        _thread = std::thread(&RenderingThread::render_loop, this);
    }
    
    void RenderingThread::enqueue_upload_task(Task& task)
    {
        
    }
    
    void RenderingThread::shutdown()
    {
        Context& context = Global::get_context();
        
        context._renderer.shutdown();
        context._render_device.Shutdown();
        
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
            
            // do resource uploading. one task per frame for now.
            if(!concurrent_queue::empty(context._graphics_upload_queue))
            {
                Task upload_task = concurrent_queue::pop(context._graphics_upload_queue);
                upload_task._function.Invoke(&upload_task._data[0]);
            }
            
            // notify done
            context._render_done_sema.notify();
            
            // wait for swap done
            context._swap_done_sema.wait();
            
        }
        
        shutdown();
    }
}
