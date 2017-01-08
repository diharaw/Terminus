#include <Resource/texture_cache.h>
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
		for (auto it : m_LoaderList)
		{
			T_SAFE_DELETE(it);
		}
	}

	void TextureCache::Initialize()
	{
	
	}

	Texture* TextureCache::Load(std::string _ID)
	{
		if (m_AssetMap.find(_ID) == m_AssetMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;

			std::string extension = filesystem::get_file_extention(_ID);

			if (m_LoaderMap.find(extension) == m_LoaderMap.end())
			{
				return nullptr;
			}
			else
			{
				AssetCommon::ImageLoadData* data = static_cast<AssetCommon::ImageLoadData*>(m_LoaderMap[extension]->Load(_ID));

				Texture* texture = m_Factory.Create(data);
				m_AssetMap[_ID] = texture;

				free(data->bytes);
				free(data);

				std::cout << "Asset successfully loaded" << std::endl;

				return texture;
			}
		}
		else
		{
			std::cout << "Asset already loaded. Returning reference." << std::endl;
			return m_AssetMap[_ID];
		}
	}

	void TextureCache::Unload(Texture* texture)
	{

	}
}
