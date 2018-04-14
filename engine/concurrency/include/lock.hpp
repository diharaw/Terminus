#pragma once

#include "mutex.hpp"

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
