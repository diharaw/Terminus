#include <concurrency/include/thread.hpp>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <process.h>
#endif

TE_BEGIN_TERMINUS_NAMESPACE

#if defined(TERMINUS_PLATFORM_WIN32)

Thread::Thread()
{
	m_stack_size = 8192;
}

Thread::~Thread()
{
    
}

int Thread::run(std::function<void(void*)> func, void* args)
{
    m_run_function = func;
    m_args = args;
    
    m_thread_handle = _beginthread( thread_proc, m_stack_size, static_cast<void*>(this));
    
    if (m_thread_handle == -1)
        return -1;
    
    m_thread_id = (HANDLE) m_thread_handle;

    return 0;
}

bool Thread::join()
{
    unsigned long rc = (unsigned long) WaitForSingleObject(m_thread_id, INFINITE);
    return (rc == 0);
}

void Thread::thread_proc(void* data)
{
    Thread* thread = static_cast<Thread*>(data);
    thread->thread_run();
    _endthread();
}

void Thread::thread_run()
{
    m_run_function(m_args);
}

#else

Thread::Thread()
{
    
}

Thread::~Thread()
{
    
}

int Thread::run(std::function<void(void*)> func, void* args)
{
    m_run_function = func;
    m_args = args;
    int err = ::pthread_create(&m_thread, 0, thread_proc, static_cast<void*>(this));
    return err;
}

bool Thread::join()
{
    int nerr = pthread_join(m_thread, 0);
    return (nerr == 0);
}

void* Thread::thread_proc(void* data)
{
    Thread* thread = static_cast<Thread*>(data);
    thread->thread_run();
    pthread_exit(NULL);
    return 0;
}

void Thread::thread_run()
{
    m_run_function(m_args);
}

#endif

TE_END_TERMINUS_NAMESPACE