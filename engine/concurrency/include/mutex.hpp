#pragma once

#include <cassert>

#include "thread_common.hpp"

class Mutex
{
public:
    friend class Lock;
    friend class ScopedLock;
    
    Mutex();
    ~Mutex();
private:
    bool lock();
    void unlock();
    
private:
#if defined(_WIN32)
    CRITICAL_SECTION m_critical_section;
#else
    pthread_mutex_t m_lock;
#endif
    
};
