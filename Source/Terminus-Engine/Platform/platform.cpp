#include <iostream>
#include <Platform/platform.h>
#include <Input/input_handler.h>
#include <Core/config.h>
#include <Graphics/imgui_backend.h>
#include <IO/filesystem.h>
#include <Core/context.h>

#include <string>
#include <SDL_syswm.h>

namespace terminus
{
    Platform::Platform()
    {
        
    }
    
    Platform::~Platform()
    {
        
    }
    
    bool Platform::create_platform_window()
    {
        Uint32 window_flags = 0;
        
        if(_resizable)
            window_flags = SDL_WINDOW_RESIZABLE;
        
        switch (_window_mode) {
                
            case WindowMode::BORDERLESS_WINDOW:
                window_flags |= SDL_WINDOW_BORDERLESS;
                break;
                
            case WindowMode::FULLSCREEN:
                window_flags |= SDL_WINDOW_FULLSCREEN;
                break;
                
            default:
                break;
        }
        
#if defined(TERMINUS_OPENGL)
        _title += " (OpenGL)";
        window_flags |= SDL_WINDOW_OPENGL;
#elif defined(TERMINUS_OPENGL_ES)
        _title += " (OpenGL ES)";
        window_flags |= SDL_WINDOW_OPENGL;
#elif defined(TERMINUS_DIRECT3D11)
        _title += " (Direct3D 11)";
#elif defined(TERMINUS_DIRECT3D12)
        _title += " (Direct3D 12)";
#elif defined(TERMINUS_VULKAN)
        _title += " (Vulkan)";
#endif
        
        _window = SDL_CreateWindow(_title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   _width,
                                   _height,
                                   window_flags | SDL_WINDOW_ALLOW_HIGHDPI);
        if(!_window)
            return false;

        
        return true;
    }
    
    bool Platform::initialize()
    {
        Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK;
        
        if (SDL_Init(flags) != 0)
            return false;

        ConfigFile* config = context::get_engine_config();
        
        _width = config->get_int(CONFIG_WINDOW_WIDTH, 1280);
        _height = config->get_int(CONFIG_WINDOW_HEIGHT, 720);
        _refresh_rate = config->get_int(CONFIG_WINDOW_REFRESH_RATE, 60);
        _resizable = config->get_bool(CONFIG_WINDOW_RESIZABLE, false);
        _vsync = config->get_bool(CONFIG_WINDOW_VSYNC, false);
        
        StringBuffer64 mode = config->get_string(CONFIG_WINDOW_MODE, "windowed");
            
        if(mode == "fullscreen")
            _window_mode = WindowMode::FULLSCREEN;
        else if(mode == "borderless")
            _window_mode = WindowMode::BORDERLESS_WINDOW;
        else
            _window_mode = WindowMode::WINDOWED;
        
        _title = config->get_string(CONFIG_WINDOW_TITLE, "Terminus App");
        
        if(config->has_value(CONFIG_APP_VERSION))
        {
            _title += " ";
            _title += config->get_string(CONFIG_APP_VERSION);
        }
        
        if(config->has_value(CONFIG_ENGINE_VERSION))
        {
            _title += " - Terminus Engine ";
            _title += config->get_string(CONFIG_ENGINE_VERSION);
        }
        
		create_platform_window();
        
        _delta      = 0;

        if(!_window)
        {
            SDL_Quit();
            return false;
        }
        
        // Poll input once to set initial mouse position
        update();
        
        input_handler::MouseDevice& device = input_handler::get_mouse_device();
        SDL_GetMouseState(&device.x_position, &device.y_position);

        return true;
    }
    
    void Platform::shutdown()
    {
        SDL_DestroyWindow(_window);
    }
    
    void Platform::update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            input_handler::process_window_events(event);
            terminus::ImGuiBackend::process_window_events(&event);
            
            switch (event.type)
            {
                case SDL_QUIT:
                    Global::get_context()._shutdown = true;
                    break;
                    
                default:
                    break;
            }
        }
    }

    void Platform::set_window_mode(WindowMode mode)
	{
        _window_mode = mode;
		create_platform_window();
	}

	void Platform::set_window_size(uint width, uint height)
	{
		_width = width;
		_height = height;

        SDL_SetWindowSize(_window, width, height);
	}
    
    void Platform::request_shutdown()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Global::get_context()._shutdown = true;
    }

	void Platform::set_relative_mouse(bool relative)
	{
		if(relative)
			SDL_SetRelativeMouseMode(SDL_TRUE);
		else
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);

			//input_handler::MouseDevice& mouse_device = input_handler::get_mouse_device();
			//SDL_WarpMouseInWindow(_window, mouse_device.x_position, mouse_device.y_position);
		}
	}

	void Platform::grab_mouse(bool grab)
	{
		if (grab)
			SDL_CaptureMouse(SDL_TRUE);
		else
			SDL_CaptureMouse(SDL_FALSE);
	}

	void Platform::set_cursor_visibility(bool visible)
	{
		SDL_ShowCursor(visible);
	}
    
    SDL_Window* Platform::get_window()
    {
        return _window;
    }

	int Platform::get_width()
	{
		return _width;
	}

	int Platform::get_height()
	{
		return _height;
	}
    
    void Platform::begin_frame()
    {
        _timer.start();
    }
    
    void Platform::end_frame()
    {
        _timer.stop();
        _delta = _timer.get_elapsed_time_sec();
    }
    
    double Platform::get_delta_time()
    {
       return _delta;
    }

#if defined(WIN32)
	HWND Platform::get_handle_win32()
	{
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(_window, &wmInfo);
        return wmInfo.info.win.window;
	}
#endif
} // namespace terminus
