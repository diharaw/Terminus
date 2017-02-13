#pragma once

#include <editor/ieditor_window.h>

namespace terminus
{
	class ProjectSelectionWindow : public IEditorWindow
	{
	public:
		ProjectSelectionWindow();
		virtual ~ProjectSelectionWindow();
		virtual void update(double dt) override;
	};
}