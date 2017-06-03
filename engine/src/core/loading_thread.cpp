#include <core/loading_thread.h>
#include <core/context.h>
#include <utility/profiler.h>
#include <core/sync.h>

namespace terminus
{
    LoadingThread::LoadingThread()
    {
        
    }
    
    LoadingThread::~LoadingThread()
    {
        
    }
    
    void LoadingThread::run()
    {
        m_thread = std::thread(&LoadingThread::load_loop, this);
    }
    
    void LoadingThread::enqueue_load_task(Task* task)
    {
		m_loading_queue.push(task);
		sync::notify_loader_wakeup();
    }
    
	Task* LoadingThread::create_load_task()
	{
		return m_loading_queue.pop();
	}

    void LoadingThread::shutdown()
    {
        
    }
    
    void LoadingThread::exit()
    {
        m_thread.join();
    }
    
    void LoadingThread::load_loop()
    {
        Context& context = global::get_context();
        
		sync::wait_for_loader_wakeup();
        
        while (!context._shutdown)
        {
            while(!m_loading_queue.empty())
            {
                TERMINUS_BEGIN_CPU_PROFILE(load_loop);
                
                Task* load_task = m_loading_queue.pop();
                
				if (load_task)
					load_task->_function.Invoke(&load_task->_data[0]);
                
                TERMINUS_END_CPU_PROFILE;
            }
            
			sync::wait_for_loader_wakeup();
        }
        
		sync::notify_loader_exit();
    }
}
