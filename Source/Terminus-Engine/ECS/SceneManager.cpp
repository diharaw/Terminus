#include "SceneManager.h"
#include <Core/context.h>

namespace terminus
{
	SceneManager::SceneManager()
	{
		m_thread_pool.Initialize(1);
	}

	SceneManager::~SceneManager()
	{

	}
    
	void SceneManager::Initialize()
	{
		
        
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

		SceneLoadData* data = new SceneLoadData();
		data->scene_name = scene;
        
        task->function.Bind<SceneManager, &SceneManager::ScenePreloadTask>(this);
        task->data = data;

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
		Scene* loaded = context::get_scene_cache().Load(load_data->scene_name);
        
        // Fire Scene Load Complete Event
        
        SceneLoadEvent* event = new SceneLoadEvent(loaded);
        EventHandler::QueueEvent(event);
	}

	void SceneManager::ScenePreloadTask(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = context::get_scene_cache().Load(load_data->scene_name);
        
        // Fire Scene Preload Complete Event
        
        ScenePreloadEvent* event = new ScenePreloadEvent(loaded);
        EventHandler::QueueEvent(event);
	}

	void SceneManager::RegisterSystem(ISystem* system)
	{

	}

	void SceneManager::InitializeScene(Scene* scene)
	{

	}
} // namespace terminus
