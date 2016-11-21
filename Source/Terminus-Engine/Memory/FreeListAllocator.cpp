#include "FreeListAllocator.h"

namespace Terminus
{
    FreeListAllocator::FreeListAllocator(size_t _Size, void* _StartAddress) : Allocator(_Size, _StartAddress), m_free_blocks((FreeBlock*)_StartAddress)
    {
        assert(_Size > sizeof(FreeBlock));
        
        m_free_blocks->m_size = _Size;
        m_free_blocks->m_next = nullptr;
    }
    
    FreeListAllocator::~FreeListAllocator()
    {
        m_free_blocks = nullptr;
    }
    
    void* FreeListAllocator::Allocate(size_t _AllocationSize, uint8 _Alignment)
    {
        assert(_AllocationSize != 0 && _Alignment != 0);
        
        FreeBlock* prev_free_block = nullptr;
        FreeBlock* free_block = m_free_blocks;
        
        while (free_block != nullptr)
        {
            uint8 adjustment = AllocatorUtility::AlignFowardAdjustmentWithHeader(free_block, _Alignment, sizeof(AllocationHeader));
            size_t total_size = _AllocationSize + adjustment;
            
            if(free_block->m_size < total_size)
            {
                prev_free_block = free_block;
                free_block = free_block->m_next;
                continue;
            }
            
            static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");
            
            if(free_block->m_size - total_size <= sizeof(AllocationHeader))
            {
                total_size = free_block->m_size;
                
                if(prev_free_block != nullptr)
                    prev_free_block->m_next = free_block->m_next;
                else
                    m_free_blocks = free_block->m_next;
            }
            else
            {
                FreeBlock* next_block = (FreeBlock*)(AllocatorUtility::Add(free_block, total_size));
                next_block->m_size = free_block->m_size - total_size;
                next_block->m_next = free_block->m_next;
                
                if(prev_free_block != nullptr)
                    prev_free_block->m_next = next_block;
                else
                    m_free_blocks = next_block;
            }
            
            uintptr_t aligned_address = (uintptr_t)free_block + adjustment;
            AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
            header->m_size = total_size;
            header->m_adjustment = adjustment;
            
            m_UsedMemory += total_size;
            m_NumAllocations++;
            
            assert(AllocatorUtility::AlignFowardAdjustment((void*)aligned_address, _Alignment) == 0);
            
            return (void*) aligned_address;
        }
        
        return nullptr;
    }
    
    void  FreeListAllocator::Deallocate(void* _Address)
    {
        assert(_Address != nullptr);
        
        AllocationHeader* header = (AllocationHeader*)AllocatorUtility::Subtract(_Address, sizeof(AllocationHeader));
        
        uintptr_t block_start = reinterpret_cast<uintptr_t>(_Address) - header->m_adjustment;
        size_t    block_size  = header->m_size;
        uintptr_t block_end   = block_start + block_size;
        
        FreeBlock* prev_free_block = nullptr;
        FreeBlock* free_block = m_free_blocks;
        
        while(free_block != nullptr)
        {
            if((uintptr_t)free_block >= block_end)
                break;
            
            prev_free_block = free_block;
            free_block = free_block->m_next;
        }
        
        if(prev_free_block == nullptr)
        {
            prev_free_block = (FreeBlock*)block_start;
            prev_free_block->m_size = block_size;
            prev_free_block->m_next = m_free_blocks;
            
            m_free_blocks = prev_free_block;
        }
        else if ((uintptr_t)prev_free_block + prev_free_block->m_size == block_size)
        {
            prev_free_block->m_size += block_size;
        }
        else
        {
            FreeBlock* temp = (FreeBlock*)block_start;
            temp->m_size = block_size;
            temp->m_next = prev_free_block->m_next;
            prev_free_block->m_next = temp;
            prev_free_block = temp;
        }
        
        if( free_block != nullptr && (uintptr_t) free_block == block_end)
        {
            prev_free_block->m_size += free_block->m_size;
            prev_free_block->m_next = free_block->m_next;
        }
        
        m_NumAllocations--;
        m_UsedMemory -= block_size;
    }
}