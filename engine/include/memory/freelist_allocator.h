#pragma once

#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

#include <memory/allocator.h>

namespace terminus
{
    class FreeListAllocator : public Allocator
    {
    private:
        struct AllocationHeader
        {
            size_t m_size;
            uint8  m_adjustment;
        };
        
        struct FreeBlock
        {
            size_t     m_size;
            FreeBlock* m_next;
        };
        
        FreeBlock* m_free_blocks;
        
    public:
        FreeListAllocator(size_t _Size, void* _StartAddress);
        ~FreeListAllocator();
        void* Allocate(size_t _AllocationSize, uint8 _Alignment) override;
        void  Deallocate(void* _Address) override;
        
    private:
        FreeListAllocator(const FreeListAllocator&);
        FreeListAllocator& operator=(const FreeListAllocator&);
    };
}

#endif
