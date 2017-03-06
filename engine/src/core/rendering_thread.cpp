#include <core/rendering_thread.h>
#include <core/context.h>
#include <graphics/imgui_backend.h>
#include <core/config.h>

#include <utility/profiler.h>

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
        concurrent_queue::push(_graphics_upload_queue, task);
    }
    
    void RenderingThread::shutdown()
    {
        Context& context = global::get_context();
        
        context._renderer.shutdown();
        context._render_device.Shutdown();
    }
    
    void RenderingThread::exit()
    {
        _thread.join();
    }
    
    void RenderingThread::render_loop()
    {
        Context& context = global::get_context();
        
        context._main_ready_sema.wait();
        
        context._render_device.Initialize();
        context._renderer.initialize();
        
        ImGuiBackend* gui_backend = context::get_imgui_backend();
        
        gui_backend->initialize();
        gui_backend->new_frame();
        
        context._render_ready_sema.notify();
        
        while (!context._shutdown)
        {
            TERMINUS_BEGIN_CPU_PROFILE(renderer)
            // submit api calls
            context._renderer.submit();
            
            // do resource uploading. one task per frame for now.
            if(!concurrent_queue::empty(_graphics_upload_queue))
            {
                Task upload_task = concurrent_queue::pop(_graphics_upload_queue);
                upload_task._function.Invoke(&upload_task._data[0]);
                context._load_wakeup_sema.notify();
            }

			TERMINUS_END_CPU_PROFILE

            // notify done
            context._render_done_sema.notify();
            
            // wait for swap done
            context._swap_done_sema.wait();
            
            gui_backend->render();
            
            context._render_device.SwapBuffers();
            
        }
        
        shutdown();
        
        context._render_exit_sema.notify();
    }
    
    void submit_gpu_upload_task(Task& task)
    {
        Context& context = global::get_context();
        // queue task into rendering thread.
        context._rendering_thread.enqueue_upload_task(task);
        context._load_wakeup_sema.wait();
    }
}
