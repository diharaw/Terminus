#pragma once

#include <editor/project.h>
#include <Core/Event/event.h>
#include <editor/ieditor_window.h>
#include <editor/editor_scene.h>
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
    
    enum class EditorState
    {
        EDIT = 0,
        PLAY
    };
    
	class Editor
	{
    public:
        CameraComponent _cam_perpective;
        CameraComponent _cam_ortho_x;
        CameraComponent _cam_ortho_y;
        CameraComponent _cam_ortho_z;
        
	private:
		Project* _current_project;
		std::unordered_map<uint32_t, IEditorWindow*> _window_map;
        Scene* _current_scene;
        int _listener_id;
        EditorState _state;
        
	public:
		Editor();
		~Editor();
        void initialize();
        void shutdown();
		void update(double dt);
		void create_project(const char* name, const char* path);
		void load_project(const char* name);
		void register_window(uint32_t id, IEditorWindow* window);
        void open_window(uint32_t id);
        void set_state(EditorState state);
        Scene* get_current_scene();
		void on_scene_load_complete(Event* event);
        void on_input_action(Event* event);
        void on_input_state(Event* event);
        void on_input_axis(Event* event);
        
    private:
        void update_camera(double dt);
	};
}
