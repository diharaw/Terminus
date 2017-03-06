#pragma once

#include <resource/material_factory.h>
#include <resource/texture_cache.h>
#include <core/types.h>

#include <unordered_map>

namespace terminus
{
	class MaterialCache
	{
	private:
		std::unordered_map<std::string, Material*> _material_map;

	public:
		MaterialCache();
		~MaterialCache();
		Material* load(String key);
		void unload(Material* material);
	};
}
