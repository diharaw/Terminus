#pragma once

#ifndef  SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "../Global.h"
#include "../Types.h"
#include "../Resource/SceneCache.h"
#include "../Core/Event/EventHandler.h"

namespace terminus
{
    using SceneMap = std::unordered_map<String, Scene*>;

	struct SceneLoadData
	{
		String scene_name;
	};

	class SceneManager
	{
	private:
		SceneMap 						 m_scene_map;
		Scene* 							 m_active_scene;
		Scene* 							 m_preload_scene;

	public:
		SceneManager();
		~SceneManager();
        void Initialize();
		void Load(String scene);
		void Preload(String scene);
		void SetActiveScene(String scene);
		void Unload(String scene);
		void SceneLoadTask(void* data);
		void ScenePreloadTask(void* data);
		void RegisterSystem(ISystem* system);

	public:
		void InitializeScene(Scene* scene);
	};

}

class SceneLoadEvent : public Event
{
public:
	static const EventType sk_Type;
	terminus::Scene* m_scene;

public:
	SceneLoadEvent(terminus::Scene* scene) : m_scene(scene) {}
	virtual ~SceneLoadEvent() {}
    inline virtual std::string GetName()             { return "Scene Load Event"; }
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual terminus::Scene* GetScene() { return m_scene; }
};

class ScenePreloadEvent : public Event
{
public:
    static const EventType sk_Type;
    terminus::Scene* m_scene;
    
public:
    ScenePreloadEvent(terminus::Scene* scene) : m_scene(scene) {}
    virtual ~ScenePreloadEvent() {}
    inline virtual std::string GetName()             { return "Scene Preload Event"; }
    inline virtual EventType GetType() { return sk_Type; }
    inline virtual terminus::Scene* GetScene() { return m_scene; }
};


#endif
