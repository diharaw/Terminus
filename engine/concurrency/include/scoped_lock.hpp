#pragma once

#include "mutex.hpp"

class ScopedLock
{
public:
    ScopedLock(Mutex& mutex);
    ~ScopedLock();
private:
    Mutex& m_mutex;
};

