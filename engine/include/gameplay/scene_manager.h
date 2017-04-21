#pragma once

#ifndef  SCENEMANAGER_H
#define SCENEMANAGER_H

#include <gameplay/scene.h>
#include <core/global.h>
#include <core/types.h>
#include <resource/scene_cache.h>
#include <core/Event/event_handler.h>

#include <vector>

namespace terminus
{
    using SceneMap = std::unordered_map<String, Scene*>;
    using SceneVector = std::vector<Scene*>;

	class SceneManager
	{
	private:
		SceneMap 	_scene_map;
        SceneVector _active_scenes;
        SceneVector _queued_scenes;

	public:
		SceneManager();
		~SceneManager();
        void initialize();
        void update(double dt);
		void load(String scene);
		void preload(String scene);
		void set_active_scene(String scene);
		void unload(String scene);
        SceneVector& active_scenes();

	private:
		void initialize_scene(Scene* scene);
        EVENT_METHOD_DECLARATION(on_scene_load_complete);
        EVENT_METHOD_DECLARATION(on_scene_preload_complete);
	};

}

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


#endif
