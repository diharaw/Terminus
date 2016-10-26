#include "TextureCache.h"
#include "../IO/FileSystem.h"
#include "../Types.h"
#include <iostream>

TextureCache::TextureCache()
{
	
}

TextureCache::~TextureCache()
{
	for (auto it : m_LoaderList)
	{
		T_SAFE_DELETE(it);
	}
}

Texture2D* TextureCache::Load(std::string _ID)
{
	if (m_AssetMap.find(_ID) == m_AssetMap.end())
	{
		std::cout << "Asset not in Cache. Loading Asset." << std::endl;

		std::string extension = FileSystem::GetFileExtention(_ID);

		if (m_LoaderMap.find(extension) == m_LoaderMap.end())
		{
			return nullptr;
		}
		else
		{
			asset_common::ImageLoadData* data = static_cast<asset_common::ImageLoadData*>(m_LoaderMap[extension]->Load(_ID));

			Texture2D* texture = m_Factory.Create(data);
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

void TextureCache::Unload(Texture2D* _Handle)
{

}