#pragma once

#include <platform/include/window.hpp>

int main()
{
	te::Window window;

	te::WindowCreateDesc desc
	{
		800,
		600,
		TE_WINDOW_POS_CENTERED,
		TE_WINDOW_POS_CENTERED,
		"Terminus Test",
		te::WindowFlag::RESIZABLE
	};

	window.initialize(desc);

	while (window.running())
	{
		te::OsEvent event;
		while (window.poll_event(event))
		{

		}
	}

	window.shutdown();

	return 0;
}