#pragma once

#include <thread>

namespace terminus
{
    class RenderingThread
    {
    public:
        RenderingThread();
        ~RenderingThread();
        
    private:
        void render_loop();
        
    private:
        std::thread _thread;
    };
}
