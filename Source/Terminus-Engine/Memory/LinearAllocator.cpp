#include "LinearAllocator.h"

namespace Terminus
{
    LinearAllocator::LinearAllocator(size_t _Size, void* _StartAddress) : Allocator(_Size, _StartAddress), m_CurrentPosition(_StartAddress)
    {
        
    }
    
    LinearAllocator::~LinearAllocator()
    {
        m_CurrentPosition = nullptr;
    }
    
    void* LinearAllocator::Allocate(size_t _AllocationSize, uint8 _Alignment)
    {
        assert(_AllocationSize != 0);
        
        uint8 adjustment = AllocatorUtility::AlignBackwardAdjustment(m_CurrentPosition, _Alignment);
        
        if (m_UsedMemory + adjustment + _AllocationSize > m_TotalMemory)
            return nullptr;
        
        uintptr_t alignedAddress = (uintptr_t)m_CurrentPosition + adjustment;
        m_CurrentPosition = (void*)(alignedAddress + _AllocationSize);
        m_NumAllocations++;
        m_UsedMemory += adjustment + _AllocationSize;
        
        return (void*)alignedAddress;
    }
    
    void  LinearAllocator::Deallocate(void* _Address)
    {
        
    }
    
    void LinearAllocator::Clear()
    {
        m_NumAllocations = 0;
        m_UsedMemory = 0;
        m_CurrentPosition = m_StartAddress;
    }
}
