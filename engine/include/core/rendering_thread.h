#pragma once

#include <thread>
#include <thread/thread_pool.h>

namespace terminus
{
    using GraphicsUploadQueue = ConcurrentQueue<Task, 100>;
    
    class RenderingThread
    {
    public:
        RenderingThread();
        ~RenderingThread();
        void run();
        void enqueue_upload_task(Task& task);
        void shutdown();
        void exit();
        
    private:
        void render_loop();
        
    public:
        GraphicsUploadQueue _graphics_upload_queue;
        
    private:
        std::thread         _thread;
    };
    
    extern void submit_gpu_upload_task(Task& task);
}
