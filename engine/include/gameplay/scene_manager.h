#pragma once

#include <core/global.h>
#include <core/types.h>
#include <resource/scene_cache.h>
#include <core/Event/event_handler.h>
#include <container/packed_array.h>
#include <container/hash_map.h>

#define MAX_SCENES 10

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;
class  Scene;
class  PoolAllocator;

class SceneManager
{
private:
	HashMap<Scene*, MAX_SCENES>		m_scene_map;
	PackedArray<Scene*, MAX_SCENES> m_active_scenes;
	PoolAllocator*					m_allocator;

public:
	SceneManager();
	~SceneManager();
    void initialize();
    void simulate(FramePacket* pkt, double dt);
	void load(StringBuffer32 scene);
	void preload(StringBuffer32 scene);
	void activate_scene(StringBuffer32 name);
	void activate_scene(Scene* scene);
	void deactivate_scene(StringBuffer32 name);
	void deactivate_scene(Scene* scene);
	void unload(StringBuffer32 scene);

private:
	void initialize_scene(Scene* scene);
	Scene* allocate();
	void deallocate(Scene* scene);
    void on_scene_load_complete(Event* event);
    void on_scene_preload_complete(Event* event);
};

TERMINUS_END_NAMESPACE

struct SceneLoadEvent : public Event
{
	static const EventType sk_Type;
	terminus::Scene* m_scene;

	SceneLoadEvent(terminus::Scene* scene) : m_scene(scene) {}
	virtual ~SceneLoadEvent() {}
    inline virtual std::string GetName()             { return "Scene Load Event"; }
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual terminus::Scene* GetScene() { return m_scene; }
};

struct ScenePreloadEvent : public Event
{
    static const EventType sk_Type;
    terminus::Scene* m_scene;
    
    ScenePreloadEvent(terminus::Scene* scene) : m_scene(scene) {}
    virtual ~ScenePreloadEvent() {}
    inline virtual std::string GetName()             { return "Scene Preload Event"; }
    inline virtual EventType GetType() { return sk_Type; }
    inline virtual terminus::Scene* GetScene() { return m_scene; }
};