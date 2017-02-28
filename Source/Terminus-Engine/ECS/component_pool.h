#pragma once

#include <container/packed_array.h>
#include <ECS/entity.h>

#include <iostream>
#include <array>
#include <cassert>

namespace terminus
{
	template <typename T>
	class ComponentPool
	{
	public:
		PackedArray<T, MAX_ENTITIES> _pool;
	private:
		std::array<ID, MAX_ENTITIES> _cmp_entity_ref;

	public:
		ComponentPool()
		{
			for (ID& entity_id : _cmp_entity_ref)
			{
				entity_id = INVALID_ID;
			}
		}

		~ComponentPool()
		{

		}

		inline T& create(Entity& entity)
		{
			// currently only one instance of a component can be attached to an entity. if already attached, return that instance.

			ID cmp_id = 0;

			if (has(entity))
			{
				cmp_id = _cmp_entity_ref[INDEX_FROM_ID(entity._id)];
			}
			else
			{
				cmp_id = _pool.add();
				_cmp_entity_ref[INDEX_FROM_ID(entity._id)] = cmp_id;
			}

			return _pool.lookup(cmp_id);
		}

		inline T& lookup(Entity& entity)
		{
			assert(has(entity));

			return _pool.lookup(_cmp_entity_ref[INDEX_FROM_ID(entity._id)]);
		}
        
        inline ID get_id(Entity& entity)
        {
            return _cmp_entity_ref[INDEX_FROM_ID(entity._id)];
        }

		inline void remove(Entity& entity)
		{
			if (has(entity))
			{
				ID cmp_id = _cmp_entity_ref[INDEX_FROM_ID(entity._id)];
				_pool.remove(cmp_id);
				_cmp_entity_ref[INDEX_FROM_ID(entity._id)] = INVALID_ID;
			}
		}

		inline bool has(Entity& entity)
		{
			return (_cmp_entity_ref[INDEX_FROM_ID(entity._id)] != INVALID_ID);
		}

		inline T* get_array()
		{
			return &_pool._objects[0];
		}

		inline int get_num_objects()
		{
			return _pool._num_objects;
		}
	};
}

