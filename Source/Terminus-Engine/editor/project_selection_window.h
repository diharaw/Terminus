#pragma once

#include <editor/ieditor_window.h>

namespace terminus
{
	class ProjectSelectionWindow : public IEditorWindow
	{
	public:
        ProjectSelectionWindow(std::string title, float rel_width, float rel_height);
		virtual ~ProjectSelectionWindow();
		virtual void window_contents(double dt) override;
	};
}
