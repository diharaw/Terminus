#include <platform/src/window_sdl2.hpp>
#include <event/src/event_manager_impl.hpp>
#include <iostream>
#include <SDL.h>

bool running = true;

void SDL_EventLoop()
{
    SDL_Event e;
    
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            // --------------------------------------------------------------------------------
            // QUIT
            // --------------------------------------------------------------------------------
            case SDL_QUIT:
            {
                running = false;
            }
            // --------------------------------------------------------------------------------
            // MOUSE
            // --------------------------------------------------------------------------------
            case SDL_MOUSEWHEEL:
            {
                //                    event.mouse_wheel.value = sdl_event.wheel.y;
            }
            case SDL_MOUSEMOTION:
            {
                //                    event.mouse_motion.xpos = sdl_event.motion.x;
                //                    event.mouse_motion.ypos = sdl_event.motion.y;
                //                    event.mouse_motion.xrel = sdl_event.motion.xrel;
                //                    event.mouse_motion.yrel = sdl_event.motion.yrel;
                //
                //                    SDL_bool relative = SDL_GetRelativeMouseMode();
                //
                //                    event.mouse_motion.relative = relative;
            }
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            {
                //                    event.mouse_button.button = sdl_event.button.button;
            }
            // --------------------------------------------------------------------------------
            // KEYBOARD
            // --------------------------------------------------------------------------------
            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                if(e.key.repeat == 0)
                {
                    uint8_t state = 0;
                    
//                    if(e.type == SDL_KEYDOWN)
//                        state = te::TE_BUTTON_STATE_PRESSED;
//                    else
//                        state = TE_BUTTON_STATE_RELEASED;
                    
                    //                        event.keyboard.scancode = e.key.keysym.scancode;;
                    //                        event.keyboard.state = state;
                }
            }
            // --------------------------------------------------------------------------------
            // CONTROLLER
            // --------------------------------------------------------------------------------
            case SDL_CONTROLLERAXISMOTION:
            {
                
            }
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERBUTTONDOWN:
            {
                
            }
            case SDL_CONTROLLERDEVICEADDED:
            {
                
            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {

            }
            // --------------------------------------------------------------------------------
            // JOYSTICK
            // --------------------------------------------------------------------------------
            case SDL_JOYAXISMOTION:
            {
                
            }
            case SDL_JOYBALLMOTION:
            {
                
            }
            case SDL_JOYHATMOTION:
            {
                
            }
            case SDL_JOYBUTTONUP:
            case SDL_JOYBUTTONDOWN:
            {
                
            }
            case SDL_JOYDEVICEADDED:
            {
                
            }
            case SDL_JOYDEVICEREMOVED:
            {
                
            }
            // --------------------------------------------------------------------------------
            // TOUCH
            // --------------------------------------------------------------------------------
            case SDL_FINGERMOTION:
            {
                
            }
            case SDL_FINGERUP:
            case SDL_FINGERDOWN:
            {
                
            }
            case SDL_MULTIGESTURE:
            {
                
            }
            // --------------------------------------------------------------------------------
            // WINDOW
            // --------------------------------------------------------------------------------
            case SDL_WINDOWEVENT:
            {
                //                    event.window.type = e.window.event;
                //                    event.window.id = e.window.windowID;
                
            }
            // --------------------------------------------------------------------------------
            // TEXT
            // --------------------------------------------------------------------------------
            case SDL_TEXTEDITING:
            {
                
            }
            case SDL_TEXTINPUT:
            {
                
            }
            // --------------------------------------------------------------------------------
            // DROP
            // --------------------------------------------------------------------------------
            case SDL_DROPFILE:
            {
                
            }
            case SDL_DROPTEXT:
            {
                
            }
            case SDL_DROPBEGIN:
            {
                
            }
            case SDL_DROPCOMPLETE:
            {
                
            }
        }
    }
}

extern "C" int main(int argc, char *argv[])
{
    uint32_t flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;
    
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
    
    te::Window window;
	window.initialize(desc);
    
    te::EventManagerImpl event_manager;
    
//    uint32_t callback_hndl = event_manager.register_callback(TE_EVENT_ACTION_INPUT, [](te::Event* e){
//        std::cout << "Action input" << std::endl;
//    });
    
	while (running)
	{
        SDL_EventLoop();
        event_manager.process_events();
	}

	window.shutdown();

    SDL_Quit();
    
	return 0;
}
