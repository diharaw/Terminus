#pragma once

#ifndef TERMINUS_GLOBAL_H
#define TERMINUS_GLOBAL_H

#include <memory/linear_allocator.h>
#include <thread/thread_pool.h>
#include <io/logger.h>
#include <core/config.h>

#define MB_IN_BYTES 1024*1024
#define MAX_MEMORY_MB 200
#define MAX_MEMORY MAX_MEMORY_MB*MB_IN_BYTES
#define MAX_PER_FRAME_MEMORY 100
#define MEMORY_ALIGNMENT 8
#define T_NEW new(__LINE__, __FILE__)

extern void* operator new (size_t size, unsigned line, const char* file);

namespace terminus
{
    using DefaultThreadPool = ThreadPool;
    class Platform;
    class ImGuiBackend;
    
    namespace global
    {
        extern void initialize();
        extern void set_platform(Platform* platform);
        extern void set_imgui_backend(ImGuiBackend* backend);
        extern LinearAllocator* get_default_allocator();
        extern LinearAllocator* get_per_frame_allocator();
        extern DefaultThreadPool*   get_default_threadpool();
        extern void shutdown();
    }
}

#endif
