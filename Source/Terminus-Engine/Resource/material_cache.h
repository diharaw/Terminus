#pragma once

#ifndef MATERIALCACHE_H
#define MATERIALCACHE_H

#include <Resource/material_factory.h>
#include <Resource/texture_cache.h>
#include <types.h>

#include <unordered_map>

namespace terminus
{
	class MaterialCache
	{
	private:
		std::unordered_map<std::string, Material*> m_MaterialMap;

	public:
		MaterialCache();
		~MaterialCache();
		Material* load(String key);
		void unload(Material* material);
	};
}

#endif
