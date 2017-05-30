#pragma once

#include <resource/scene_factory.h>

TERMINUS_BEGIN_NAMESPACE

class SceneCache
{
private:
	std::unordered_map<String, Scene*> m_AssetMap;

public:
	SceneCache();
	~SceneCache();
    void initialize();
	Scene* load(String _ID);
	void unload(Scene* scene);
};

TERMINUS_END_NAMESPACE
