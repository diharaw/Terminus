#pragma once

#include <container/handle.h>
#include <container/deque.h>

template<typename T, size_t N>
class HandleArray
{
private:
	struct Generation
	{
		uint16_t _generation : 15;
		uint16_t _valid : 1;
	};

	T				  _objects[N];
	Generation		  _generations[N];
	Deque<uint16_t,N> _free_indices;
	uint32_t		  _num_objects;

public:
	HandleArray()
	{
		_num_objects = 0;

		for (uint16_t i = 0; i < N; i++)
		{
			_generations[i]._generation = 0;
			_generations[i]._valid = 0;
			_free_indices.push_back(i);
		}
	}

	~HandleArray()
	{

	}

	inline Handle<T> create()
	{
		uint16_t index = _free_indices.pop_front();
		Handle<T> handle;
		handle._index = index;
		handle._generation = _generations[index]._generation++;
		_generations[index]._valid = 1;
		_num_objects++;

		return handle;
	}

	inline Handle<T> create(T object)
	{
		uint16_t index = _free_indices.pop_front();
		_objects[index] = object;
		Handle<T> handle;
		handle._index = index;
		handle._generation = _generations[index]._generation++;
		_generations[index]._valid = 1;
		_num_objects++;

		return handle;
	}

	inline uint32_t size() { return _num_objects; }

	inline bool has(Handle<T>& handle)
	{
		return  (_generations[handle._index]._generation - 1 == handle._generation && _generations[handle._index]._valid == 1);
	}

	inline bool get(Handle<T>& handle, T& object)
	{
		bool result = has(handle);

		if (result)
			object = _objects[handle._index];

		return has(handle);
	}

	inline T* get_ptr(Handle<T>& handle)
	{
		if (has(handle))
			return &_objects[handle._index];
		else
			return nullptr;
	}

	inline void remove(Handle<T>& handle)
	{
		if (_generations[handle._index]._valid == 1)
		{
			_generations[handle._index]._generation++;
			_generations[handle._index]._valid = 0;
			_free_indices.push_front(handle._index);
			_num_objects--;
		}
	}
};
