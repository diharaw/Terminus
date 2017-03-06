#pragma once

#include <container/deque.h>

template<typename T, size_t N>
class HashMap
{
public:
	struct HashEntry
	{
		uint64_t key;
		uint32_t next;
		T		 value;
	};

	struct FindResult
	{
		uint32_t hash_index;
		uint32_t data_prev_index;
		uint32_t data_index;
	};

	uint32_t		   _hash[N];
	Deque<uint32_t, N> _free_indices;
	HashEntry		   _data[N];
	uint32_t		   _num_objects;
	const uint32_t INVALID_INDEX = 0xffffffffu;

public:
	HashMap()
	{
		for (uint32_t i = 0; i< N; ++i)
		{
			_hash[i] = INVALID_INDEX;
			_data[i].next = INVALID_INDEX;
			_free_indices.push_back(i);
		}

		_num_objects = 0;
	}

	~HashMap()
	{

	}

	void set(uint64_t key, T value)
	{
		uint32_t data_index = find_or_make(key);
		_data[data_index].value = value;
	}

	bool has(uint64_t key)
	{
		uint32_t data_index = find_or_fail(key);
		return data_index != INVALID_INDEX;
	}

	bool get(uint64_t key, T& object)
	{
		uint32_t data_index = find_or_fail(key);

		if (data_index == INVALID_INDEX)
			return false;
		else
		{
			object = _data[data_index].value;
			return true;
		}
	}

	T* get_ref(uint64_t key)
	{
		const uint32_t i = find_or_fail(key);
		return i == INVALID_INDEX ? nullptr : &_data[i].value;
	}

	void remove(uint64_t key)
	{
		FindResult result = find(key);

		// check if key actually exists
		if (result.data_index != INVALID_INDEX)
			erase(result);
	}

	uint32_t size()
	{
		return _num_objects;
	}

private:
	FindResult find(uint64_t key)
	{
		FindResult result;

		result.hash_index = INVALID_INDEX;
		result.data_prev_index = INVALID_INDEX;
		result.data_index = INVALID_INDEX;

		result.hash_index = key % N;
		result.data_index = _hash[result.hash_index];

		while (result.data_index != INVALID_INDEX)
		{
			if (_data[result.data_index].key == key)
				return result;

			result.data_prev_index = result.data_index;
			result.data_index = _data[result.data_index].next;
		}

		return result;
	}

	// tries to find an object. if not found returns INVALID_INDEX.
	uint32_t find_or_fail(uint64_t key)
	{
		FindResult result = find(key);
		return result.data_index;
	}

	// tries to find an object. if not found creates Hash Entry.
	uint32_t find_or_make(uint64_t key)
	{
		FindResult result = find(key);

		if (result.data_index == INVALID_INDEX)
		{
			result.data_index = _free_indices.pop_front();
			_data[result.data_index].next = INVALID_INDEX;
			_data[result.data_index].key = key;
			_num_objects++;

			if (result.data_prev_index != INVALID_INDEX)
				_data[result.data_prev_index].next = result.data_index;

			if (_hash[result.hash_index] == INVALID_INDEX)
				_hash[result.hash_index] = result.data_index;
		}

		return result.data_index;
	}

	void erase(FindResult& result)
	{
		_free_indices.push_front(result.data_index);
		_num_objects--;

		if (result.data_prev_index == INVALID_INDEX)
			_hash[result.hash_index] = INVALID_INDEX;
		else
			_data[result.data_prev_index].next = _data[result.data_index].next;
	}
};
