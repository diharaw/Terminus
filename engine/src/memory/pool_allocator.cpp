#include <Memory/pool_allocator.h>

namespace terminus
{
    PoolAllocator::PoolAllocator(size_t _ObjectSize, uint8 _ObjectAlignment, size_t _Size, void* _StartAddress) : Allocator(_Size, _StartAddress), m_object_size(_ObjectSize), m_object_alignment(_ObjectAlignment)
    {
        assert(_ObjectSize >= sizeof(void*));
        uint8 adjustment = AllocatorUtility::AlignFowardAdjustment(_StartAddress, _ObjectAlignment);
        m_freelist = (void**)AllocatorUtility::Add(_StartAddress, adjustment);
        size_t num_objects = (_Size - adjustment) / _ObjectSize;
        
        void** p = m_freelist;
        
        for(int i = 0; i < num_objects - 1; i++)
        {
            *p = AllocatorUtility::Add(p, _ObjectSize);
            p = (void**) *p;
        }
        
        *p = nullptr;
    }
    
    PoolAllocator::~PoolAllocator()
    {
        m_freelist = nullptr;
    }
    
    void* PoolAllocator::Allocate(size_t _AllocationSize, uint8 _Alignment)
    {
        assert(_AllocationSize == m_object_size && _Alignment == m_object_alignment);
        
        if(m_freelist == nullptr)
            return nullptr;
        
        void* p = m_freelist;
        m_freelist = (void**)(*m_freelist);
        
        m_UsedMemory += _AllocationSize;
        m_NumAllocations++;
        
        return p;
    }
    
    void PoolAllocator::Deallocate(void* _Address)
    {
        *((void**)_Address) = m_freelist;
        m_freelist = (void**)_Address;
        m_UsedMemory -= m_object_size;
        m_NumAllocations--;
    }
}
