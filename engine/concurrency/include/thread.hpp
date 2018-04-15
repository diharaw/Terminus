#pragma once

#include <functional>
#include <concurrency/include/thread_common.hpp>
#include <core/include/terminus_macros.hpp>

#if defined(_WIN32)
#else
#include <pthread.h>
#endif

TE_BEGIN_TERMINUS_NAMESPACE

class Thread
{
public:
    Thread();
    ~Thread();
    int run(std::function<void(void*)> func, void* args);
    bool join();
    
private:
#if defined(_WIN32)
    static void thread_proc(void* data);
#else
    static void* thread_proc(void* data);
#endif
    void thread_run();
    
private:
#if defined(_WIN32)
    int      m_thread_handle;
    DWORD    m_dw_thread_id;
    unsigned m_stack_size;
    HANDLE   m_thread_id;
#else
    pthread_t m_thread;
#endif
    void*     m_args;
    std::function<void(void*)> m_run_function;
};

TE_END_TERMINUS_NAMESPACE