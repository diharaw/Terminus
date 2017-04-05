#include <core/rendering_thread.h>
#include <core/context.h>
#include <graphics/imgui_backend.h>
#include <core/config.h>
#include <core/sync.h>

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
        context._render_device.shutdown();
    }
    
    void RenderingThread::exit()
    {
        _thread.join();
    }
    
    void RenderingThread::render_loop()
    {
        Context& context = global::get_context();
        
		sync::wait_for_main_ready();
        
        context._render_device.initialize();
        context._renderer.initialize();
        
        ImGuiBackend* gui_backend = context::get_imgui_backend();
        
        gui_backend->initialize();
        gui_backend->new_frame();
        
		sync::notify_renderer_ready();
        
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
				sync::notify_loader_wakeup();
            }

			TERMINUS_END_CPU_PROFILE

            // notify done
			sync::notify_renderer_done();
            
            // wait for swap done
			sync::wait_for_swap_done();
            
            gui_backend->render();
            
            context._render_device.swap_buffers();
            
        }
        
        shutdown();
        
		sync::notify_renderer_exit();
    }
    
    void submit_gpu_upload_task(Task& task)
    {
        Context& context = global::get_context();
        // queue task into rendering thread.
        context._rendering_thread.enqueue_upload_task(task);
		sync::wait_for_loader_wakeup();
    }
}
