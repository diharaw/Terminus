#include <gameplay/scene_manager.h>
#include <gameplay/scene.h>
#include <core/context.h>
#include <memory/pool_allocator.h>

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
        EventHandler::queue_event(event);
    }
    
    void scene_preload_task(void* data)
    {
        SceneLoadData* load_data = (SceneLoadData*)data;
        Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));
        
        // Fire Scene Preload Complete Event
        
        ScenePreloadEvent* event = new ScenePreloadEvent(loaded);
        EventHandler::queue_event(event);
    }
    
    void scene_unload_task(void* data)
    {
        SceneUnloadData* task_data = (SceneUnloadData*)data;
        task_data->_scene->shutdown();
        
        TE_SAFE_DELETE(task_data->_scene);
    }
    
	SceneManager::SceneManager()
	{
        EventCallback callback;
        callback.Bind<SceneManager, &SceneManager::on_scene_load_complete>(this);
        EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);
	}

	SceneManager::~SceneManager()
	{

	}
    
	void SceneManager::initialize()
	{
		void* memory = global::get_default_allocator()->Allocate(sizeof(Scene) * MAX_SCENES, 8);
		m_allocator = TE_NEW(global::get_default_allocator()) PoolAllocator(sizeof(Scene), 8, sizeof(Scene) * MAX_SCENES, memory);
	}
    
    void SceneManager::simulate(FramePacket* pkt, double dt)
    {
        for (auto scene : _active_scenes)
        {
            scene->simulate(pkt, dt);
        }
    }

	void SceneManager::load(StringBuffer32 scene)
	{
        Task task;
        
        LoadingThread& loading_thread = global::get_context()._loading_thread;
        
        SceneLoadData* data = task_data<SceneLoadData>(task);
        
        const char* name = scene.c_str();
        strcpy(&data->scene_name[0], name);
        
        task._function.Bind<&scene_load_task>();
        
        loading_thread.enqueue_load_task(task);
	}

	void SceneManager::preload(StringBuffer32 scene)
	{
        Task task;
        
        LoadingThread& loading_thread = global::get_context()._loading_thread;

        SceneLoadData* data = task_data<SceneLoadData>(task);

		const char* name = scene.c_str();
		strcpy(&data->scene_name[0], name);

        task._function.Bind<&scene_preload_task>();

        loading_thread.enqueue_load_task(task);
	}
    
	void SceneManager::set_active_scene(StringBuffer32 scene)
	{
		
	}

	void SceneManager::unload(StringBuffer32 scene)
	{
        for(int i = 0; i < _active_scenes.size(); i++)
        {
            String scene_str = String(_active_scenes[i]->_name.c_str());
            
            if(scene_str == scene)
            {
                // is an active scene
                // TODO: more graceful scene unload
                Task task;
                LoadingThread& loading_thread = global::get_context()._loading_thread;
                
                SceneUnloadData* data = task_data<SceneUnloadData>(task);
                data->_scene = _active_scenes[i];
                task._function.Bind<&scene_unload_task>();
                loading_thread.enqueue_load_task(task);
                
                _active_scenes.erase(_active_scenes.begin() + i);
                
                break;
            }
        }
	}

	Scene* SceneManager::allocate()
	{
		return TE_NEW(m_allocator) Scene();
	}

	void SceneManager::deallocate(Scene* scene)
	{
		if (scene)
		{
			TE_DELETE(scene, m_allocator);
		}
	}

	void SceneManager::initialize_scene(Scene* scene)
	{

	}
    
    void SceneManager::on_scene_load_complete(Event* event)
    {
        SceneLoadEvent* event_data = (SceneLoadEvent*)event;

		Scene* scene = event_data->GetScene();
        
    }
    

	void SceneManager::on_scene_preload_complete(Event* event)
    {
        
    }
} // namespace terminus
