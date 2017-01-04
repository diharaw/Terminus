#pragma once

#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include "Allocator.h"

namespace terminus
{
    class PoolAllocator : public Allocator
    {
    private:
        size_t m_object_size;
        uint8  m_object_alignment;
        void** m_freelist;
        
    public:
        PoolAllocator(size_t _ObjectSize, uint8 _ObjectAlignment, size_t _Size, void* _StartAddress);
        ~PoolAllocator();
        void* Allocate(size_t _AllocationSize, uint8 _Alignment) override;
        void  Deallocate(void* _Address) override;
    };
}

#endif
