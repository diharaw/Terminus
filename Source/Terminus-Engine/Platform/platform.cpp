#include <iostream>
#include "platform.h"
#include "../Input/Input.h"
#include "../Core/Config.h"
#include <string>
#include "../GUI/ImGuiBackend.h"
#include "../IO/FileSystem.h"
#include <SDL_syswm.h>

namespace platform
{
    SDL_Window*            _window;
	int					   _width;
	int					   _height;
	WindowMode             _window_mode;
	bool				   _vsync;
	int					   _refresh_rate;
    bool                   _resizable;
	String				   _title;
    bool                   _is_running;

    bool create_platform_window()
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
        
        _title = "Terminus Engine - Build ";
        _title += std::to_string(TERMINUS_BUILD);
        
        
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
                                   window_flags);
        if(!_window)
            return false;
        
        return true;
    }
    
    bool initialize()
    {
        Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK;
        _is_running = true;
        
        if (SDL_Init(flags) != 0)
            return false;

		JsonDocument config;
		FileHandle handle = FileSystem::read_file("TerminusConfig.json", true);

		if (handle.buffer)
		{
			config.Parse(handle.buffer);
			
			if (config.HasMember("width"))
				_width = config["width"].GetInt();
			else
				return false;

			if (config.HasMember("height"))
				_height = config["height"].GetInt();
			else
				return false;

			if (config.HasMember("refresh_rate"))
				_refresh_rate = config["refresh_rate"].GetInt();
			else
				return false;

			if (config.HasMember("vsync"))
				_vsync = config["vsync"].GetBool();
			else
				return false;

			if (config.HasMember("window_mode"))
            {
                String window_mode_str = String(config["window_mode"].GetString());
                
                if(window_mode_str == "WINDOWED")
                    _window_mode = WindowMode::WINDOWED;
                else if(window_mode_str == "BORDERLESS_WINDOW")
                    _window_mode = WindowMode::BORDERLESS_WINDOW;
                else if(window_mode_str == "FULLSCREEN")
                    _window_mode = WindowMode::FULLSCREEN;
                else
                    _window_mode = WindowMode::WINDOWED;
            }
			else
				return false;
            
            if (config.HasMember("resizable"))
                _resizable = config["resizable"].GetBool();
            else
                return false;
		}
		else
		{
			_width = 1280;
			_height = 720;
			_refresh_rate = 60;
            _resizable = true;
            _window_mode = WindowMode::WINDOWED;
			_vsync = false;
		}
		
		create_platform_window();

        if(!_window)
        {
            SDL_Quit();
            return false;
        }
        
        // Poll input once to set initial mouse position
        update();
        
        Input::MouseDevice* device = Input::GetMouseDevice();
        SDL_GetMouseState(&device->x_position, &device->y_position);

        return true;
    }
    
    void shutdown()
    {
        SDL_DestroyWindow(_window);
    }
    
    void update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Input::ProcessWindowEvents(event);
            Terminus::ImGuiBackend::process_window_events(&event);
            
            switch (event.type) {
                case SDL_QUIT:
                    _is_running = false;
                    break;
                    
                default:
                    break;
            }
        }
    }

    void set_window_mode(WindowMode mode)
	{
        _window_mode = mode;
		create_platform_window();
	}

	void set_window_size(uint width, uint height)
	{
		_width = width;
		_height = height;

        SDL_SetWindowSize(_window, width, height);
	}
    
    void request_shutdown()
    {
        _is_running = false;
    }
    
    bool shutdown_requested()
    {
        return !_is_running;
    }
    
    SDL_Window* get_window()
    {
        return _window;
    }

	int get_width()
	{
		return _width;
	}

	int get_height()
	{
		return _height;
	}

#if defined(WIN32)
	HWND get_handle_win32()
	{
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(m_Window, &wmInfo);
        return wmInfo.info.win.window;
	}
#endif
}
