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

        LoadingThread& loading_thread = Global::get_context()._loading_thread;
        
		task._function.Bind<SceneManager, &SceneManager::SceneLoadTask>(this);
        SceneLoadData* data = task_data<SceneLoadData>(task);
		strcpy(data->scene_name, scene.c_str());
		
		loading_thread.enqueue_load_task(task);
	}

	void SceneManager::Preload(String scene)
	{
        Task task;
        
        LoadingThread& loading_thread = Global::get_context()._loading_thread;

        SceneLoadData* data = task_data<SceneLoadData>(task);

		const char* name = scene.c_str();
		strcpy(&data->scene_name[0], name);

        task._function.Bind<SceneManager, &SceneManager::ScenePreloadTask>(this);

        loading_thread.enqueue_load_task(task);
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
		Scene* loaded = context::get_scene_cache().Load(String(load_data->scene_name));
        
        // Fire Scene Load Complete Event
        
        SceneLoadEvent* event = new SceneLoadEvent(loaded);
        EventHandler::QueueEvent(event);
	}

	void SceneManager::ScenePreloadTask(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = context::get_scene_cache().Load(String(load_data->scene_name));
        
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
