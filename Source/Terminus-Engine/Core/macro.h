#pragma once

// Compiler feature abstraction

#if defined(__GCC__) || defined(__clang__)
    #define T_FORCE_INLINE  __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define T_FORCE_INLINE __forceinline
#endif
