#pragma once

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdint>
#include <assert.h>
#include <new>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

namespace terminus
{
    namespace AllocatorUtility
    {
        // Memory Alignment
        
        void* AlignFoward(void* _Address, uint8 _Alignment);
        const void* AlignFoward(const void* _Address, uint8 _Alignment);
        
        void* AlignBackward(void* _Address, uint8 _Alignment);
        const void* AlignBackward(const void* _Address, uint8 _Alignment);
        
        // Alignment Adjustment Offset Helpers
        
        uint8 AlignFowardAdjustment(void* _Address, uint8 _Alignment);
        uint8 AlignFowardAdjustmentWithHeader(void* _Address, uint8 _Alignment, uint8 _HeaderSize);
        uint8 AlignBackwardAdjustment(void* _Address, uint8 _Alignment);
        
        // Pointer Arithmetic
        
        void* Add(void* _Pointer, size_t _Size);
        const void* Add(const void* _Pointer, size_t _Size);
        
        void* Subtract(void* _Pointer, size_t _Size);
        const void* Subtract(const void* _Pointer, size_t _Size);
    }
    
    class Allocator
    {
    protected:
        void*  m_StartAddress;
        
    public:
        size_t m_TotalMemory;
        size_t m_UsedMemory;
        size_t m_NumAllocations;
        
    public:
        Allocator(size_t _Size, void* _StartAddress)
        {
            m_StartAddress = _StartAddress;
            m_TotalMemory = _Size;
            
            m_UsedMemory = 0;
            m_NumAllocations = 0;
        }
        
        virtual ~Allocator()
        {
            assert(m_NumAllocations == 0 && m_UsedMemory == 0);
            
            m_StartAddress = nullptr;
            m_TotalMemory = 0;
        }
        
        virtual void* Allocate(size_t _AllocationSize, uint8 _Alignment) = 0;
        virtual void  Deallocate(void* _Address) = 0;

        inline void*  GetStart() { return m_StartAddress; }
        inline size_t GetTotalMemory() { return m_TotalMemory; }
        inline size_t GetUsedMemory() { return m_UsedMemory; }
        inline size_t GetNumAllocations() { return m_NumAllocations; }
    };
    
    namespace AllocatorUtility
    {
        inline void* AlignFoward(void* _Address, uint8 _Alignment)
        {
            return (void*)((reinterpret_cast<uintptr_t>(_Address) + static_cast<uintptr_t>(_Alignment - 1)) & (static_cast<uintptr_t>(~(_Alignment - 1))));
        }
        
        inline const void* AlignFoward(const void* _Address, uint8 _Alignment)
        {
            return (void*)((reinterpret_cast<uintptr_t>(_Address) + static_cast<uintptr_t>(_Alignment - 1)) & (static_cast<uintptr_t>(~(_Alignment - 1))));
        }
        
        inline void* AlignBackward(void* _Address, uint8 _Alignment)
        {
            return (void*)(reinterpret_cast<uintptr_t>(_Address) & static_cast<uintptr_t>(~(_Alignment - 1)));
        }
        
        inline const void* AlignBackward(const void* _Address, uint8 _Alignment)
        {
            return (void*)(reinterpret_cast<uintptr_t>(_Address) & static_cast<uintptr_t>(~(_Alignment - 1)));
        }
        
        inline uint8 AlignFowardAdjustment(void* _Address, uint8 _Alignment)
        {
            uint8 adjustment = _Alignment - (reinterpret_cast<uintptr_t>(_Address) & static_cast<uintptr_t>(_Alignment - 1));
            
            if (adjustment == _Alignment)
                return 0;
            
            return adjustment;
        }
        
        inline uint8 AlignFowardAdjustmentWithHeader(void* _Address, uint8 _Alignment, uint8 _HeaderSize)
        {
            uint8 adjustment = AlignFowardAdjustment(_Address, _Alignment);
            uint8 neededSpace = _HeaderSize;
            
            if (adjustment < neededSpace)
            {
                neededSpace -= adjustment;
                adjustment += _Alignment * (neededSpace / _Alignment);
                
                if (neededSpace % _Alignment > 0)
                    adjustment += _Alignment;
            }
            
            return adjustment;
        }
        
        inline uint8 AlignBackwardAdjustment(void* _Address, uint8 _Alignment)
        {
            uint8 adjustment = (reinterpret_cast<uintptr_t>(_Address) & static_cast<uintptr_t>(~(_Alignment - 1)));
            
            if (adjustment == _Alignment)
                return 0;
            
            return adjustment;
        }
        
        inline void* Add(void* _Pointer, size_t _Size)
        {
            return (void*)(reinterpret_cast<uintptr_t>(_Pointer) + _Size);
        }
        
        inline const void* Add(const void* _Pointer, size_t _Size)
        {
            return (const void*)(reinterpret_cast<uintptr_t>(_Pointer) + _Size);
        }
        
        inline void* Subtract(void* _Pointer, size_t _Size)
        {
            return (void*)(reinterpret_cast<uintptr_t>(_Pointer) - _Size);
        }
        
        inline const void* Subtract(const void* _Pointer, size_t _Size)
        {
            return (const void*)(reinterpret_cast<uintptr_t>(_Pointer) - _Size);
        }
    }
}

#endif
