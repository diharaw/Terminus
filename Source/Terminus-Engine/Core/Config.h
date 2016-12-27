#ifndef CONFIG_H
#define CONFIG_H

#define TERMINUS_BUILD 161108

//#define TERMINUS_OPENGL

#define TERMINUS_ENABLE_ERROR_CHECK

#ifdef __APPLE__
    #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_METAL)
        #define TERMINUS_OPENGL
    #endif
#else
    #ifdef WIN32
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_DIRECT3D11)
            #define TERMINUS_DIRECT3D11
        #endif
    #else
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_VULKAN)
            #define TERMINUS_OPENGL
        #endif
    #endif
#endif

#endif

// Terminus Platform

#if defined(__APPLE__)
    #include "TargetConditionals.h"
    #if !TARGET_OS_IPHONE
        #define TERMINUS_PLATFORM_MACOS
    #elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define TERMINUS_PLATFORM_IOS
    #endif
#elif defined(WIN32)
    #define TERMINUS_PLATFORM_WIN32
#else
    #define TERMINUS_PLATFORM_LINUX
#endif

// Build Type

#define TERMINUS_WITH_EDITOR

#define TERMINUS_PROFILING

#if defined(TERMINUS_PROFILING)
    #define TERMINUS_BEGIN_CPU_PROFILE(x) rmt_BeginCPUSample(x, 0);
    #define TERMINUS_END_CPU_PROFILE rmt_EndCPUSample();
    #define TERMINUS_PROFILER_INSTANCE Remotery* rmt;
    #define TERMINUS_CREATE_PROFILER rmt_CreateGlobalInstance(&rmt);
    #define TERMINUS_DESTROY_PROFILER rmt_DestroyGlobalInstance(rmt);
#else
    #define TERMINUS_BEGIN_CPU_PROFILE(x)
    #define TERMINUS_END_CPU_PROFILE
    #define TERMINUS_PROFILER_INSTANCE
    #define TERMINUS_CREATE_PROFILER
    #define TERMINUS_DESTROY_PROFILER
#endif


