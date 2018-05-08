#pragma once

#include <assert.h>
#include <concurrency/thread_common.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class Semaphore
{
public:
    Semaphore();
    ~Semaphore();
    void signal();
    void wait();
    
private:
#if defined(_WIN32)
    HANDLE m_semaphore;
#else
    pthread_mutex_t m_lock;
    pthread_cond_t  m_cond;
#endif
};

TE_END_TERMINUS_NAMESPACE
