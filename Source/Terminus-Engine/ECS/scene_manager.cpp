#include <ECS/scene_manager.h>
#include <Core/context.h>

namespace terminus
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{

	}
    
	void SceneManager::Initialize()
	{
		
        
	}

	void SceneManager::Load(String scene)
	{
        Task task;

        ResourceThreadPool* thread_pool = Global::GetResourceThreadPool();
        
		task._function.Bind<SceneManager, &SceneManager::SceneLoadTask>(this);
        SceneLoadData* data = task_data<SceneLoadData>(task);
		data->scene_name = scene;
		
		thread_pool->enqueue(task);
	}

	void SceneManager::Preload(String scene)
	{
        Task task;
        
        ResourceThreadPool* thread_pool = Global::GetResourceThreadPool();

        SceneLoadData* data = task_data<SceneLoadData>(task);
		data->scene_name = scene;
        
        task._function.Bind<SceneManager, &SceneManager::ScenePreloadTask>(this);

		thread_pool->enqueue(task);
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
