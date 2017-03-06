#pragma once

#include <stdint.h>
#include <climits>
#include <core/types.h>

#include <container/packed_array.h>

#define MAX_ENTITIES 1024
#define INVALID_ID USHRT_MAX

namespace terminus
{
	using EntityKey = uint64_t;

	struct Entity
	{
		ID _id;
		EntityKey      _key;
		int            _version;
		StringBuffer32 _name;
	};
}
