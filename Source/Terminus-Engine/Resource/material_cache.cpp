#include <Resource/material_cache.h>
#include <IO/filesystem.h>
#include <Resource/text_loader.h>
#include <Core/context.h>
#include <iostream>
#include <rapidjson.h>

namespace terminus
{
	MaterialCache::MaterialCache()
	{
        filesystem::add_directory("assets/material");
	}

	MaterialCache::~MaterialCache()
	{

	}

    Material* MaterialCache::load(String key)
	{
        if(key == "")
            return nullptr;
        
		if (_material_map.find(key) == _material_map.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
            
            Material* material = material_factory::create(key);
            _material_map[key] = material;
            return material;
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return _material_map[key];
		}
	}

	void MaterialCache::unload(Material* material)
	{
        TextureCache& cache = context::get_texture_cache();
        
        for (int i = 0; i < 5; i++)
        {
            if(material->texture_maps[i])
            {
                cache.unload(material->texture_maps[i]);
            }
        }
        
        T_SAFE_DELETE(material);
	}
}
