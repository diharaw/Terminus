#include <editor/project_selection_window.h>

namespace terminus
{
    ProjectSelectionWindow::ProjectSelectionWindow(std::string title,
                                                   float rel_width,
                                                   float rel_height) : IEditorWindow(title, rel_width, rel_height)
	{
        
	}

	ProjectSelectionWindow::~ProjectSelectionWindow()
	{

	}

	void ProjectSelectionWindow::window_contents(double dt)
	{
        ImGui::Text("Hello ImGui!");
	}
}
