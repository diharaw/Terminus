#include <iostream>

#include <io/include/file.hpp>
#include <io/src/filesystem_impl.hpp>
#include <io/src/input_manager_impl.hpp>
#include <io/src/input_map_impl.hpp>
#include <platform/src/window_sdl2.hpp>
#include <event/src/event_manager_impl.hpp>
#include <SDL.h>

using namespace te;

FileSystem fs;
InputManagerImpl input_manager;
EventManagerImpl event_manager;

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
                input_manager.process_mouse_wheel(&event_manager, e.wheel.y);
                break;
            }
            case SDL_MOUSEMOTION:
            {
                SDL_bool relative = SDL_GetRelativeMouseMode();
                input_manager.process_mouse_motion(&event_manager, e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel, relative);
            }
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            {
                if(e.type == SDL_MOUSEBUTTONDOWN)
                    input_manager.process_mouse_button(&event_manager, e.button.button, TE_BUTTON_STATE_PRESSED);
                else
                    input_manager.process_mouse_button(&event_manager, e.button.button, TE_BUTTON_STATE_RELEASED);
                
                break;
            }
            // --------------------------------------------------------------------------------
            // KEYBOARD
            // --------------------------------------------------------------------------------
            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                if(e.key.repeat == 0)
                {
                    if(e.type == SDL_KEYUP)
                        input_manager.process_keyboard_button(&event_manager, e.key.keysym.scancode, TE_BUTTON_STATE_RELEASED);
                    else
                        input_manager.process_keyboard_button(&event_manager, e.key.keysym.scancode, TE_BUTTON_STATE_PRESSED);
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


void test_filesystem()
{
    fs.vfs_mount_dir("/Users/diharaw/Desktop/fs", "somedir");
    //te::filesystem::mount_dir("/Users/diharaw/Desktop/fs/ar.zip", "somedir");
    
    te::IFile* test = fs.vfs_open_file("somedir/inner/test.txt", TE_FS_READ | TE_FS_TEXT);
    
    size_t size = test->size();
    char* buf = (char*)malloc(size + 1);
    
    test->read(buf, size, 1);
    buf[size] = '\0';
    
    std::cout << buf << std::endl;
    
    test->close();
    
    free(buf);
}

void on_state_input(te::Event* e)
{
    if(e->data.input_state.hash == TE_HASH("fire"))
    {
        if(e->data.input_state.state == 1)
            std::cout << "Fire State ON" << std::endl;
        else
            std::cout << "Fire State OFF" << std::endl;
    }
}

void on_action_input(te::Event* e)
{
    if(e->data.input_action.hash == TE_HASH("block"))
        std::cout << "Block action" << std::endl;
}

void on_axis_input(te::Event* e)
{
    if(e->data.input_axis.hash == TE_HASH("forward"))
    {
        std::cout << "Forward axis : " << e->data.input_axis.value << std::endl;
    }
}

int main()
{
    uint32_t flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;
    
    if (SDL_Init(flags) != 0)
        return false;
    
    WindowCreateDesc desc
    {
        1280,
        720,
        TE_WINDOW_POS_CENTERED,
        TE_WINDOW_POS_CENTERED,
        "Terminus Test",
        TE_WINDOW_RESIZABLE
    };
    
    Window window;
    window.initialize(desc);
    input_manager.initialize();
    
    uint32_t player = input_manager.create_player();
    
    InputMapImpl input_map;
    input_map.set_action(TE_HASH("block"), TE_MOUSE_LEFT, TE_UNASSIGNED_INPUT, TE_UNASSIGNED_INPUT);
    input_map.set_state(TE_HASH("fire"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_A, TE_UNASSIGNED_INPUT);
    input_map.set_axis(TE_HASH("forward"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_W, TE_KEYBOARD_S, TE_UNASSIGNED_INPUT);
    
    input_manager.set_input_map(&input_map, player);
    
    uint32_t callback_hndl = event_manager.register_callback(TE_EVENT_STATE_INPUT, on_state_input);
    
    event_manager.register_callback(TE_EVENT_AXIS_INPUT, on_axis_input);
    event_manager.register_callback(TE_EVENT_ACTION_INPUT, on_action_input);
    
    while (running)
    {
        SDL_EventLoop();
        event_manager.process_events();
    }
    
    event_manager.unregister_callback(TE_EVENT_STATE_INPUT, callback_hndl);
    
    window.shutdown();
    
    SDL_Quit();
    

	return 0;
}
