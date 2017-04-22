#pragma once

#include <string>

namespace terminus
{
	class Scene;

	namespace editor
	{
		extern void	initialize();
		extern void	shutdown();
		extern void	update();
		extern void	update_play_mode(double delta);
		extern void enter_play_mode();
		extern Scene* active_scene();
		extern void exit_play_mode();
		extern bool play_mode();
		extern void load_project(std::string project);
		extern void load_scene(std::string scene);
	}
}