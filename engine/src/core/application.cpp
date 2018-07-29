#include <core/application.hpp>
#include <core/engine_core.hpp>
#include <SDL.h>
#include <SDL_syswm.h>
#include <string.h>
#include <io/logger.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// -----------------------------------------------------------------------------------------------------------------------------------

Application::Application()
{
	m_width = 640;
	m_height = 480;
}

// -----------------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
    
}

// -----------------------------------------------------------------------------------------------------------------------------------

int Application::run(int argc, char *argv[])
{
	// Initialize engine
	if (!initialize_engine())
		return 1;

	// Initialize game
	if (!initialize())
		return 1;

	// Main loop
	while (m_running)
	{
		// Pre-game simulation update
		pre_update_engine();
		// Game simulation
		update();
		// Post-game simulation update
		post_update_engine();
	}

	// Shutdown game
	shutdown();
	// Shutdown engine
	shutdown_engine();

	return 0;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool Application::initialize_engine()
{
	logger::initialize();

	logger::open_console_stream();
	logger::open_file_stream();

    uint32_t window_flags = 0;

#if defined(TE_GFX_BACKEND_GL)
	window_flags |= SDL_WINDOW_OPENGL;
#elif defined(TE_GFX_BACKEND_VK)
	window_flags |= SDL_WINDOW_VULKAN;
#endif

#if defined(TERMINUS_PLATFORM_MACOS)
	window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
#endif
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_RESIZABLE, m_window_flags))
        window_flags |= SDL_WINDOW_RESIZABLE;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_FULLSCREEN, m_window_flags))
        window_flags |= SDL_WINDOW_FULLSCREEN;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_BORDERLESS, m_window_flags))
        window_flags |= SDL_WINDOW_BORDERLESS;
    
	m_x_pos = SDL_WINDOWPOS_CENTERED;
	m_y_pos = SDL_WINDOWPOS_CENTERED;
    
    m_sdl_window = SDL_CreateWindow(m_title.c_str(),
                                    m_x_pos,
                                    m_y_pos,
                                    m_width,
                                    m_height,
                                    window_flags);
    if (!m_sdl_window)
        return false;
    
    int dw = 0;
    int dh = 0;
    
    SDL_GL_GetDrawableSize(m_sdl_window, &dw, &dh);
    
    m_drawable_width = dw;
    m_drawable_height = dh;
    m_running = true;

	// Initialize engine core
	global::initialize_engine_core(this);
	// Initial graphics device
	global::gfx_device().initialize();
	// Initialize input manager Joystick state
	global::input_manager().initialize();
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::pre_update_engine()
{
	InputManager& input_manager = global::input_manager();

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			// --------------------------------------------------------------------------------
			// QUIT
			// --------------------------------------------------------------------------------
			case SDL_QUIT:
			{
				m_running = false;
			}
			// --------------------------------------------------------------------------------
			// MOUSE
			// --------------------------------------------------------------------------------
			case SDL_MOUSEWHEEL:
			{
				input_manager.process_mouse_wheel(e.wheel.y);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				SDL_bool relative = SDL_GetRelativeMouseMode();
				input_manager.process_mouse_motion(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel, relative);
			}
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
					input_manager.process_mouse_button(e.button.button, TE_BUTTON_STATE_PRESSED);
				else
					input_manager.process_mouse_button(e.button.button, TE_BUTTON_STATE_RELEASED);

				break;
			}
			// --------------------------------------------------------------------------------
			// KEYBOARD
			// --------------------------------------------------------------------------------
			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{
				if (e.key.repeat == 0)
				{
					if (e.type == SDL_KEYUP)
						input_manager.process_keyboard_button(e.key.keysym.scancode, TE_BUTTON_STATE_RELEASED);
					else
						input_manager.process_keyboard_button(e.key.keysym.scancode, TE_BUTTON_STATE_PRESSED);
				}
			}
			// --------------------------------------------------------------------------------
			// CONTROLLER
			// --------------------------------------------------------------------------------
			case SDL_CONTROLLERAXISMOTION:
			{

			}
			case SDL_CONTROLLERBUTTONUP:
			case SDL_CONTROLLERBUTTONDOWN:
			{

			}
			case SDL_CONTROLLERDEVICEADDED:
			{

			}
			case SDL_CONTROLLERDEVICEREMOVED:
			{

			}
			// --------------------------------------------------------------------------------
			// JOYSTICK
			// --------------------------------------------------------------------------------
			case SDL_JOYAXISMOTION:
			{

			}
			case SDL_JOYBALLMOTION:
			{

			}
			case SDL_JOYHATMOTION:
			{

			}
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{

			}
			case SDL_JOYDEVICEADDED:
			{

			}
			case SDL_JOYDEVICEREMOVED:
			{

			}
			// --------------------------------------------------------------------------------
			// TOUCH
			// --------------------------------------------------------------------------------
			case SDL_FINGERMOTION:
			{

			}
			case SDL_FINGERUP:
			case SDL_FINGERDOWN:
			{

			}
			case SDL_MULTIGESTURE:
			{

			}
			// --------------------------------------------------------------------------------
			// WINDOW
			// --------------------------------------------------------------------------------
			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					global::gfx_device().recreate_swap_chain();
				}
			}
			// --------------------------------------------------------------------------------
			// TEXT
			// --------------------------------------------------------------------------------
			case SDL_TEXTEDITING:
			{

			}
			case SDL_TEXTINPUT:
			{

			}
			// --------------------------------------------------------------------------------
			// DROP
			// --------------------------------------------------------------------------------
			case SDL_DROPFILE:
			{

			}
			case SDL_DROPTEXT:
			{

			}
			case SDL_DROPBEGIN:
			{

			}
			case SDL_DROPCOMPLETE:
			{

			}
		}
	}

	global::event_manager().process_events();
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::post_update_engine()
{

}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::shutdown_engine()
{
	// Shutdown graphics device
	global::gfx_device().shutdown();

	if (m_sdl_window)
	{
		SDL_DestroyWindow(m_sdl_window);
		m_sdl_window = nullptr;
	}

	// Shutdown engine core
	global::shutdown_engine_core();
	// Shutdown SDL
	SDL_Quit();	
	// Close logger streams
	logger::close_file_stream();
	logger::close_console_stream();
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::hide()
{
	SDL_HideWindow(m_sdl_window);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::show()
{
	SDL_ShowWindow(m_sdl_window);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::hide_cursor()
{
    SDL_ShowCursor(SDL_FALSE);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::show_cursor()
{
    SDL_ShowCursor(SDL_TRUE);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void* Application::handle()
{
    return (void*)m_sdl_window;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void* Application::native_handle()
{
    SDL_SysWMinfo info;
    SDL_GetWindowWMInfo(m_sdl_window, &info);
    
#if defined(TERMINUS_PLATFORM_WIN32)
    return (void*)info.info.win.window;
#elif defined(TERMINUS_PLATFORM_MACOS)
    return (void*)info.info.cocoa.window;
#elif defined(TERMINUS_PLATFORM_LINUX)
    return (void*)info.info.x11.window;
#else
	return nullptr;
#endif
}

// -----------------------------------------------------------------------------------------------------------------------------------

StringBuffer64 Application::title()
{
    return m_title;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::set_title(const StringBuffer64& title)
{
	m_title = title;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool Application::shown()
{
    return TE_HAS_BIT_FLAG(SDL_WINDOW_SHOWN, SDL_GetWindowFlags(m_sdl_window));
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Application::exit()
{
	m_running = false;
}

// -----------------------------------------------------------------------------------------------------------------------------------

bool Application::is_running()
{
	return m_running;
}

// -----------------------------------------------------------------------------------------------------------------------------------

uint32_t Application::width()
{
	return m_width;
}

// -----------------------------------------------------------------------------------------------------------------------------------

uint32_t Application::height()
{
	return m_height;
}

// -----------------------------------------------------------------------------------------------------------------------------------

uint32_t Application::drawable_width()
{
	return m_drawable_width;
}

// -----------------------------------------------------------------------------------------------------------------------------------

uint32_t Application::drawable_height()
{
	return m_drawable_height;
}

// -----------------------------------------------------------------------------------------------------------------------------------

TE_END_TERMINUS_NAMESPACE