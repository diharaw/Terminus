#include "editor_dock.h"
#include <imgui.h>
#include <imgui_dock.h>
#include <utility/imgui_helper.h>

namespace terminus
{
	EditorDock::EditorDock()
	{
		_visible = true;
	}

	EditorDock::~EditorDock()
	{

	}

	void EditorDock::set_title(StringBuffer32 title)
	{
		_title = title;
	}

	void EditorDock::update(double dt)
	{
		if (ImGui::BeginDock(_title.c_str(), &_visible))
		{
			update_internal(dt);
			_rect = imgui_helpers::get_dock_size();
		}
		ImGui::EndDock();
	}

	Vector2 EditorDock::size()
	{
		return _rect;
	}
}