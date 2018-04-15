#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class StackAllocator : public IAllocator
{
private:
    struct AllocationHeader
    {
#ifdef _DEBUG
        void* m_previous_address;
#endif
        uint8_t m_adjustment;
    };
    
#ifdef _DEBUG
    void* m_previous_position;
#endif
    void* m_current_position;
    
public:
	StackAllocator();
	virtual ~StackAllocator();
	void initialize(IAllocator* backing, size_t max_size);
	void initialize(void* memory, size_t max_size);
	void* allocate(size_t size, size_t count, size_t align) override;
	void free(void* ptr) override;
    
private:
    void initialize_internal();
};

TE_END_TERMINUS_NAMESPACE
