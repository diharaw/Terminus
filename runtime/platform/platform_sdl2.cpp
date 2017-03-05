#include <iostream>
#include <input/input_handler.h>
#include <core/context.h>
#include <core/config.h>
#include <graphics/imgui_backend.h>
#include <io/filesystem.h>

#include "platform_sdl2.h"

#include <string>
#include <SDL_syswm.h>

#ifdef TERMINUS_OPENGL
	#ifdef __APPLE__
		#define GL_MAX_MAJOR_VERSION 4
		#define GL_MAX_MINOR_VERSION 1
	#else
		#define GL_MAX_MAJOR_VERSION 4
		#define GL_MAX_MINOR_VERSION 5
	#endif
#endif

namespace terminus
{
	PlatformSDL2::PlatformSDL2()
	{

	}

	PlatformSDL2::~PlatformSDL2()
	{

	}

	bool PlatformSDL2::create_platform_window()
	{
		Uint32 window_flags = 0;

		if (_resizable)
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
			window_flags | SDL_WINDOW_ALLOW_HIGHDPI);
		if (!_window)
			return false;


		return true;
	}

	bool PlatformSDL2::initialize()
	{
		Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK;

		if (SDL_Init(flags) != 0)
			return false;

		ConfigFile* config = context::get_engine_config();

		_width = config->get_int(CONFIG_WINDOW_WIDTH, 1280);
		_height = config->get_int(CONFIG_WINDOW_HEIGHT, 720);
		_refresh_rate = config->get_int(CONFIG_WINDOW_REFRESH_RATE, 60);
		_resizable = config->get_bool(CONFIG_WINDOW_RESIZABLE, false);
		_vsync = config->get_bool(CONFIG_WINDOW_VSYNC, false);

		StringBuffer64 mode = config->get_string(CONFIG_WINDOW_MODE, "windowed");

		if (mode == "fullscreen")
			_window_mode = WindowMode::FULLSCREEN;
		else if (mode == "borderless")
			_window_mode = WindowMode::BORDERLESS_WINDOW;
		else
			_window_mode = WindowMode::WINDOWED;

		_title = config->get_string(CONFIG_WINDOW_TITLE, "Terminus App");

		if (config->has_value(CONFIG_APP_VERSION))
		{
			_title += " ";
			_title += config->get_string(CONFIG_APP_VERSION);
		}

		if (config->has_value(CONFIG_ENGINE_VERSION))
		{
			_title += " - Terminus Engine ";
			_title += config->get_string(CONFIG_ENGINE_VERSION);
		}

		create_platform_window();

		_delta = 0;

		if (!_window)
		{
			SDL_Quit();
			return false;
		}

		// Poll input once to set initial mouse position
		update();

		input_handler::MouseDevice& device = input_handler::get_mouse_device();
		SDL_GetMouseState(&device.x_position, &device.y_position);

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(_window, &wmInfo);
		_handle = wmInfo.info.win.window;

		return true;
	}

	void PlatformSDL2::shutdown()
	{
		SDL_DestroyWindow(_window);
	}

	void PlatformSDL2::update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			terminus::ImGuiBackend::process_window_events(&event);
			input_handler::process_window_events(event);

			switch (event.type)
			{
			case SDL_QUIT:
				global::get_context()._shutdown = true;
				break;

			default:
				break;
			}
		}
	}

	void PlatformSDL2::set_window_mode(WindowMode mode)
	{
		_window_mode = mode;
		create_platform_window();
	}

	void PlatformSDL2::set_window_size(uint width, uint height)
	{
		_width = width;
		_height = height;

		SDL_SetWindowSize(_window, width, height);
	}

	void PlatformSDL2::set_relative_mouse(bool relative)
	{
		if (relative)
			SDL_SetRelativeMouseMode(SDL_TRUE);
		else
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);

			//input_handler::MouseDevice& mouse_device = input_handler::get_mouse_device();
			//SDL_WarpMouseInWindow(_window, mouse_device.x_position, mouse_device.y_position);
		}
	}

	void PlatformSDL2::grab_mouse(bool grab)
	{
		if (grab)
			SDL_CaptureMouse(SDL_TRUE);
		else
			SDL_CaptureMouse(SDL_FALSE);
	}

	void PlatformSDL2::set_cursor_visibility(bool visible)
	{
		SDL_ShowCursor(visible);
	}

#if defined(TERMINUS_OPENGL)
	void PlatformSDL2::create_opengl_context()
	{
#ifdef __APPLE__
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAX_MAJOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MAX_MINOR_VERSION);

		_gl_context = SDL_GL_CreateContext(_window);
		SDL_GL_MakeCurrent(_window, _gl_context);
		SDL_GL_GetDrawableSize(_window, &_drawable_width, &_drawable_height);
	}

	void PlatformSDL2::destroy_opengl_context()
	{
		SDL_GL_DeleteContext(_gl_context);
	}

	void PlatformSDL2::swap_buffers_opengl()
	{
		SDL_GL_SwapWindow(_window);
	}
#endif
} // namespace terminus
