#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class PoolAllocator : public IAllocator
{
public:
	PoolAllocator();
    virtual ~PoolAllocator();
	void initialize(void* memory, size_t max_size, size_t object_size, uint8_t alignment);
	void* allocate(size_t size, size_t align) override;
	void deallocate(void* ptr) override;

private:
    void initialize_internal();
    
private:
	size_t   m_object_size;
    uint8_t  m_object_alignment;
    void**   m_freelist;
};

TE_END_TERMINUS_NAMESPACE
