#include <iostream>
#include "PlatformBackend.h"
#include "../Input/Input.h"
#include "../Graphics/Config.h"
#include <string>
#include "../GUI/ImGuiBackend.h"

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
	int m_Width;
	int m_Height;

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
    
    bool Initialize(int _Width, int _Height)
    {
        if (!glfwInit())
            return false;

		m_Width = _Width;
		m_Height = _Height;
		std::string title = "Terminus Engine - Build ";
		title += std::to_string(TERMINUS_BUILD);

#if defined(TERMINUS_OPENGL)
		title += " (OpenGL)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAX_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MAX_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(TERMINUS_OPENGL_ES)
		title += " (OpenGL ES)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(TERMINUS_DIRECT3D11)
		title += " (Direct3D 11)";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		
        m_Window = glfwCreateWindow(_Width, _Height, title.c_str(), NULL, NULL);
        
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
    
    bool CreateFromFile()
    {
        return false;
    }
    
    void Shutdown()
    {
        glfwTerminate();
    }
    
    void Update()
    {
        glfwPollEvents();
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
