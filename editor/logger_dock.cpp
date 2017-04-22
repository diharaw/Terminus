#include "logger_dock.h"
#include <imgui.h>
#include <imgui_dock.h>
#include <graphics/imgui_console.h>

namespace terminus
{
	LoggerDock::LoggerDock()
	{

	}

	LoggerDock::~LoggerDock()
	{

	}

	void LoggerDock::update_internal(double dt)
	{
		imgui_console::draw();
	}
}