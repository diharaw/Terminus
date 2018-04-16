#include <concurrency/include/mutex.hpp>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

#if defined(WIN32)

Mutex::Mutex()
{
    bool status = InitializeCriticalSectionAndSpinCount(&m_critical_section, 0x00000400);
	assert(status);
}

Mutex::~Mutex()
{
    DeleteCriticalSection(&m_critical_section);
}

bool Mutex::lock()
{
    EnterCriticalSection(&m_critical_section);
	return true;
}

void Mutex::unlock()
{
    LeaveCriticalSection(&m_critical_section);
}

#else

Mutex::Mutex()
{
    assert(pthread_mutex_init(&m_lock, NULL) == 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_lock);
}

bool Mutex::lock()
{
    return pthread_mutex_lock(&m_lock);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_lock);
}

#endif

TE_END_TERMINUS_NAMESPACE