#ifndef PlatformBackend_h
#define PlatformBackend_h

#include <Core/config.h>
#include <GL/glew.h>
#include <types.h>
#include <SDL.h>
#include <mutex>
#include <Utility/timer.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#endif

#if !defined(TERMINUS_PLATFORM_IOS)
    #include <nfd.h>
    #include <boxer/boxer.h>
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

enum class WindowMode
{
    WINDOWED,
    BORDERLESS_WINDOW,
    FULLSCREEN
};


namespace terminus
{
    class Platform
    {
    public:
        
        Platform();
        
        ~Platform();
        
        bool initialize();
        
        void shutdown();
        
        void update();
        
        void request_shutdown();

		void set_cursor_visibility(bool visible);

		void set_relative_mouse(bool relative);

		void grab_mouse(bool grab);
        
        void set_window_mode(WindowMode mode);
        
        void set_window_size(uint width, uint height);
        
        SDL_Window* get_window();
        
        int get_width();
        
        int get_height();
        
        void begin_frame();
        void end_frame();
        double get_delta_time();
        
#if defined(WIN32)
        HWND get_handle_win32();
#endif
        
    private:
        bool create_platform_window();
        
    private:
        SDL_Window*            _window;
        int					   _width;
        int					   _height;
        WindowMode             _window_mode;
        bool				   _vsync;
        int					   _refresh_rate;
        bool                   _resizable;
        String				   _title;
        std::mutex             _mutex;
        double                 _delta;
        Timer                  _timer;
        
    };
    
    namespace platform
    {
        inline Selection show_message_box(String _Message, String _Title, Style _Style = Style::Info, Buttons _Buttons = Buttons::OK)
        {
#if !defined(TERMINUS_PLATFORM_IOS)
            Selection result = (Selection)boxer::show(_Message.c_str(), _Title.c_str(), (boxer::Style)_Style, (boxer::Buttons)_Buttons);
            return result;
#else
            return Selection::OK;
#endif
        }
        
        inline String open_file_dialog(String _Extensions, String _DefaultPath = "")
        {
#if !defined(TERMINUS_PLATFORM_IOS)
            char* openPath = NULL;
            nfdresult_t result = NFD_OpenDialog(_Extensions.c_str(), _DefaultPath.c_str(), &openPath);
            if (result == NFD_OKAY)
            {
                return String(openPath);
            }
            else
                return nullptr;
#else
            return nullptr;
#endif
        }
        
        inline String save_file_dialog(String _Extensions, String _DefaultPath = "")
        {
#if !defined(TERMINUS_PLATFORM_IOS)
            char* savePath = NULL;
            nfdresult_t result = NFD_SaveDialog(_Extensions.c_str(), _DefaultPath.c_str(), &savePath);
            if (result == NFD_OKAY)
            {
                return String(savePath);
            }
            else
                return nullptr;
#else
            return nullptr;
#endif
        }
    }
}

#endif
