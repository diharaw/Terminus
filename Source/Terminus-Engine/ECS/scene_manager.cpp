#include <ECS/scene_manager.h>
#include <Core/context.h>

namespace terminus
{
    struct SceneLoadData
    {
        char scene_name[100];
    };
    
    struct SceneUnloadData
    {
        Scene* _scene;
    };
    
    void scene_load_task(void* data)
    {
        SceneLoadData* load_data = (SceneLoadData*)data;
        Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));
        
        // Fire Scene Load Complete Event
        
        SceneLoadEvent* event = new SceneLoadEvent(loaded);
        EventHandler::QueueEvent(event);
    }
    
    void scene_preload_task(void* data)
    {
        SceneLoadData* load_data = (SceneLoadData*)data;
        Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));
        
        // Fire Scene Preload Complete Event
        
        ScenePreloadEvent* event = new ScenePreloadEvent(loaded);
        EventHandler::QueueEvent(event);
    }
    
    void scene_unload_task(void* data)
    {
        SceneUnloadData* task_data = (SceneUnloadData*)data;
        task_data->_scene->shutdown();
        
        T_SAFE_DELETE(task_data->_scene);
    }
    
	SceneManager::SceneManager()
	{
        EventCallback callback;
        callback.Bind<SceneManager, &SceneManager::on_scene_load_complete>(this);
        EventHandler::RegisterListener(SceneLoadEvent::sk_Type, callback);
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
        
        SceneLoadData* data = task_data<SceneLoadData>(task);
        
        const char* name = scene.c_str();
        strcpy(&data->scene_name[0], name);
        
        task._function.Bind<&scene_load_task>();
        
        loading_thread.enqueue_load_task(task);
	}

	void SceneManager::preload(String scene)
	{
        Task task;
        
        LoadingThread& loading_thread = Global::get_context()._loading_thread;

        SceneLoadData* data = task_data<SceneLoadData>(task);

		const char* name = scene.c_str();
		strcpy(&data->scene_name[0], name);

        task._function.Bind<&scene_preload_task>();

        loading_thread.enqueue_load_task(task);
	}

	void SceneManager::set_active_scene(String scene)
	{
		
	}

	void SceneManager::unload(String scene)
	{
        for(int i = 0; i < _active_scenes.size(); i++)
        {
            if(_active_scenes[i]->_name == scene)
            {
                // is an active scene
                // TODO: more graceful scene unload
                Task task;
                LoadingThread& loading_thread = Global::get_context()._loading_thread;
                
                SceneUnloadData* data = task_data<SceneUnloadData>(task);
                data->_scene = _active_scenes[i];
                task._function.Bind<&scene_unload_task>();
                loading_thread.enqueue_load_task(task);
                
                _active_scenes.erase(_active_scenes.begin() + i);
                
                break;
            }
        }
	}

	void SceneManager::initialize_scene(Scene* scene)
	{

	}
    
    EVENT_METHOD_DEFINITION(SceneManager, on_scene_load_complete)
    {
        SceneLoadEvent* event_data = (SceneLoadEvent*)event;
        _active_scenes.push_back(event_data->GetScene());
    }
    
    EVENT_METHOD_DEFINITION(SceneManager, on_scene_preload_complete)
    {
        
    }
} // namespace terminus
