#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct OsAllocatorStats
{
	size_t reserved;
	size_t commited;
};

class OsAllocator
{
public:
	OsAllocator();
	~OsAllocator();

	// Retrieve allocator stats.
	OsAllocatorStats stats();

	// Reserve virtual address space.
	void* reserve(size_t size);

	// Commit the virtual address space to physical memory.
	void commit(void* address, size_t size);

	// Reserve and commit memory in one go.
	void* reserve_commit(size_t size);

	// Decommit previously commited memory.
	void  decommit(void* address, size_t size);

	// Release previously reserved virtual addressed space.
	void  free(void* address, size_t size);

private:
	size_t m_reserved;
	size_t m_commited;
};

TE_END_TERMINUS_NAMESPACE