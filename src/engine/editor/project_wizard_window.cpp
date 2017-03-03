#include <editor/project_wizard_window.h>
#include <Core/context.h>

namespace terminus
{
    ProjectWizardWindow::ProjectWizardWindow(std::string title,
                                                   Vector2 rel_size,
                                                   Vector2 size,
                                                   Vector2 rel_pos,
                                                   Vector2 pos,
                                                   int flags) : IEditorWindow(title, rel_size, size, rel_pos, pos, flags)
    {
       
    }
    
    
    ProjectWizardWindow::~ProjectWizardWindow()
    {

    }
    
    void ProjectWizardWindow::window_contents(double dt)
    {
        ImGui::Begin(_title.c_str(), &_is_open, _flags);
        
        ImGui::InputText("Project Name", (char*)_project_name.c_str(), _project_name.size());
        ImGui::InputText("Path", (char*)_save_path.c_str(), _save_path.size());
        
        ImGui::SameLine();
        
        if(ImGui::Button("Browse"))
        {
            String path = platform::save_file_dialog("");
            _save_path = path.c_str();
        }
        
        if(ImGui::Button("Back"))
        {
            context::get_editor().open_window(EditorWindowType::PROJECT_SELECTION);
            close();
        }
        
        ImGui::SameLine();
        
        if(ImGui::Button("Create"))
        {
            context::get_editor().create_project(_project_name.c_str(), _save_path.c_str());
            context::get_editor().open_window(EditorWindowType::MAIN_DOCK);
            close();
        }
        
        ImGui::End();
    }
    
    void ProjectWizardWindow::initialize()
    {
        resize();
        _project_name = "Untitled Project";
        _save_path = "";
        _pos = imgui_helpers::get_pos_from_rel_center(_rel_pos, _size);
        _flags = ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    }
}
