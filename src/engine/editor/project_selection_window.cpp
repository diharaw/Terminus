#include <editor/project_selection_window.h>
#include <Core/context.h>

namespace terminus
{
    ProjectSelectionWindow::ProjectSelectionWindow(std::string title,
                                                   Vector2 rel_size,
                                                   Vector2 size,
                                                   Vector2 rel_pos,
                                                   Vector2 pos,
                                                   int flags) : IEditorWindow(title, rel_size, size, rel_pos, pos, flags)
	{
        _project_path = "";
        
        EventCallback callback;
        callback.Bind<ProjectSelectionWindow, &ProjectSelectionWindow::on_scene_load_complete>(this);
        _listener_id = EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);
	}


	ProjectSelectionWindow::~ProjectSelectionWindow()
	{
        EventHandler::unregister_listener(SceneLoadEvent::sk_Type, _listener_id);
	}

	void ProjectSelectionWindow::window_contents(double dt)
	{
        ImGui::Begin(_title.c_str(), &_is_open, _flags);
        
        if(ImGui::Button("New Project"))
        {
            context::get_editor().open_window(EditorWindowType::PROJECT_WIZARD);
            close();
        }
        
        if(ImGui::Button("Load Project"))
        {
            _project_path = platform::open_file_dialog("");
            
            if(_project_path != "")
                context::get_editor().load_project(_project_path.c_str());
        }
        
        ImGui::End();
	}
    
    void ProjectSelectionWindow::initialize()
    {
        resize();
        _pos = imgui_helpers::get_pos_from_rel_center(_rel_pos, _size);
        _flags = ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    }
    
    void ProjectSelectionWindow::on_scene_load_complete(Event* event)
    {
        close();
        context::get_editor().open_window(EditorWindowType::MAIN_DOCK);
    }
}
