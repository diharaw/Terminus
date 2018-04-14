#include <concurrency/include/scoped_lock.hpp>

ScopedLock::ScopedLock(Mutex& mutex) : m_mutex(mutex)
{
    m_mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_mutex.unlock();
}
