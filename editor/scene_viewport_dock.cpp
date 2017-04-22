#include "scene_viewport_dock.h"
#include <imgui.h>
#include <imgui_dock.h>
#include <utility/imgui_helper.h>

#include <core/Event/event_handler.h>

namespace terminus
{
	SceneViewportDock::SceneViewportDock()
	{

	}

	SceneViewportDock::~SceneViewportDock()
	{

	}

	void SceneViewportDock::update_internal(double dt)
	{
		Vector2 rect = imgui_helpers::get_dock_size();

		if (rect.x != _rect.x || rect.y != _rect.y)
		{
			_rect = rect;

			//DrawableRectEvent* event = new DrawableRectEvent(rect);
			//EventHandler::queue_event(event);
		}
	}
}