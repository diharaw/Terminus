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

/**
 * Represents the selected button of a displayed Message Box.
 */

enum class Selection
{
    OK,         /**< OK button. */
    Cancel,     /**< Cancel button. */
    Yes,        /**< Yes button. */
    No,         /**< No button. */
    None,       /**< No buttn selected. */
    Error       /**< Error value. */
};

/**
 * Button combinations available for a Message Box.
 */

enum class Buttons
{
    OK,         /**< OK button. */
    OKCancel,   /**< OK and Cancel buttons. */
    YesNo       /**< Yes and No buttons. */
};

/**
 * Message Box styles.
 */

enum class Style
{
    Info,       /**< Informational style. Used for general messages. */
    Warning,    /**< Warning style. */
    Error,      /**< Error style. */
    Question    /**< Question style. Used for prompts. */
};

/**
 * Represents the window mode to be used.
 */

enum class WindowMode
{
    WINDOWED,           /**< Windowed mode. */
    BORDERLESS_WINDOW,  /**< Borderless window mode. Removes the decorator around the window. */
    FULLSCREEN          /**< Fullscreen mode. */
};


namespace terminus
{
	struct PlatformInitDesc
	{
		Vector2			window_rect;
		WindowMode		window_mode;
		bool			vsync;
		bool			resizable;
		StringBuffer128 title;
		uint32_t		refresh_rate;
	};

    /**
     *  Platform base class. Provides the engine with access to window creation, input and other platform specifics.
     *  Must be implemented in order to build engine.
     *  @see PlatformSDL2.
     */
    
    class Platform
    {
    public:
        /**
         * Platform constructor.
         */
		Platform();
        
        /**
         * Platform virtual destructor.
         * A more elaborate description of the constructor.
         */
		virtual ~Platform();
        
        /**
         * Initializes platform. Behind the scenes, it will create the platform window using settings defined in the Config file.
         * Optionally creates an OpenGL context if engine is built targetting the OpenGL graphics API.
         * @return Returns true if initialization is successful. Returns false otherwise.
         */
        virtual bool initialize() = 0;
        
        /**
         * Shuts down platform. Destroys window, OpenGL context.
         */
		virtual void shutdown() = 0;
        
        /**
         * Runs the Window message loop and queues appropriate Events.
         */
        virtual void update() = 0;
        
        /**
         * Set OS cursor visibility.
         * @param bool representing the desired state of cursor visibility.
         */
		virtual void set_cursor_visibility(bool visible) = 0;
        
        /**
         * Set OS cursor relative movement.
         * TODO
         * @param bool representing the desired state of relative cursor movement.
         */
		virtual void set_relative_mouse(bool relative) = 0;
        
        /**
         * Prevent mouse from going beyond the bounds of the window.
         * @param bool representing the desired mouse grab state.
         */
		virtual void grab_mouse(bool grab) = 0;
        
        /**
         * Changes the mode of current window.
         * @param Enum representing the desired Window mode.
         */
        virtual void set_window_mode(WindowMode mode) = 0;
        
        /**
         * Changes the resolution of current window.
         * @param The desired Width of window.
         * @param The desired Height of window.
         */
        virtual void set_window_size(uint width, uint height) = 0;

		/**
		* Changes the title of current window.
		* @param Window title string.
		*/
		virtual void set_window_size(StringBuffer128 title) = 0;
        
        /**
         * Requests platform shutdown. This will cause the main loop to exit upon the start of the next frame.
         */
		void request_shutdown();
        
        /**
         * Returns the Width of the Window rect.
         * @return Width of window rect.
         */
        int get_width();
        
        /**
         * Returns the Height of the Window rect.
         * @return Height of window rect.
         */
        int get_height();
        
        /**
         * Returns the Width of the Drawable rect.
         * @return Width of Drawable rect.
         */
		int get_drawable_width();
        
        /**
         * Returns the height of the Drawable rect.
         * @return height of Drawable rect.
         */
		int get_drawable_height();
        
        /**
         * Performs timer resets and prepares for new frame.
         * Called during the beginning of the frame.
         */
        void begin_frame();
        
        /**
         * Performs delta time calculation.
         * Called during the end of the frame.
         */
        void end_frame();
        
        /**
         * Returns the delta time of last frame.
         * @return Delta time of last frame.
         */
        double get_delta_time();

#if defined(TERMINUS_OPENGL)
        /**
         * Creates OpenGL context. 
         * Only available when built for OpenGL.
         */
		virtual void create_opengl_context() = 0;
        
        /**
         * Destroys OpenGL context.
         * Only available when built for OpenGL.
         */
		virtual void destroy_opengl_context() = 0;
        
        /**
         * Swaps the front and back buffer of the Window.
         * Only available when built for OpenGL.
         */
		virtual void swap_buffers_opengl() = 0;
#endif
        
#if defined(TERMINUS_PLATFORM_WIN32)
        /**
         * Returns the Win32 window handle.
         * @return Win32 HWND handle.
         */
        HWND get_handle_win32();
#endif
      
    public:
        /**
         * A HashMap containing the String hashes of keyboard keys and their platform-independent scancodes.
         */
        HashMap<uint32_t, MAX_KEYBOARD_INPUTS>     _keyboard_code_map;
        
        /**
         * A HashMap containing the String hashes of mouse buttons and axis and their platform-independent codes.
         */
        HashMap<uint8_t, MAX_MOUSE_INPUTS>        _mouse_code_map;
        
        /**
         * A HashMap containing the String hashes of gamepad buttons and axis and their platform-independent codes.
         */
        HashMap<uint8_t, MAX_GAMEPAD_INPUTS>      _gamepad_code_map;
        
    protected:
        
        /**
         * Window rect width.
         */
        int					   _width;
        
        /**
         * Window rect height.
         */
        int					   _height;
        
        /**
         * Drawable rect width.
         */
		int                    _drawable_width;
        
        /**
         * Drawable rect height.
         */
		int                    _drawable_height;
        
        /**
         * Current window mode.
         */
        WindowMode             _window_mode;
        
        /**
         * Vsync enabled flag.
         */
        bool				   _vsync;
        
        /**
         * Current refresh rate of monitor.
         */
        int					   _refresh_rate;
        
        /**
         * Window resize flag.
         */
        bool                   _resizable;
        
        /**
         * Window title.
         */
        StringBuffer128        _title;
        
        /**
         * A mutex. What is this used for again?
         */
        std::mutex             _mutex;
        
        /**
         * Current delta time.
         */
        double                 _delta;
        
        /**
         * Timer instance. Used for getting delta time.
         */
        Timer                  _timer;
#if defined(TERMINUS_PLATFORM_WIN32)
        /**
         * Win32 Window handle.
         */
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
