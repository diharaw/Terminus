#include <platform/src/window_sdl2.hpp>
#include <SDL.h>
#include <iostream>

int main()
{
    uint32_t flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK;
    
    if (SDL_Init(flags) != 0)
        return false;
    
	te::WindowCreateDesc desc
	{
		1280,
		720,
		TE_WINDOW_POS_CENTERED,
		TE_WINDOW_POS_CENTERED,
		"Terminus Test",
        TE_WINDOW_RESIZABLE
	};

    te::WindowCreateDesc desc2
    {
        800,
        600,
        TE_WINDOW_POS_CENTERED,
        TE_WINDOW_POS_CENTERED,
        "Terminus Test 2",
        TE_WINDOW_RESIZABLE
    };
    
    te::Window window;
    window.initialize(desc);
    
    te::Window window2;

    window2.initialize(desc2);
    
    uint32_t count = 2;
    
    bool running = true;
    
    while (count > 0)
	{
		
	}

    window.shutdown();
    window2.shutdown();
    
    SDL_Quit();
    
	return 0;
}
