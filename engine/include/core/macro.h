#pragma once

// Compiler feature abstraction

#if defined(__GCC__) || defined(__clang__)
    //#define T_FORCE_INLINE  __attribute__((always_inline))
    #define TE_FORCE_INLINE  inline
#elif defined(_MSC_VER)
    #define TE_FORCE_INLINE __forceinline
#endif

#define TE_SAFE_DELETE(x) if(x){ delete x; x = nullptr; }
#define TE_SAFE_DELETE_ARRAY(x) if(x) { delete[] x; x = nullptr; }
#define TE_HANDLE_VALID(x) x != USHRT_MAX

#if defined(_MSC_VER)
#define TE_ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__) || defined(__clang__)
#define TE_ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

// Dynamic library export

#if defined(_MSC_VER)
    //  Microsoft
    #define TERMINUS_API __declspec(dllexport)
    #define TERMINUS_API_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
    //  GCC
    #define TERMINUS_API __attribute__((visibility("default")))
    #define TERMINUS_API_IMPORT
#else
    //  do nothing and hope for the best?
    #define TERMINUS_API
    #define TERMINUS_API_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define TERMINUS_BEGIN_NAMESPACE namespace terminus {
#define TERMINUS_END_NAMESPACE }

#define DECLARE_FACTORY_FUNC(class_name, base_class)			 \
extern "C" TERMINUS_API base_class* __cdecl Create##class_name() \
{																 \
return new class_name();                                         \
}
