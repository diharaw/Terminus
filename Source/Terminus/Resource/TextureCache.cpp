#include "TextureCache.h"
#include "../IO/FileSystem.h"
#include "../Types.h"
#include <iostream>

namespace Terminus { namespace Resource {

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

	void TextureCache::Initialize(Graphics::RenderDevice* device)
	{
		m_device = device;
		m_Factory.Initialize(device);
	}

	Graphics::Texture* TextureCache::Load(std::string _ID)
	{
		if (m_AssetMap.find(_ID) == m_AssetMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;

			std::string extension = FileSystem::get_file_extention(_ID);

			if (m_LoaderMap.find(extension) == m_LoaderMap.end())
			{
				return nullptr;
			}
			else
			{
				asset_common::ImageLoadData* data = static_cast<asset_common::ImageLoadData*>(m_LoaderMap[extension]->Load(_ID));

				Graphics::Texture* texture = m_Factory.Create(data);
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

	void TextureCache::Unload(Graphics::Texture* texture)
	{

	}

} }