#pragma once

#include <thread>
#include <Thread/thread_pool.h>

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
        
    private:
        std::thread         _thread;
        GraphicsUploadQueue _graphics_upload_queue;
    };
}
