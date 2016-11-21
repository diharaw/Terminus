#ifndef PlatformBackend_h
#define PlatformBackend_h

#include "../../Third Party/glew/include/GL/glew.h"
#include "../Types.h"
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <boxer/boxer.h>

#if defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3native.h"
#endif

enum class Selection
{
    OK,
    Cancel,
    Yes,
    No,
    None,
    Error
};

enum class Buttons
{
    OK,
    OKCancel,
    YesNo
};

enum class Style
{
    Info,
    Warning,
    Error,
    Question
};

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

#if defined(WIN32)
	extern HWND GetHandleWin32();
#endif
    
    inline Selection show_message_box(String _Message, String _Title, Style _Style = Style::Info, Buttons _Buttons = Buttons::OK)
    {
        Selection result = (Selection)boxer::show(_Message.c_str(), _Title.c_str(), (boxer::Style)_Style, (boxer::Buttons)_Buttons);
        return result;
    }
    
    inline String* open_file_dialog(String _Extensions, String _DefaultPath = "")
    {
        char* openPath = NULL;
        nfdresult_t result = NFD_OpenDialog(_Extensions.c_str(), _DefaultPath.c_str(), &openPath);
        if (result == NFD_OKAY)
        {
            String* openPathStr = new String(openPath);
            return openPathStr;
        }
        else
            return nullptr;
    }
    
    inline String* save_file_dialog(String _Extensions, String _DefaultPath = "")
    {
        char* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog(_Extensions.c_str(), _DefaultPath.c_str(), &savePath);
        if (result == NFD_OKAY)
        {
            String* savePathStr = new String(savePath);
            return savePathStr;
        }
        else
            return nullptr;
    }
}

#endif 
