#pragma once

#include <types.h>
#include <ECS/entity.h>
#include <ECS/scene.h>

#include <vector>
#include <unordered_map>

namespace terminus
{
	struct Project
	{
		JsonDocument doc;

		Project();
		~Project();
		void create(const char* file_name, const char* path);
		void load(const char* file_name);
		void save();
	};
}