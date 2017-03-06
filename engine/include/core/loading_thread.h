#pragma once

#include <thread/thread_pool.h>
#include <thread/semaphore.h>
#include <thread>

namespace terminus
{
    using LoadingQueue = ConcurrentQueue<Task, 100>;
    
    class LoadingThread
    {
    public:
        LoadingThread();
        ~LoadingThread();
        void run();
        void enqueue_load_task(Task& task);
        void shutdown();
        void exit();
        
    private:
        void load_loop();
        
    private:
        std::thread  _thread;
        LoadingQueue _loading_queue;
    };
}
