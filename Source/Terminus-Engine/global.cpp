#include <global.h>
#include <types.h>
#include <Core/context.h>
#include <Graphics/imgui_console.h>
#include <iostream>

namespace terminus
{
    namespace Global
    {
        void* g_memory = nullptr;
        void* g_per_frame_memory = nullptr;
        LinearAllocator* g_allocator = nullptr;
        LinearAllocator* g_per_frame_allocator = nullptr;
        DefaultThreadPool*   g_thread_pool = nullptr;
        Context* g_context = nullptr;
        
        void Initialize()
        {
            logger::initialize();
            
            logger::open_file_stream();
            
            g_memory = malloc(MAX_MEMORY);
            g_allocator = new LinearAllocator(MAX_MEMORY, g_memory);
            
            g_per_frame_memory = malloc(MAX_PER_FRAME_MEMORY);
            g_per_frame_allocator = new LinearAllocator(MAX_PER_FRAME_MEMORY, g_per_frame_memory);
            
			g_thread_pool = T_NEW DefaultThreadPool();
            
            g_context = T_NEW Context();
            g_context->_shutdown = false;
            
        #if defined(TERMINUS_WITH_EDITOR)
            imgui_console::initialize();
            logger::open_custom_stream(&imgui_console::logger_callback);
        #endif
        }
        
        Context& get_context()
        {
            return *g_context;
        }
        
        LinearAllocator* GetDefaultAllocator()
        {
            return g_allocator;
        }
        
        LinearAllocator* GetPerFrameAllocator()
        {
            return g_per_frame_allocator;
        }
        
        DefaultThreadPool* GetDefaultThreadPool()
        {
            return g_thread_pool;
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
            
        #if defined(TERMINUS_WITH_EDITOR)
            logger::close_custom_stream();
            imgui_console::shutdown();
        #endif
            
            logger::close_file_stream();
        }
    }
}

void* operator new (size_t size, unsigned line, const char* file)
{
    std::cout << "Performing Allocation of size " << size << " in File " << file << " at line number " << line << std::endl;
    return terminus::Global::g_allocator->Allocate(size, MEMORY_ALIGNMENT);
}
