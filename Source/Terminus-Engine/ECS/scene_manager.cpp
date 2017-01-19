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
    
	void SceneManager::initialize()
	{
		
        
	}
    
    void SceneManager::update(double dt)
    {
        for (auto scene : _active_scenes)
        {
            scene->update(dt);
        }
    }

	void SceneManager::load(String scene)
	{
        Task task;

        LoadingThread& loading_thread = Global::get_context()._loading_thread;
        
		task._function.Bind<SceneManager, &SceneManager::scene_load_task>(this);
        SceneLoadData* data = task_data<SceneLoadData>(task);
		strcpy(data->scene_name, scene.c_str());
		
		loading_thread.enqueue_load_task(task);
	}

	void SceneManager::preload(String scene)
	{
        Task task;
        
        LoadingThread& loading_thread = Global::get_context()._loading_thread;

        SceneLoadData* data = task_data<SceneLoadData>(task);

		const char* name = scene.c_str();
		strcpy(&data->scene_name[0], name);

        task._function.Bind<SceneManager, &SceneManager::scene_preload_task>(this);

        loading_thread.enqueue_load_task(task);
	}

	void SceneManager::set_active_scene(String scene)
	{
		
	}

	void SceneManager::unload(String scene)
	{

	}

	void SceneManager::scene_load_task(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));
        
        // Fire Scene Load Complete Event
        
        SceneLoadEvent* event = new SceneLoadEvent(loaded);
        EventHandler::QueueEvent(event);
	}

	void SceneManager::scene_preload_task(void* data)
	{
		SceneLoadData* load_data = (SceneLoadData*)data;
		Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));
        
        // Fire Scene Preload Complete Event
        
        ScenePreloadEvent* event = new ScenePreloadEvent(loaded);
        EventHandler::QueueEvent(event);
	}
    
	void SceneManager::initialize_scene(Scene* scene)
	{

	}
    
    EVENT_METHOD_DEFINITION(SceneManager, on_scene_load_complete)
    {
        
    }
    
    EVENT_METHOD_DEFINITION(SceneManager, on_scene_preload_complete)
    {
        
    }
} // namespace terminus
