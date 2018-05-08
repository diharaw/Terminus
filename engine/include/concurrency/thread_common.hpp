#pragma once

#include <core/terminus_macros.hpp>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#if defined(TERMINUS_PLATFORM_WIN32)
    #define TE_THREAD_LOCAL __declspec( thread )
#else
    #define TE_THREAD_LOCAL thread_local
#endif
