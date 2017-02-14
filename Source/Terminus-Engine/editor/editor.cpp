#include <editor/editor.h>
#include <editor/project_selection_window.h>
#include <editor/project_wizard_window.h>
#include <editor/main_dock_window.h>
#include <ECS/scene_manager.h>
#include <string>

namespace terminus
{
	Editor::Editor()
	{
		_current_project = nullptr;
        _current_scene = nullptr;
	}

	Editor::~Editor()
	{
        EventHandler::unregister_listener(SceneLoadEvent::sk_Type, _listener_id);
        
        for(auto itr : _window_map)
        {
            if(itr.second)
            {
                T_SAFE_DELETE(itr.second);
            }
        }
	}

    void Editor::initialize()
    {
        EventCallback callback;
        callback.Bind<Editor, &Editor::on_scene_load_complete>(this);
        _listener_id = EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);
        
        register_window(EditorWindowType::PROJECT_SELECTION, new ProjectSelectionWindow("Select Project",
                                                                                        Vector2(0.6f, 0.6f),
                                                                                        Vector2(0.0f, 0.0f),
                                                                                        Vector2(0.5f, 0.5f),
                                                                                        Vector2(0.0f, 0.0f),
                                                                                        0));
        
        register_window(EditorWindowType::PROJECT_WIZARD, new ProjectWizardWindow("Create Project",
                                                                                        Vector2(0.6f, 0.6f),
                                                                                        Vector2(0.0f, 0.0f),
                                                                                        Vector2(0.5f, 0.5f),
                                                                                        Vector2(0.0f, 0.0f),
                                                                                        0));
        
        register_window(EditorWindowType::MAIN_DOCK, new MainDockWindow("Main Dock",
                                                                         Vector2(1.0f, 1.0f),
                                                                         Vector2(0.0f, 0.0f),
                                                                         Vector2(0.0f, 0.0f),
                                                                         Vector2(0.0f, 0.0f),
                                                                         0));
        
        open_window(EditorWindowType::PROJECT_SELECTION);
    }
    
	void Editor::update(double dt)
	{
        for(auto itr : _window_map)
        {
            if(itr.second->is_open())
                itr.second->update(dt);
        }
	}

	void Editor::create_project(const char* name, const char* path)
	{
		_current_project = new Project();
		_current_project->create(name, path);
	}

	void Editor::load_project(const char* name)
	{
		_current_project = new Project();
        _current_project->load(name);
	}
    
    void Editor::open_window(uint32_t id)
    {
        if(_window_map.find(id) != _window_map.end())
        {
            IEditorWindow* wnd = _window_map[id];
            wnd->initialize();
            wnd->open();
        }
    }

	void Editor::register_window(uint32_t id, IEditorWindow* window)
	{
        _window_map[id] = window;
        window->initialize();
	}

    Scene* Editor::get_current_scene()
    {
        return _current_scene;
    }
    
	void Editor::on_scene_load_complete(Event* event)
	{
        SceneLoadEvent* event_data = (SceneLoadEvent*)event;
        _current_scene = event_data->GetScene();
	}
}
