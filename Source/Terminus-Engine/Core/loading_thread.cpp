#include <Core/loading_thread.h>
#include <Core/context.h>

namespace terminus
{
    LoadingThread::LoadingThread()
    {
        concurrent_queue::clear(_loading_queue);
    }
    
    LoadingThread::~LoadingThread()
    {
        
    }
    
    void LoadingThread::run()
    {
        _thread = std::thread(&LoadingThread::load_loop, this);
    }
    
    void LoadingThread::enqueue_load_task(Task& task)
    {
        concurrent_queue::push(_loading_queue, task);
        Global::get_context()._load_wakeup_sema.notify();
    }
    
    void LoadingThread::shutdown()
    {
        
    }
    
    void LoadingThread::exit()
    {
        _thread.join();
    }
    
    void LoadingThread::load_loop()
    {
        Context& context = Global::get_context();
        
        context._load_wakeup_sema.wait();
        
        while (!context._shutdown)
        {
            while(!concurrent_queue::empty(_loading_queue))
            {
                Task load_task = concurrent_queue::pop(_loading_queue);
                load_task._function.Invoke(&load_task._data[0]);
                
                while(true)
                {
                    if(concurrent_queue::empty(context._rendering_thread._graphics_upload_queue))
                        break;
                    else
                        context._load_wakeup_sema.wait();
                }
            }
            
            context._load_wakeup_sema.wait();
        }
        
        context._load_exit_sema.notify();
    }
}
