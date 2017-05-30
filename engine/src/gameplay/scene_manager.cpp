#include <gameplay/scene_manager.h>
#include <gameplay/scene.h>
#include <core/context.h>
#include <memory/pool_allocator.h>

TERMINUS_BEGIN_NAMESPACE

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

	callback.Bind<SceneManager, &SceneManager::on_scene_preload_complete>(this);
	EventHandler::register_listener(ScenePreloadEvent::sk_Type, callback);
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
    for(uint32_t i = 0; i < m_active_scenes.size(); i++)
		m_active_scenes[i]->simulate(pkt, dt);
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

void SceneManager::activate_scene(StringBuffer32 name)
{
	Scene* scene = nullptr;

	if (m_scene_map.get(HASH(name.c_str()), scene))
	{
		ID scene_id = m_active_scenes.add();
		scene->m_scene_id = scene_id;
		m_active_scenes.set(scene_id, scene);
	}
}

void SceneManager::activate_scene(Scene* scene)
{
	ID scene_id = m_active_scenes.add();
	scene->m_scene_id = scene_id;
	m_active_scenes.set(scene_id, scene);
}

void SceneManager::deactivate_scene(StringBuffer32 name)
{
	Scene* scene = nullptr;

	if (m_scene_map.get(HASH(name.c_str()), scene))
		m_active_scenes.remove(scene->m_scene_id);
}

void SceneManager::deactivate_scene(Scene* scene)
{
	if(scene)
		m_active_scenes.remove(scene->m_scene_id);
}

void SceneManager::unload(StringBuffer32 scene)
{
	Scene* existing_scene = nullptr;

	if (m_scene_map.get(HASH(scene.c_str()), existing_scene))
	{
		Task task;
		LoadingThread& loading_thread = global::get_context()._loading_thread;

		SceneUnloadData* data = task_data<SceneUnloadData>(task);
		data->_scene = existing_scene;
		task._function.Bind<&scene_unload_task>();
		loading_thread.enqueue_load_task(task);

		m_active_scenes.remove(existing_scene->m_scene_id);
		m_scene_map.remove(HASH(scene.c_str()));
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

	if (scene)
	{
		m_scene_map.set(HASH(scene->m_name.c_str()), scene);
		activate_scene(scene);
	}
}

void SceneManager::on_scene_preload_complete(Event* event)
{
	ScenePreloadEvent* event_data = (ScenePreloadEvent*)event;
	Scene* scene = event_data->GetScene();

	if (scene)
		m_scene_map.set(HASH(scene->m_name.c_str()), scene);
}

TERMINUS_END_NAMESPACE
