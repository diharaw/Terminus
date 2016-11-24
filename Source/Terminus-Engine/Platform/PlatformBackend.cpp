#include <iostream>
#include "PlatformBackend.h"
#include "../Input/Input.h"
#include "../Graphics/Config.h"
#include <string>
#include "../GUI/ImGuiBackend.h"
#include "../IO/FileSystem.h"

#ifdef TERMINUS_OPENGL
#ifdef __APPLE__
#define GL_MAX_MAJOR_VERSION 4
#define GL_MAX_MINOR_VERSION 1
#else
#define GL_MAX_MAJOR_VERSION 4
#define GL_MAX_MINOR_VERSION 5
#endif
#endif

namespace PlatformBackend
{
    GLFWwindow* m_Window;
	GLFWmonitor* m_monitor;
	int					   m_Width;
	int					   m_Height;
	bool					   m_fullscreen;
	bool					   m_vsync;
	int					   m_refresh_rate;
	String				   m_title;

	// Callbacks

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
        Input::ProcessMouseButtonInput(button, action);
		Terminus::ImGuiBackend::mouse_button_callback(window, button, action, mods);
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Terminus::ImGuiBackend::scroll_callback(window, xoffset, yoffset);
	}
    
    void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::ProcessCursorInput(xpos, ypos);
    }

	void KeyCallback(GLFWwindow* window, int key, int param, int action, int mods)
	{
        Input::ProcessKeyboardInput(key, action);
		Terminus::ImGuiBackend::key_callback(window, key, param, action, mods);
	}

	void CharCallback(GLFWwindow* window, unsigned int c)
	{
		Terminus::ImGuiBackend::char_callback(window, c);
	}
    
    bool Initialize()
    {
        if (!glfwInit())
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

			if (config.HasMember("fullscreen"))
				m_fullscreen = config["fullscreen"].GetBool();
			else
				return false;
		}
		else
		{
			m_Width = 1280;
			m_Height = 720;
			m_refresh_rate = 60;
			m_fullscreen = false;
			m_vsync = false;
		}

		m_title = "Terminus Engine - Build ";
		m_title += std::to_string(TERMINUS_BUILD);

#if defined(TERMINUS_OPENGL)
		title += " (OpenGL)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAX_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MAX_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(TERMINUS_OPENGL_ES)
		title += " (OpenGL ES)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(TERMINUS_DIRECT3D11)  || defined(TERMINUS_DIRECT3D12) || defined(TERMINUS_VULKAN) 
		m_title += " (Direct3D 11)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		
		m_monitor = glfwGetPrimaryMonitor();
		SetFullscreen(m_fullscreen);

        if(!m_Window)
        {
            glfwTerminate();
            return false;
        }

        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetScrollCallback(m_Window, ScrollCallback);
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetCursorPosCallback(m_Window, CursorPositionCallback);
        glfwSetCharCallback(m_Window, CharCallback);
        
        // Poll input once to set initial mouse position
        glfwPollEvents();
        
        Input::MouseDevice* device = Input::GetMouseDevice();
        glfwGetCursorPos(m_Window, &device->x_position, &device->y_position);

        return true;
    }
    
    void Shutdown()
    {
        glfwTerminate();
    }
    
    void Update()
    {
        glfwPollEvents();
    }

	void SetFullscreen(bool fullscreen)
	{
		m_fullscreen = fullscreen;

		if (m_Window)
		{
			if (m_fullscreen)
				m_Window = glfwCreateWindow(m_Width, m_Height, m_title.c_str(), m_monitor, NULL);
			else
				m_Window = glfwCreateWindow(m_Width, m_Height, m_title.c_str(), NULL, NULL);
		}
		else
		{
			const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
			
			if (m_fullscreen)
				m_monitor = glfwGetPrimaryMonitor();
			else
				m_monitor = NULL;

			glfwSetWindowMonitor(m_Window, m_monitor, 0, 0, m_Width, m_Height, mode->refreshRate);
		}
	}

	void SetWindowSize(uint width, uint height)
	{
		m_Width = width;
		m_Height = height;

		const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
		glfwSetWindowMonitor(m_Window, NULL, 0, 0, m_Width, m_Height, mode->refreshRate);
	}
    
    void RequestShutdown()
    {
        glfwSetWindowShouldClose(m_Window, GLFW_FALSE);
    }
    
    bool IsShutdownRequested()
    {
        return glfwWindowShouldClose(m_Window);
    }
    
    GLFWwindow* GetWindow()
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
		return glfwGetWin32Window(m_Window);
	}
#endif
}
