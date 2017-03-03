#include <Resource/scene_cache.h>
#include <Resource/scene_factory.h>
#include <Resource/text_loader.h>
#include <IO/filesystem.h>

namespace terminus
{
	SceneCache::SceneCache()
	{
        filesystem::add_directory("assets/scene");
	}

	SceneCache::~SceneCache()
	{

	}
    
    void SceneCache::initialize()
    {

    }

	Scene* SceneCache::load(String _ID)
	{
		if (m_AssetMap.find(_ID) == m_AssetMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
			std::string extension = filesystem::get_file_extention(_ID);

            asset_common::TextLoadData* data = text_loader::load(_ID);
            Scene* scene = scene_factory::create(data);
            m_AssetMap[_ID] = scene;
            return scene;
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return m_AssetMap[_ID];
		}
	}

	void SceneCache::unload(Scene* scene)
	{

	}
} // namespace terminus
