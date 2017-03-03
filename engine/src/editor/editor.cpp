#include <editor/editor.h>
#include <editor/project_selection_window.h>
#include <editor/project_wizard_window.h>
#include <editor/main_dock_window.h>
#include <ECS/scene_manager.h>
#include <Input/input_handler.h>
#include <string>

namespace terminus
{
	Editor::Editor()
	{
		_current_project = nullptr;
        _current_scene = nullptr;
        _state = EditorState::EDIT;
	}

	Editor::~Editor()
	{
        
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
        
        //input_handler::load_input_map(input_handler::get_default_player_context(), "editor_input_map");
    }
    
    void Editor::shutdown()
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
    
	void Editor::update(double dt)
	{
        update_camera(dt);
        
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
    
    void Editor::set_state(EditorState state)
    {
        _state = state;
    }
    
    void Editor::update_camera(double dt)
    {
        if(_state == EditorState::EDIT)
        {
            
        }
    }
    
    void Editor::on_input_action(Event* event)
    {
        if(_state == EditorState::EDIT)
        {
            
        }
    }
    
    void Editor::on_input_state(Event* event)
    {
        if(_state == EditorState::EDIT)
        {
            
        }
    }
    
    void Editor::on_input_axis(Event* event)
    {
        if(_state == EditorState::EDIT)
        {
            
        }
    }
}
