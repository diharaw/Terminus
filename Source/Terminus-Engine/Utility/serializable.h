#pragma once

#include <unordered_map>
#include <stdint.h>

#include <Core/macro.h>

#define SERIALIZE(member, ptr, offset) memcpy(((char*)ptr + offset), &member, sizeof(member)) 
#define DESERIALIZE(member, ptr, offset) memcpy(&member, ((char*)ptr + offset), sizeof(member)) 

namespace terminus
{
	class Serializable
	{
	public:
		Serializable() {}
		virtual ~Serializable() {}
		virtual void serialize(void* mem) = 0;
		virtual void deserialize(void* data) = 0;
		virtual size_t get_size() = 0;
	};
}
