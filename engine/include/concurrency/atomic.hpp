#pragma once

#include <core/terminus_macros.hpp>
#include <stdint.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else

#endif

TE_BEGIN_TERMINUS_NAMESPACE

namespace atomic
{
#if defined(TERMINUS_PLATFORM_WIN32)
	inline int32_t increment(int32_t* ptr) { return InterlockedIncrement((long*)ptr); }
	inline int64_t increment64(int64_t* ptr) { return InterlockedIncrement64(ptr); }
	inline int32_t decrement(int32_t* ptr) { return InterlockedDecrement((long*)ptr); }
	inline int64_t decrement64(int64_t* ptr) { return InterlockedDecrement64(ptr); }
	inline int32_t add(int32_t* ptr, int32_t value) { return InterlockedAdd((long*)ptr, (long)value); }
	inline int64_t add64(int64_t* ptr, int64_t value) { return InterlockedAdd64(ptr, value); }
	inline int32_t sub(int32_t* ptr, int32_t value) { return InterlockedAdd((long*)ptr, (long)-value); }
	inline int64_t sub64(int64_t* ptr, int64_t value) { return InterlockedAdd64(ptr, -value); }
	inline int32_t compare_exchange(int32_t* ptr, int32_t old_val, int32_t new_val) { return InterlockedCompareExchange((long*)ptr, new_val, old_val); }
	inline int32_t compare_exchange64(int64_t* ptr, int64_t old_val, int64_t new_val) { return InterlockedCompareExchange64(ptr, new_val, old_val); }
#else

#endif
}

TE_END_TERMINUS_NAMESPACE