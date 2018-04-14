#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class PoolAllocator : public Allocator
{
public:
	PoolAllocator();
    virtual ~PoolAllocator();
    void initialize(Allocator* backing, size_t max_size, size_t object_size, uint8_t alignment);
	void initialize(void* memory, size_t max_size, size_t object_size, uint8_t alignment);
	void* allocate(size_t size, size_t count, size_t align) override;
	void free(void* ptr) override;

private:
    void initialize_internal();
    
private:
	size_t   m_object_size;
    uint8_t  m_object_alignment;
    void**   m_freelist;
};

TE_END_TERMINUS_NAMESPACE
