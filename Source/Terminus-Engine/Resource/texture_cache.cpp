#include <Resource/texture_cache.h>
#include <Resource/stb_image_loader.h>
#include <IO/filesystem.h>
#include <types.h>

#include <iostream>

namespace terminus
{
	TextureCache::TextureCache()
	{
        filesystem::add_directory("assets/texture");
	}

	TextureCache::~TextureCache()
	{

	}

	Texture* TextureCache::load(std::string id)
	{
		if (m_AssetMap.find(id) == m_AssetMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;

            asset_common::ImageLoadData* data = stb_image_loader::load(id);

            Texture* texture = texture_factory::create(data);
            m_AssetMap[id] = texture;

            std::cout << "Asset successfully loaded" << std::endl;

            return texture;
        }
		else
		{
			std::cout << "Asset already loaded. Returning reference." << std::endl;
			return m_AssetMap[id];
		}
	}

	void TextureCache::unload(Texture* texture)
	{

	}
}
