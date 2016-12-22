#include "Global.h"
#include "Types.h"
#include <iostream>

namespace Terminus
{
    namespace Global
    {
        void* g_memory = nullptr;
        void* g_per_frame_memory = nullptr;
        LinearAllocator* g_allocator = nullptr;
        LinearAllocator* g_per_frame_allocator = nullptr;
        ThreadPool*   g_thread_pool = nullptr;
        ThreadPool*   g_rendering_thread_pool = nullptr;
        
        void Initialize()
        {
            g_memory = malloc(MAX_MEMORY);
            g_allocator = new LinearAllocator(MAX_MEMORY, g_memory);
            
            g_per_frame_memory = malloc(MAX_PER_FRAME_MEMORY);
            g_per_frame_allocator = new LinearAllocator(MAX_PER_FRAME_MEMORY, g_per_frame_memory);
            
			g_thread_pool = T_NEW ThreadPool();
			g_thread_pool->Initialize();
            
            g_rendering_thread_pool = T_NEW ThreadPool();
            g_rendering_thread_pool->Initialize(1);
        }
        
        LinearAllocator* GetDefaultAllocator()
        {
            return g_allocator;
        }
        
        LinearAllocator* GetPerFrameAllocator()
        {
            return g_per_frame_allocator;
        }
        
        ThreadPool* GetDefaultThreadPool()
        {
            return g_thread_pool;
        }
        
        ThreadPool* GetRenderingThreadPool()
        {
            return g_rendering_thread_pool;
        }
        
        void Shutdown()
        {
            if(g_per_frame_memory)
            {
                g_per_frame_allocator->Clear();
                std::cout << "Per-Frame Allocator : "<< g_per_frame_allocator->GetUsedMemory() << " / " << g_per_frame_allocator->GetTotalMemory() << std::endl;
                T_SAFE_DELETE(g_per_frame_allocator);
            }
            
            free(g_per_frame_memory);
            
            if(g_allocator)
            {
                g_allocator->Clear();
                std::cout << "Default Allocator : "<< g_allocator->GetUsedMemory() << " / " << g_allocator->GetTotalMemory() << std::endl;
                T_SAFE_DELETE(g_allocator);
            }
            
            free(g_memory);
        }
    }
}

void* operator new (size_t size, unsigned line, const char* file)
{
    std::cout << "Performing Allocation of size " << size << " in File " << file << " at line number " << line << std::endl;
    return Terminus::Global::g_allocator->Allocate(size, MEMORY_ALIGNMENT);
}
