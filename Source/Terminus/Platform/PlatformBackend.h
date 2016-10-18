#ifndef PlatformBackend_h
#define PlatformBackend_h

#include "../../Third Party/glew/include/GL/glew.h"
#include <GLFW/glfw3.h>

namespace PlatformBackend
{
    /**
     * Initializes Window and optionally Creates OpenGL/OpenGL ES context if OpenGL is requested
     * @param _Width Width of window.
     * @param _Height Height of window.
     * @return true if Initialization is successful. false if failed.
     */
    extern bool Initialize(int width, int height);
    /**
     * Initializes Window via a configuration file containing window properties
     * @return true if Initialization is successful. false if failed.
     */
    extern bool CreateFromFile();
    /**
     * Shutdown window and rendering conxtet if present
     */
    extern void Shutdown();
    /**
     * Polls for input events
     */
    extern void Update();
    /**
     * Requests window Shutdown
     */
    extern void RequestShutdown();
    /**
     * Check if an Application shutdown was requested
     * @return true window should shutdown. false if not.
     */
    extern bool IsShutdownRequested();
    /**
     * Retrieve the GLFWwindow pointer
     * @return GLFWwindow pointer
     */
    extern GLFWwindow* GetWindow();
    /**
     * Get Window Width
     * @return int Window Width
     */
    extern int GetWidth();
    /**
     * Get Window Height
     * @return int Window Height
     */
	extern int GetHeight();
}

#endif 
