#pragma once

#ifndef  SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "../Global.h"
#include "../Types.h"
#include "../Resource/SceneCache.h"
#include "../Core/Event/Event.h"

namespace Terminus { namespace ECS {

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
		Resource::SceneCache*            m_scene_cache;
		ThreadPool						 m_thread_pool;

	public:
		SceneManager();
		~SceneManager();
		void Initialize(Resource::SceneCache* sceneCache);
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

} }

class SceneLoadEvent : public Event
{
private:
	static const EventType sk_Type;
	Terminus::ECS::Scene* m_scene;

public:
	SceneLoadEvent(Terminus::ECS::Scene* scene) : m_scene(scene) {}
	virtual ~SceneLoadEvent() {}
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual Terminus::ECS::Scene* GetScene() { return m_scene; }
};

#endif
