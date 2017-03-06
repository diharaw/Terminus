#include <iostream>
#include <input/input_handler.h>
#include <core/context.h>
#include <core/config.h>
#include <graphics/imgui_backend.h>
#include <io/filesystem.h>

#include "platform_sdl2.h"
#include "../graphics/imgui_backend_sdl2.h"

#include <string>
#include <SDL_syswm.h>
#include <SDL_mouse.h>

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
    
    void PlatformSDL2::create_input_map()
    {
        // setup keyboard codes
        _keyboard_code_map.set(HASH("A"), SDLK_a);
        _keyboard_code_map.set(HASH("B"), SDLK_b);
        _keyboard_code_map.set(HASH("C"), SDLK_c);
        _keyboard_code_map.set(HASH("D"), SDLK_d);
        _keyboard_code_map.set(HASH("E"), SDLK_e);
        _keyboard_code_map.set(HASH("F"), SDLK_f);
        _keyboard_code_map.set(HASH("G"), SDLK_g);
        _keyboard_code_map.set(HASH("H"), SDLK_h);
        _keyboard_code_map.set(HASH("I"), SDLK_i);
        _keyboard_code_map.set(HASH("J"), SDLK_j);
        _keyboard_code_map.set(HASH("K"), SDLK_k);
        _keyboard_code_map.set(HASH("L"), SDLK_l);
        _keyboard_code_map.set(HASH("M"), SDLK_m);
        _keyboard_code_map.set(HASH("N"), SDLK_n);
        _keyboard_code_map.set(HASH("O"), SDLK_o);
        _keyboard_code_map.set(HASH("P"), SDLK_p);
        _keyboard_code_map.set(HASH("Q"), SDLK_q);
        _keyboard_code_map.set(HASH("R"), SDLK_r);
        _keyboard_code_map.set(HASH("S"), SDLK_s);
        _keyboard_code_map.set(HASH("T"), SDLK_t);
        _keyboard_code_map.set(HASH("U"), SDLK_u);
        _keyboard_code_map.set(HASH("V"), SDLK_v);
        _keyboard_code_map.set(HASH("W"), SDLK_w);
        _keyboard_code_map.set(HASH("X"), SDLK_x);
        _keyboard_code_map.set(HASH("Y"), SDLK_y);
        _keyboard_code_map.set(HASH("Z"), SDLK_z);
        _keyboard_code_map.set(HASH("1"), SDLK_1);
        _keyboard_code_map.set(HASH("2"), SDLK_2);
        _keyboard_code_map.set(HASH("3"), SDLK_3);
        _keyboard_code_map.set(HASH("4"), SDLK_4);
        _keyboard_code_map.set(HASH("5"), SDLK_5);
        _keyboard_code_map.set(HASH("6"), SDLK_6);
        _keyboard_code_map.set(HASH("7"), SDLK_7);
        _keyboard_code_map.set(HASH("8"), SDLK_8);
        _keyboard_code_map.set(HASH("9"), SDLK_9);
        _keyboard_code_map.set(HASH("0"), SDLK_0);
        _keyboard_code_map.set(HASH("RETURN"), SDLK_RETURN);
        _keyboard_code_map.set(HASH("ESCAPE"), SDLK_ESCAPE);
        _keyboard_code_map.set(HASH("BACKSPACE"), SDLK_BACKSPACE);
        _keyboard_code_map.set(HASH("TAB"), SDLK_TAB);
        _keyboard_code_map.set(HASH("SPACE"), SDLK_SPACE);
        _keyboard_code_map.set(HASH("LSHIFT"), SDLK_LSHIFT);
        _keyboard_code_map.set(HASH("RSHIFR"), SDLK_RSHIFT);
        _keyboard_code_map.set(HASH("LCTRL"), SDLK_LCTRL);
        _keyboard_code_map.set(HASH("RCTRL"), SDLK_RCTRL);
        _keyboard_code_map.set(HASH("INSERT"), SDLK_INSERT);
        _keyboard_code_map.set(HASH("HOME"), SDLK_HOME);
        _keyboard_code_map.set(HASH("PAGEUP"), SDLK_PAGEUP);
        _keyboard_code_map.set(HASH("DELETE"), SDLK_DELETE);
        _keyboard_code_map.set(HASH("END"), SDLK_END);
        _keyboard_code_map.set(HASH("PAGEDOWN"), SDLK_PAGEUP);
        _keyboard_code_map.set(HASH("RIGHT"), SDLK_RIGHT);
        _keyboard_code_map.set(HASH("LEFT"), SDLK_LEFT);
        _keyboard_code_map.set(HASH("UP"), SDLK_UP);
        _keyboard_code_map.set(HASH("DOWN"), SDLK_DOWN);
        _keyboard_code_map.set(HASH("KP_NUMLOCK"), SDLK_NUMLOCKCLEAR);
        _keyboard_code_map.set(HASH("KP_DIVIDE"), SDLK_KP_DIVIDE);
        _keyboard_code_map.set(HASH("KP_MULTIPLY"), SDLK_KP_MULTIPLY);
        _keyboard_code_map.set(HASH("KP_MINUS"), SDLK_KP_MINUS);
        _keyboard_code_map.set(HASH("KP_PLUS"), SDLK_KP_PLUS);
        _keyboard_code_map.set(HASH("KP_ENTER"), SDLK_KP_ENTER);
        _keyboard_code_map.set(HASH("KP_1"), SDLK_KP_1);
        _keyboard_code_map.set(HASH("KP_2"), SDLK_KP_2);
        _keyboard_code_map.set(HASH("KP_3"), SDLK_KP_3);
        _keyboard_code_map.set(HASH("KP_4"), SDLK_KP_4);
        _keyboard_code_map.set(HASH("KP_5"), SDLK_KP_5);
        _keyboard_code_map.set(HASH("KP_6"), SDLK_KP_6);
        _keyboard_code_map.set(HASH("KP_7"), SDLK_KP_7);
        _keyboard_code_map.set(HASH("KP_8"), SDLK_KP_8);
        _keyboard_code_map.set(HASH("KP_9"), SDLK_KP_9);
        _keyboard_code_map.set(HASH("KP_0"), SDLK_KP_0);
        _keyboard_code_map.set(HASH("KP_PERIOD"), SDLK_KP_PERIOD);
        
        // mouse codes
        _mouse_code_map.set(HASH("MOUSE_LEFT"), SDL_BUTTON_LEFT);
        _mouse_code_map.set(HASH("MOUSE_RIGHT"), SDL_BUTTON_RIGHT);
        _mouse_code_map.set(HASH("MOUSE_MIDDLE"), SDL_BUTTON_MIDDLE);
        _mouse_code_map.set(HASH("MOUSE_AXIS_X"), MOUSE_AXIS_X);
        _mouse_code_map.set(HASH("MOUSE_AXIS_Y"), MOUSE_AXIS_Y);
        _mouse_code_map.set(HASH("MOUSE_WHEEL"), MOUSE_WHEEL);
        
        // gamepad codes
        _gamepad_code_map.set(HASH("GAMEPAD_X"), SDL_CONTROLLER_BUTTON_X);
        _gamepad_code_map.set(HASH("GAMEPAD_Y"), SDL_CONTROLLER_BUTTON_Y);
        _gamepad_code_map.set(HASH("GAMEPAD_A"), SDL_CONTROLLER_BUTTON_A);
        _gamepad_code_map.set(HASH("GAMEPAD_B"), SDL_CONTROLLER_BUTTON_B);
        _gamepad_code_map.set(HASH("GAMEPAD_LEFT_X"), SDL_CONTROLLER_AXIS_LEFTX);
        _gamepad_code_map.set(HASH("GAMEPAD_LEFT_Y"), SDL_CONTROLLER_AXIS_LEFTY);
        _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_X"), SDL_CONTROLLER_AXIS_RIGHTX);
        _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_Y"), SDL_CONTROLLER_AXIS_RIGHTY);
        _gamepad_code_map.set(HASH("GAMEPAD_DPAD_LEFT"), SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        _gamepad_code_map.set(HASH("GAMEPAD_DPAD_RIGHT"), SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        _gamepad_code_map.set(HASH("GAMEPAD_DPAD_UP"), SDL_CONTROLLER_BUTTON_DPAD_UP);
        _gamepad_code_map.set(HASH("GAMEPAD_DPAD_DOWN"), SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        _gamepad_code_map.set(HASH("GAMEPAD_LEFT_STICK"), SDL_CONTROLLER_BUTTON_LEFTSTICK);
        _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_STICK"), SDL_CONTROLLER_BUTTON_RIGHTSTICK);
        _gamepad_code_map.set(HASH("GAMEPAD_LEFT_SHOULDER"), SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_SHOULDER"), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        _gamepad_code_map.set(HASH("GAMEPAD_TRIGGER_LEFT"), SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        _gamepad_code_map.set(HASH("GAMEPAD_TRIGGER_RIGHT"), SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
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
        
        create_input_map();

		input_handler::MouseDevice& device = input_handler::get_mouse_device();
		SDL_GetMouseState(&device.x_position, &device.y_position);

#if defined(TERMINUS_WIN32)
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(_window, &wmInfo);
		_handle = wmInfo.info.win.window;
#endif
        imgui_backend_sdl2::set_window(_window);

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
            imgui_backend_sdl2::process_window_events(&event);

			switch (event.type)
			{
                case SDL_MOUSEWHEEL:
                    input_handler::process_mouse_wheel_input(event.wheel.y);
                    break;
                    
                case SDL_MOUSEMOTION:
                {
                    SDL_bool relative = SDL_GetRelativeMouseMode();
                    input_handler::process_cursor_input(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, relative);
                    break;
                }
                    
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                {
                    InputActionType type;
                    
                    if(event.type == SDL_MOUSEBUTTONDOWN)
                        type = InputActionType::MOUSE_BUTTON_DOWN;
                    if(event.type == SDL_MOUSEBUTTONUP)
                        type = InputActionType::MOUSE_BUTTON_UP;

                    input_handler::process_mouse_button_input(event.button.button, type);
                    break;
                }
                    
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                {
                    if(event.key.repeat == 0)
                    {
                        InputActionType type;
                        
                        if(event.type == SDL_KEYDOWN)
                            type = InputActionType::KEY_DOWN;
                        if(event.type == SDL_KEYUP)
                            type = InputActionType::KEY_UP;
                        
                        input_handler::process_keyboard_input(event.key.keysym.sym, event.key.keysym.scancode, type);
                    }
                        
                        
                    break;
                }
                    
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
