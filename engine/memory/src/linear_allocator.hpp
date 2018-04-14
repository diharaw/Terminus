#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class LinearAllocator : public Allocator
{
public:
	LinearAllocator();
	virtual ~LinearAllocator();
	void initialize(Allocator* backing, size_t max_size);
	void initialize(void* memory, size_t max_size);
	void* allocate(size_t size, size_t count, size_t align) override;
	void free(void* ptr) override;
	void clear();

protected:
	void* m_position;
};

TE_END_TERMINUS_NAMESPACE
