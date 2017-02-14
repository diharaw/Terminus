#pragma once

#include <editor/project.h>
#include <Core/Event/event.h>
#include <editor/ieditor_window.h>
#include <unordered_map>
#include <vector>

namespace terminus
{
    class Scene;
    
    enum EditorWindowType
    {
        PROJECT_SELECTION = 0,
        PROJECT_WIZARD,
        MAIN_DOCK,
        ANIMATION_EDITOR,
        RENDERING_PATH_EDITOR
    };
    
	class Editor
	{
	private:
		Project* _current_project;
		std::unordered_map<uint32_t, IEditorWindow*> _window_map;
        Scene* _current_scene;
        int _listener_id;
        
	public:
		Editor();
		~Editor();
        void initialize();
		void update(double dt);
		void create_project(const char* name, const char* path);
		void load_project(const char* name);
		void register_window(uint32_t id, IEditorWindow* window);
        void open_window(uint32_t id);
        Scene* get_current_scene();
		void on_scene_load_complete(Event* event);
	};
}
