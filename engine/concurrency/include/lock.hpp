#pragma once

#include "mutex.hpp"

TE_BEGIN_TERMINUS_NAMESPACE

class Lock
{
public:
    Lock(Mutex& mutex);
    ~Lock();
    void lock();
    void unlock();
    
private:
    Mutex& m_mutex;
};

TE_END_TERMINUS_NAMESPACE
