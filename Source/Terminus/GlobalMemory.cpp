#include "GlobalMemory.h"
#include <iostream>

namespace Terminus
{
    namespace Memory
    {
        void* g_memory = nullptr;
        LinearAllocator* g_allocator = nullptr;
        
        void Initialize()
        {
            g_memory = malloc(MAX_MEMORY);
            g_allocator = new LinearAllocator(MAX_MEMORY, g_memory);
        }
        
        LinearAllocator* DefaultAllocator()
        {
            return g_allocator;
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
    return Terminus::Memory::g_allocator->Allocate(size, MEMORY_ALIGNMENT);
}
