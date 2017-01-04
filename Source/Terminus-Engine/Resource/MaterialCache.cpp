#include "MaterialCache.h"
#include "../IO/FileSystem.h"
#include <iostream>
#include <rapidjson.h>

namespace terminus { namespace Resource {

	MaterialCache::MaterialCache()
	{

	}

	MaterialCache::~MaterialCache()
	{

	}

	void MaterialCache::Initialize(Graphics::RenderDevice* device, TextureCache* textureCache)
	{
		m_device = device;
		m_TextureCache = textureCache;
	}

	Graphics::Material* MaterialCache::Load(String key)
	{
		if (m_MaterialMap.find(key) == m_MaterialMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
			std::string extension = FileSystem::get_file_extention(key);

			if (m_LoaderMap.find(extension) == m_LoaderMap.end())
			{
				return nullptr;
			}
			else
			{
				AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(key));

				Graphics::Material* material = m_Factory.Create(data, m_TextureCache);
				m_MaterialMap[key] = material;
				return material;
			}
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return m_MaterialMap[key];
		}
	}

	void MaterialCache::Unload(Graphics::Material* material)
	{

	}

} }
