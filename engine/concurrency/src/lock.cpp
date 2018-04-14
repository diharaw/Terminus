#include <concurrency/include/lock.hpp>

Lock::Lock(Mutex& mutex) : m_mutex(mutex)
{
    
}

Lock::~Lock()
{
    
}

void Lock::lock()
{
    m_mutex.lock();
}

void Lock::unlock()
{
    m_mutex.unlock();
}
