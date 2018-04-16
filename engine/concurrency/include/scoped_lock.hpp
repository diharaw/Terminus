#pragma once

#include "mutex.hpp"

TE_BEGIN_TERMINUS_NAMESPACE

class ScopedLock
{
public:
    ScopedLock(Mutex& mutex);
    ~ScopedLock();
private:
    Mutex& m_mutex;
};

TE_END_TERMINUS_NAMESPACE

