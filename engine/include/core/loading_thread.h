#pragma once

#include <resource/upload_queue.h>
#include <thread>

namespace terminus
{
	struct Task;

    using LoadingQueue = UploadQueue<Task, 100>;
    
    class LoadingThread
    {
    public:
        LoadingThread();
        ~LoadingThread();
        void run();
		Task* create_load_task();
        void enqueue_load_task(Task* task);
        void shutdown();
        void exit();
        
    private:
        void load_loop();
        
    private:
        std::thread  m_thread;
        LoadingQueue m_loading_queue;
    };
}
