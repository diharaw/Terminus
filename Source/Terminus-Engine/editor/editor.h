#pragma once

#include <editor/project.h>
#include <Core/Event/event.h>
#include <editor/ieditor_window.h>
#include <unordered_map>

namespace terminus
{
	class Editor
	{
	private:
		Project* _current_project;
		std::unordered_map<uint32_t, IEditorWindow*> _window_map;

	public:
		Editor();
		~Editor();
		void update(double dt);
		void create_project(const char* name, const char* path);
		void load_project(const char* name);
		void register_window(uint32_t id, IEditorWindow* window);
		void on_scene_load_complete(Event* event);
	};
}