#include "Memory/LinearAllocator.h"
#include "Thread/TaskScheduler.h"

#define MB_IN_BYTES 1024*1024
#define MAX_MEMORY_MB 10
#define MAX_MEMORY MAX_MEMORY_MB*MB_IN_BYTES
#define MAX_PER_FRAME_MEMORY 100
#define MEMORY_ALIGNMENT 8
#define T_NEW new(__LINE__, __FILE__)

extern void* operator new (size_t size, unsigned line, const char* file);

namespace Terminus
{
    namespace Global
    {
        extern void Initialize();
        extern LinearAllocator* GetDefaultAllocator();
        extern LinearAllocator* GetPerFrameAllocator();
        extern TaskScheduler*   GetTaskScheduler();
        extern void Shutdown();
    }
}
