#pragma once

#include <stdint.h>
#include <core/terminus_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

namespace memory
{
	inline void* align_forward(void* address, uint8_t alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & (static_cast<uintptr_t>(~(alignment - 1))));
	}

	inline const void* align_forward(const void* address, uint8_t alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & (static_cast<uintptr_t>(~(alignment - 1))));
	}

	inline void* align_backward(void* address, uint8_t alignment)
	{
		return (void*)(reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline const void* align_backward(const void* address, uint8_t alignment)
	{
		return (void*)(reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline uint8_t align_foward_adjustment(void* address, uint8_t alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline uint8_t align_foward_adjustment_with_header(void* address, uint8_t alignment, uint8_t header_size)
	{
		uint8_t adjustment = align_foward_adjustment(address, alignment);
		uint8_t neededSpace = header_size;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}

	inline uint8_t align_backward_adjustment(void* address, uint8_t alignment)
	{
		uint8_t adjustment = (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(~(alignment - 1)));

		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline void* add(void* pointer, size_t size)
	{
		return (void*)(reinterpret_cast<uintptr_t>(pointer) + size);
	}

	inline const void* add(const void* pointer, size_t size)
	{
		return (const void*)(reinterpret_cast<uintptr_t>(pointer) + size);
	}

	inline void* subtract(void* pointer, size_t size)
	{
		return (void*)(reinterpret_cast<uintptr_t>(pointer) - size);
	}

	inline const void* subtract(const void* pointer, size_t size)
	{
		return (const void*)(reinterpret_cast<uintptr_t>(pointer) - size);
	}
}

TE_END_TERMINUS_NAMESPACE