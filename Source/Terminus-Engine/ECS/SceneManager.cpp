#include "SceneManager.h"

namespace terminus
{
	SceneManager::SceneManager()
	{
		m_thread_pool.Initialize(1);
        
        EventCallback callback;
        callback.Bind<SceneManager, &SceneManager::OnSceneLoad>(this);
        EventHandler::RegisterListener(SceneLoadEvent::sk_Type, callback);
	}

	SceneManager::~SceneManager()
	{

	}
    
	void SceneManager::Initialize(SceneCache* sceneCache, RenderDevice* device)
	{
		m_scene_cache = sceneCache;
        m_render_device = device;
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
        
        // Fire Scene Load Complete Event
        
        SceneLoadEvent* event = new SceneLoadEvent(loaded);
        EventHandler::QueueEvent(event);
	}

	void SceneManager::ScenePreloadTask(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = m_scene_cache->Load(load_data->scene_name);
        
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
    
    EVENT_METHOD_DEFINITION(SceneManager, OnSceneLoad)
    {
        SceneLoadEvent* event_data = (SceneLoadEvent*)event;
        InitializeScene(event_data->GetScene());
    }
    
    EVENT_METHOD_DEFINITION(SceneManager, OnScenePreload)
    {
        ScenePreloadEvent* event_data = (ScenePreloadEvent*)event;
        InitializeScene(event_data->GetScene());
    }
}
