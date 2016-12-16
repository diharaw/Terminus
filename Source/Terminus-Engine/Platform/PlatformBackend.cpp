#include <iostream>
#include "PlatformBackend.h"
#include "../Input/Input.h"
#include "../Graphics/Config.h"
#include <string>
#include "../GUI/ImGuiBackend.h"
#include "../IO/FileSystem.h"

namespace PlatformBackend
{
    SDL_Window*            m_Window;
	int					   m_Width;
	int					   m_Height;
	WindowMode             m_window_mode;
	bool				   m_vsync;
	int					   m_refresh_rate;
    bool                   m_resizable;
	String				   m_title;
    bool                   m_is_running;

    bool CreateWindow()
    {
        Uint32 window_flags = 0;
        
        if(m_resizable)
            window_flags = SDL_WINDOW_RESIZABLE;
        
        switch (m_window_mode) {
                
            case WindowMode::BORDERLESS_WINDOW:
                window_flags |= SDL_WINDOW_BORDERLESS;
                break;
                
            case WindowMode::FULLSCREEN:
                window_flags |= SDL_WINDOW_FULLSCREEN;
                break;
                
            default:
                break;
        }
        
        m_title = "Terminus Engine - Build ";
        m_title += std::to_string(TERMINUS_BUILD);
        
        
#if defined(TERMINUS_OPENGL)
        m_title += " (OpenGL)";
        window_flags |= SDL_WINDOW_OPENGL;
#elif defined(TERMINUS_OPENGL_ES)
        m_title += " (OpenGL ES)";
        window_flags |= SDL_WINDOW_OPENGL;
#elif defined(TERMINUS_DIRECT3D11)
        m_title += " (Direct3D 11)";
#elif defined(TERMINUS_DIRECT3D12)
        m_title += " (Direct3D 12)";
#elif defined(TERMINUS_VULKAN)
        m_title += " (Vulkan)";
#endif
        
        m_Window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Height, m_Width, window_flags);
        if(!m_Window)
            return false;
        
        return true;
    }
    
    bool Initialize()
    {
        Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK;
        m_is_running = true;
        
        if (SDL_Init(flags) != 0)
            return false;

		JsonDocument config;
		FileHandle handle = FileSystem::read_file("TerminusConfig.json", true);

		if (handle.buffer)
		{
			config.Parse(handle.buffer);
			
			if (config.HasMember("width"))
				m_Width = config["width"].GetInt();
			else
				return false;

			if (config.HasMember("height"))
				m_Height = config["height"].GetInt();
			else
				return false;

			if (config.HasMember("refresh_rate"))
				m_refresh_rate = config["refresh_rate"].GetInt();
			else
				return false;

			if (config.HasMember("vsync"))
				m_vsync = config["vsync"].GetBool();
			else
				return false;

			if (config.HasMember("window_mode"))
            {
                String window_mode_str = String(config["window_mode"].GetString());
                
                if(window_mode_str == "WINDOWED")
                    m_window_mode = WindowMode::WINDOWED;
                else if(window_mode_str == "BORDERLESS_WINDOW")
                    m_window_mode = WindowMode::BORDERLESS_WINDOW;
                else if(window_mode_str == "FULLSCREEN")
                    m_window_mode = WindowMode::FULLSCREEN;
                else
                    m_window_mode = WindowMode::WINDOWED;
            }
			else
				return false;
            
            if (config.HasMember("resizable"))
                m_resizable = config["resizable"].GetBool();
            else
                return false;
		}
		else
		{
			m_Width = 1280;
			m_Height = 720;
			m_refresh_rate = 60;
            m_window_mode = WindowMode::WINDOWED;
			m_vsync = false;
		}
		
        CreateWindow();

        if(!m_Window)
        {
            SDL_Quit();
            return false;
        }
        
        // Poll input once to set initial mouse position
        Update();
        
        Input::MouseDevice* device = Input::GetMouseDevice();
        SDL_GetMouseState(&device->x_position, &device->y_position);

        return true;
    }
    
    void Shutdown()
    {
        SDL_DestroyWindow(m_Window);
    }
    
    void Update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Input::ProcessWindowEvents(event);
            Terminus::ImGuiBackend::process_window_events(&event);
            
            switch (event.type) {
                case SDL_QUIT:
                    m_is_running = false;
                    break;
                    
                default:
                    break;
            }
        }
    }

    void SetWindowMode(WindowMode mode)
	{
        m_window_mode = mode;
        CreateWindow();
	}

	void SetWindowSize(uint width, uint height)
	{
		m_Width = width;
		m_Height = height;

        SDL_SetWindowSize(m_Window, width, height);
	}
    
    void RequestShutdown()
    {
        m_is_running = false;
    }
    
    bool IsShutdownRequested()
    {
        return !m_is_running;
    }
    
    SDL_Window* GetWindow()
    {
        return m_Window;
    }

	int GetWidth()
	{
		return m_Width;
	}

	int GetHeight()
	{
		return m_Height;
	}

#if defined(WIN32)
	HWND GetHandleWin32()
	{
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(g_Window, &wmInfo);
        return wmInfo.info.win.window;
	}
#endif
}
