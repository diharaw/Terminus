#pragma once

#include <core/macro.h>
#include <container/hash_map.h>
#include <container/packed_array.h>

TERMINUS_BEGIN_NAMESPACE

template<typename T, size_t N>
struct EntityDataMap
{
	HashMap<ID, N> entites;
	PackedArray<T, N> cache;

	T* add(Entity& entity)
	{
		if (!entites.has(entity.id))
		{
			ID id = cache.add();
			T& entry = cache.get(id);
			entites.set(entity.id, id);
			return &entry;
		}
		else
			return nullptr;
	}

	void remove(Entity& entity)
	{
		ID id;

		if (entites.get(id))
		{
			cache.remove(id);
			entites.remove(entity.id);
		}
	}

	uint32_t size()
	{
		return cache.size();
	}

	T* array()
	{
		return &cache._objects[0];
	}
};

TERMINUS_END_NAMESPACE