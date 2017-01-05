//
//  StackAllocator.cpp
//  CustomAllocator
//
//  Created by Fidenz on 8/5/16.
//  Copyright © 2016 Fidenz. All rights reserved.
//

#include "StackAllocator.h"

namespace terminus
{
    StackAllocator::StackAllocator(size_t _Size, void* _StartAddress) : Allocator(_Size, _StartAddress), m_CurrentPosition(_StartAddress)
    {
        assert(_Size > 0);
        
        #ifdef _DEBUG
        m_PreviousPosition = nullptr;
        #endif
    }
    
    StackAllocator::~StackAllocator()
    {
        #ifdef _DEBUG
        m_PreviousPosition = nullptr;
        #endif
        
        m_CurrentPosition = nullptr;
    }
    
    void* StackAllocator::Allocate(size_t _AllocationSize, uint8 _Alignment)
    {
        assert(_AllocationSize != 0);
        uint8 adjustment = terminus::AllocatorUtility::AlignFowardAdjustmentWithHeader(m_CurrentPosition, _Alignment, sizeof(AllocationHeader));
        
        if(m_UsedMemory + adjustment + _AllocationSize > m_TotalMemory)
            return nullptr;
        
        void* alignedAddress = terminus::AllocatorUtility::Add(m_CurrentPosition, adjustment);
        AllocationHeader* header = (AllocationHeader*)(terminus::AllocatorUtility::Subtract(alignedAddress, sizeof(AllocationHeader)));
        
        header->m_Adjustment = adjustment;
        
        #ifdef _DEBUG
        header->m_PreviousAddress = m_PreviousPosition;
        m_PreviousPosition = alignedAddress;
        #endif
        
        m_CurrentPosition = terminus::AllocatorUtility::Add(alignedAddress, _AllocationSize);
        m_UsedMemory += _AllocationSize + adjustment;
        m_NumAllocations++;
        
        return alignedAddress;
    }
    
    void  StackAllocator::Deallocate(void* _Address)
    {
        AllocationHeader* header = (AllocationHeader*)(terminus::AllocatorUtility::Subtract(_Address, sizeof(AllocationHeader)));
        m_UsedMemory -= (uintptr_t)m_CurrentPosition - (uintptr_t)_Address + header->m_Adjustment;
        
        m_CurrentPosition = terminus::AllocatorUtility::Subtract(_Address, header->m_Adjustment);
        
        #ifdef _DEBUG
        m_PreviousPosition = header->m_PreviousAddress;
        #endif
        
        m_NumAllocations--;
    }
}

