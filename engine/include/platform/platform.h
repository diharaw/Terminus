#pragma once

#include <core/types.h>
#include <core/config.h>
#include <utility/timer.h>
#include <container/hash_map.h>
#include <mutex>

#if defined(TERMINUS_OPENGL)
    #include <GL/glew.h>
#endif

#if defined(TERMINUS_PLATFORM_WIN32)
	#include <Windows.h>
#endif

#define MAX_KEYBOARD_INPUTS 100
#define MAX_MOUSE_INPUTS 10
#define MAX_GAMEPAD_INPUTS 20

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
		virtual ~Platform();
        virtual bool initialize() = 0;
		virtual void shutdown() = 0;
        virtual void update() = 0;
		virtual void set_cursor_visibility(bool visible) = 0;
		virtual void set_relative_mouse(bool relative) = 0;
		virtual void grab_mouse(bool grab) = 0;
        virtual void set_window_mode(WindowMode mode) = 0;
        virtual void set_window_size(uint width, uint height) = 0;
		void request_shutdown();
        int get_width();
        int get_height();
		int get_drawable_width();
		int get_drawable_height();
        void begin_frame();
        void end_frame();
        double get_delta_time();

#if defined(TERMINUS_OPENGL)
		virtual void create_opengl_context() = 0;
		virtual void destroy_opengl_context() = 0;
		virtual void swap_buffers_opengl() = 0;
#endif
        
#if defined(TERMINUS_PLATFORM_WIN32)
        HWND get_handle_win32();
#endif
      
    public:
        HashMap<uint32_t, MAX_KEYBOARD_INPUTS>     _keyboard_code_map;
        HashMap<uint8_t, MAX_MOUSE_INPUTS>        _mouse_code_map;
        HashMap<uint8_t, MAX_GAMEPAD_INPUTS>      _gamepad_code_map;
        
    protected:
        int					   _width;
        int					   _height;
		int                    _drawable_width;
		int                    _drawable_height;
        WindowMode             _window_mode;
        bool				   _vsync;
        int					   _refresh_rate;
        bool                   _resizable;
        StringBuffer64         _title;
        std::mutex             _mutex;
        double                 _delta;
        Timer                  _timer;
#if defined(TERMINUS_PLATFORM_WIN32)
		HWND				   _handle;
#endif
    };
    
    namespace platform
    {
        extern Selection show_message_box(String _Message, String _Title, Style _Style = Style::Info, Buttons _Buttons = Buttons::OK);
        extern String open_file_dialog(String _Extensions, String _DefaultPath = "");
        extern String save_file_dialog(String _Extensions, String _DefaultPath = "");
    }
}
