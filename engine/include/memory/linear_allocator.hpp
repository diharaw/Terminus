#pragma once

#include <core/terminus_macros.hpp>
#include <memory/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class LinearAllocator : public IAllocator
{
public:
	LinearAllocator(void* memory, size_t max_size);
	virtual ~LinearAllocator();
	void* allocate(size_t size, size_t align) override;
	void deallocate(void* ptr) override;
	void clear();

protected:
	void* m_position;
};

TE_END_TERMINUS_NAMESPACE
