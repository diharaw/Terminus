#include "SceneManager.h"

namespace Terminus { namespace ECS {

	SceneManager::SceneManager()
	{
		m_thread_pool.Initialize(1);
	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::Initialize(Resource::SceneCache* sceneCache)
	{
		m_scene_cache = sceneCache;
	}

	void SceneManager::Load(String scene)
	{
		TaskData* task = m_thread_pool.CreateTask();

		task->function.Bind<SceneManager, &SceneManager::SceneLoadTask>(this);
		SceneLoadData* data = new SceneLoadData();
		data->scene_name = scene;
		
		m_thread_pool.Submit();
	}

	void SceneManager::Preload(String scene)
	{
		TaskData* task = m_thread_pool.CreateTask();

		task->function.Bind<SceneManager, &SceneManager::ScenePreloadTask>(this);
		SceneLoadData* data = new SceneLoadData();
		data->scene_name = scene;

		m_thread_pool.Submit();
	}

	void SceneManager::SetActiveScene(String scene)
	{
		
	}

	void SceneManager::Unload(String scene)
	{

	}

	void SceneManager::SceneLoadTask(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = m_scene_cache->Load(load_data->scene_name);
	}

	void SceneManager::ScenePreloadTask(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = m_scene_cache->Load(load_data->scene_name);
	}

	void SceneManager::RegisterSystem(ISystem* system)
	{

	}

	void SceneManager::InitializeScene(Scene* scene)
	{

	}

} }
