#include <Core/rendering_thread.h>
#include <Core/context.h>
#include <Graphics/imgui_backend.h>
#include <Core/config.h>

#include <Utility/Remotery.h>

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
    }
    
    void RenderingThread::exit()
    {
        _thread.join();
    }
    
    void RenderingThread::render_loop()
    {
        Context& context = Global::get_context();
        
        context._main_ready_sema.wait();
        
        context._render_device.Initialize();
        context._renderer.initialize();
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize();
        ImGuiBackend::new_frame();
#endif
        
        context._render_ready_sema.notify();
        
        while (!context._shutdown)
        {
            TERMINUS_BEGIN_CPU_PROFILE(render_loop)
            // submit api calls
            context._renderer.submit();
            
            // do resource uploading. one task per frame for now.
            if(!concurrent_queue::empty(_graphics_upload_queue))
            {
                Task upload_task = concurrent_queue::pop(_graphics_upload_queue);
                upload_task._function.Invoke(&upload_task._data[0]);
            }
            
            // notify done
            context._render_done_sema.notify();
            
            // wait for swap done
            context._swap_done_sema.wait();
            TERMINUS_END_CPU_PROFILE
        }
        
        shutdown();
        
        context._render_exit_sema.notify();
    }
}
