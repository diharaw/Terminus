#pragma once

#include <assert.h>
#include "thread_common.hpp"

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
