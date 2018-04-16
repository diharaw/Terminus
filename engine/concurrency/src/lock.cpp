#include <concurrency/include/lock.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

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

TE_END_TERMINUS_NAMESPACE
