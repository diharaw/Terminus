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
