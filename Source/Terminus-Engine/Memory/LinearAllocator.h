#pragma once

#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

#include "Allocator.h"

namespace Terminus
{
    class LinearAllocator : public Allocator
    {
    private:
        void* m_CurrentPosition;
        
    public:
        LinearAllocator(size_t _Size, void* _StartAddress);
        ~LinearAllocator();
        void* Allocate(size_t _AllocationSize, uint8 _Alignment) override;
        void  Deallocate(void* _Address) override;
        void Clear();
    };
}

#endif