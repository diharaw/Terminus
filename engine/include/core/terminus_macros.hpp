#pragma once

// Terminus Namespace 
#define TE_BEGIN_TERMINUS_NAMESPACE namespace te {
#define TE_END_TERMINUS_NAMESPACE }

// GFX Namespace
#define TE_BEGIN_GFX_NAMESPACE namespace gfx {
#define TE_END_GFX_NAMESPACE }

// Bitflag Macros
#define TE_HAS_BIT_FLAG(FLAG, VARIABLE) (VARIABLE & FLAG) == FLAG

// OpenGL Error Checking
#define TERMINUS_ENABLE_ERROR_CHECK

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

// Compiler Type
#if defined(__clang__)
    #define TERMINUS_COMPILER_CLANG
#elif defined(__GNUC__)
    #define TERMINUS_COMPILER_GCC
#elif defined(_MSC_VER)
    #define TERMINUS_COMPILER_MSVC
#endif

// Export/Import
#if defined(TERMINUS_COMPILER_MSVC)
    //  Microsoft
    #if defined(TERMINUS_DLL_EXPORT)
        #define TE_API __declspec(dllexport)
    #elif defined(TERMINUS_DLL_IMPORT)
        #define TE_API __declspec(dllimport)
    #else
        #define TE_API
    #endif
#elif defined(TERMINUS_COMPILER_GCC) || defined(TERMINUS_COMPILER_CLANG)
    //  GCC
    #if defined(TERMINUS_DLL_EXPORT)
        #define TE_API __attribute__((visibility("default")))
    #elif defined(TERMINUS_DLL_IMPORT)
        #define TE_API __attribute__((visibility("default")))
    #else
        #define TE_API
    #endif
#define TERMINUS_API_IMPORT
#else
    //  do nothing and hope for the best?
    #define TE_API
    #pragma warning Unknown dynamic link import/export semantics.
#endif


#define TE_DECLARE_APP_MAIN(CLASS)				\
extern "C" int SDL_main(int argc, char *argv[]) \
{												\
	CLASS app;									\
	return app.run(argc, argv);					\
}
