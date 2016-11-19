#include "Global.h"
#include <iostream>

namespace Terminus
{
    namespace Global
    {
        void* g_memory = nullptr;
        void* g_per_frame_memory = nullptr;
        LinearAllocator* g_allocator = nullptr;
        LinearAllocator* g_per_frame_allocator = nullptr;
        TaskScheduler*   g_task_scheduler = nullptr;
        
        void Initialize()
        {
            g_memory = malloc(MAX_MEMORY);
            g_allocator = new LinearAllocator(MAX_MEMORY, g_memory);
            
            //g_per_frame_memory = malloc(MAX_PER_FRAME_MEMORY);
            //g_per_frame_allocator = new LinearAllocator(MAX_PER_FRAME_MEMORY, g_per_frame_memory);
            
            //g_task_scheduler = T_NEW TaskScheduler();
        }
        
        LinearAllocator* GetDefaultAllocator()
        {
            return g_allocator;
        }
        
        LinearAllocator* GetPerFrameAllocator()
        {
            return g_per_frame_allocator;
        }
        
        TaskScheduler* GetTaskScheduler()
        {
            return g_task_scheduler;
        }
        
        void Shutdown()
        {
            std::cout << g_allocator->GetUsedMemory() << " / " << g_allocator->GetTotalMemory() << std::endl;
            
            if(g_allocator)
            {
                g_allocator->Clear();
                delete g_allocator;
                g_allocator = nullptr;
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
