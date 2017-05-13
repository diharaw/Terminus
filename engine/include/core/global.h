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
#define TE_NEW(allocator) new(allocator, __LINE__, __FILE__)
#define TE_DELETE(object, allocator) delete(object, allocator, __LINE__, __FILE__)

extern void* operator new (size_t size, unsigned line, const char* file);

template <class T>
void* operator new(size_t bytes, T* allocator, int line, const char* file)
{
	return allocator->Allocate(bytes, 8);
}

template <class T>
void operator delete(void* ptr, T* allocator, int line, const char* file)
{
	allocator->Deallocate(ptr);
}

namespace terminus
{
    using DefaultThreadPool = ThreadPool;
    class Platform;
    class ImGuiBackend;

	extern Allocator* g_static_allocator;
	extern Allocator* g_heap_allocator;
    
    namespace global
    {
		extern Allocator* static_allocator();
		extern Allocator* heap_allocator();
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
