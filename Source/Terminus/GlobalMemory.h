#include "Memory/LinearAllocator.h"

#define MAX_MEMORY 100
#define MEMORY_ALIGNMENT 8
#define T_NEW new(__LINE__, __FILE__)

extern void* operator new (size_t size, unsigned line, const char* file);

namespace Terminus
{
    namespace Memory
    {
        extern void Initialize();
        extern LinearAllocator* DefaultAllocator();
        extern void Shutdown();
    }
}
