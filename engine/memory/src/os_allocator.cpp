#include <memory/src/os_allocator.hpp>
#include <assert.h>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__APPLE__)
#include <sys/mman.h>
#else

#endif

TE_BEGIN_TERMINUS_NAMESPACE

OsAllocator::OsAllocator()
{
	m_reserved = 0;
	m_commited = 0;
}

OsAllocator::~OsAllocator()
{
	m_reserved = 0;
	m_commited = 0;
}

OsAllocatorStats OsAllocator::stats()
{
	OsAllocatorStats stats;
	stats.reserved = m_reserved;
	stats.commited = m_commited;

	return stats;
}

#if defined(_WIN32)

void* OsAllocator::reserve(size_t size)
{
	m_reserved += size;
	return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
}

void OsAllocator::commit(void* address, size_t size)
{
	assert(address);
	m_commited += size;
	void* ptr = VirtualAlloc(address, size, MEM_COMMIT, PAGE_READWRITE);
	assert(ptr);
}

void* OsAllocator::reserve_commit(size_t size)
{
	m_commited += size;
	m_reserved += size;
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void OsAllocator::decommit(void* address, size_t size)
{
	if (address)
	{
		m_commited -= size;
		VirtualFree((void*)address, size, MEM_DECOMMIT);
	}
}

void OsAllocator::free(void* address, size_t size)
{
	if (address)
	{
		m_reserved -= size;
		VirtualFree((void*)address, 0, MEM_RELEASE);
	}
}

#else

void* OsAllocator::reserve(size_t size)
{
	m_reserved += size;
	void * ptr = mmap((void*)0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
	msync(ptr, size, MS_SYNC | MS_INVALIDATE);
	return ptr;
}

void OsAllocator::commit(void* address, size_t size)
{
	assert(address);
	m_commited += size;
	void * ptr = mmap(address, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_ANON, -1, 0);
	msync(address, size, MS_SYNC | MS_INVALIDATE);
	assert(ptr);
}

void* OsAllocator::reserve_commit(size_t size)
{
	m_reserved += size;
	m_commited += size;
	void * ptr = mmap((void*)0, size, PROT_NONE | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_SHARED | MAP_ANON, -1, 0);
	msync(ptr, size, MS_SYNC | MS_INVALIDATE);
	return ptr;
}

void OsAllocator::decommit(void* address, size_t size)
{
	if (address)
	{
		m_commited -= size;
		mmap(address, size, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
		msync(address, size, MS_SYNC | MS_INVALIDATE);
	}
}

void OsAllocator::free(void* address, size_t size)
{
	if (address)
	{
		m_reserved -= size;
		msync(address, size, MS_SYNC);
		munmap(address, size);
	}
}

#endif

TE_END_TERMINUS_NAMESPACE
