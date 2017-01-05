#include "SceneCache.h"
#include "../IO/FileSystem.h"

namespace terminus
{
	SceneCache::SceneCache()
	{

	}

	SceneCache::~SceneCache()
	{

	}
    
    void SceneCache::Initialize()
    {

    }

	Scene* SceneCache::Load(String _ID)
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
				AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(_ID));
				Scene* scene = m_Factory.Create(data);
				m_AssetMap[_ID] = scene;
				return scene;
			}
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return m_AssetMap[_ID];
		}
	}

	void SceneCache::Unload(Scene* scene)
	{

	}
} // namespace terminus
