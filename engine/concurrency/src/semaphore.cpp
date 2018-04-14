#include <concurrency/include/semaphore.hpp>

#if defined(_WIN32)

Semaphore::Semaphore()
{
    m_semaphore = CreateSemaphore(NULL, 0, 256000, NULL);
}

Semaphore::~Semaphore()
{
    CloseHandle(m_semaphore);
}

void Semaphore::signal()
{
    ReleaseSemaphore(m_semaphore, 1, NULL);
}

void Semaphore::wait()
{
    WaitForSingleObject(m_semaphore, INFINITE);
}

#else

Semaphore::Semaphore()
{
    assert(pthread_mutex_init(&m_lock, NULL) == 0);
    assert(pthread_cond_init(&m_cond, NULL) == 0);
}

Semaphore::~Semaphore()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_lock);
}

void Semaphore::signal()
{
    pthread_cond_signal(&m_cond);
}

void Semaphore::wait()
{
    pthread_cond_wait(&m_cond, &m_lock);
}

#endif
