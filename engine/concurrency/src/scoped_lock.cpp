#include <concurrency/include/scoped_lock.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

ScopedLock::ScopedLock(Mutex& mutex) : m_mutex(mutex)
{
    m_mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_mutex.unlock();
}

TE_END_TERMINUS_NAMESPACE