#include <iostream>
#include <input/input_handler.h>
#include <core/context.h>
#include <core/config.h>
#include <graphics/imgui_backend.h>
#include <io/filesystem.h>

#include <platform/platform_sdl2.h>
#include <graphics/imgui_backend_sdl2.h>

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
        _keyboard_code_map.set(HASH("A"), 					KeyboardButton::A);
        _keyboard_code_map.set(HASH("B"), 					KeyboardButton::B);
        _keyboard_code_map.set(HASH("C"), 					KeyboardButton::C);
        _keyboard_code_map.set(HASH("D"), 					KeyboardButton::D);
        _keyboard_code_map.set(HASH("E"), 					KeyboardButton::E);
        _keyboard_code_map.set(HASH("F"), 					KeyboardButton::F);
        _keyboard_code_map.set(HASH("G"), 					KeyboardButton::G);
        _keyboard_code_map.set(HASH("H"), 					KeyboardButton::H);
        _keyboard_code_map.set(HASH("I"), 					KeyboardButton::I);
        _keyboard_code_map.set(HASH("J"), 					KeyboardButton::J);
        _keyboard_code_map.set(HASH("K"), 					KeyboardButton::K);
        _keyboard_code_map.set(HASH("L"), 					KeyboardButton::L);
        _keyboard_code_map.set(HASH("M"), 					KeyboardButton::M);
        _keyboard_code_map.set(HASH("N"), 					KeyboardButton::N);
        _keyboard_code_map.set(HASH("O"), 					KeyboardButton::O);
        _keyboard_code_map.set(HASH("P"), 					KeyboardButton::P);
        _keyboard_code_map.set(HASH("Q"), 					KeyboardButton::Q);
        _keyboard_code_map.set(HASH("R"), 					KeyboardButton::R);
        _keyboard_code_map.set(HASH("S"), 					KeyboardButton::S);
        _keyboard_code_map.set(HASH("T"), 					KeyboardButton::T);
        _keyboard_code_map.set(HASH("U"), 					KeyboardButton::U);
        _keyboard_code_map.set(HASH("V"), 					KeyboardButton::V);
        _keyboard_code_map.set(HASH("W"), 					KeyboardButton::W);
        _keyboard_code_map.set(HASH("X"), 					KeyboardButton::X);
        _keyboard_code_map.set(HASH("Y"), 					KeyboardButton::Y);
        _keyboard_code_map.set(HASH("Z"), 					KeyboardButton::Z);
        _keyboard_code_map.set(HASH("1"), 					KeyboardButton::NUM_1);
        _keyboard_code_map.set(HASH("2"), 					KeyboardButton::NUM_2);
        _keyboard_code_map.set(HASH("3"), 					KeyboardButton::NUM_3);
        _keyboard_code_map.set(HASH("4"), 					KeyboardButton::NUM_4);
        _keyboard_code_map.set(HASH("5"), 					KeyboardButton::NUM_5);
        _keyboard_code_map.set(HASH("6"), 					KeyboardButton::NUM_6);
        _keyboard_code_map.set(HASH("7"), 					KeyboardButton::NUM_7);
        _keyboard_code_map.set(HASH("8"), 					KeyboardButton::NUM_8);
        _keyboard_code_map.set(HASH("9"), 					KeyboardButton::NUM_9);
        _keyboard_code_map.set(HASH("0"), 					KeyboardButton::NUM_0);
        _keyboard_code_map.set(HASH("RETURN"), 				KeyboardButton::RETURN);
        _keyboard_code_map.set(HASH("ESCAPE"), 				KeyboardButton::ESCAPE);
        _keyboard_code_map.set(HASH("BACKSPACE"), 			KeyboardButton::BACKSPACE);
        _keyboard_code_map.set(HASH("TAB"), 				KeyboardButton::TAB);
        _keyboard_code_map.set(HASH("SPACE"), 				KeyboardButton::SPACE);
        _keyboard_code_map.set(HASH("LSHIFT"), 				KeyboardButton::LSHIFT);
        _keyboard_code_map.set(HASH("RSHIFR"), 				KeyboardButton::RSHIFT);
        _keyboard_code_map.set(HASH("LCTRL"), 				KeyboardButton::LCTRL);
        _keyboard_code_map.set(HASH("RCTRL"), 				KeyboardButton::RCTRL);
        _keyboard_code_map.set(HASH("INSERT"), 				KeyboardButton::INSERT);
        _keyboard_code_map.set(HASH("HOME"), 				KeyboardButton::HOME);
        _keyboard_code_map.set(HASH("PAGEUP"), 				KeyboardButton::PAGEUP);
        _keyboard_code_map.set(HASH("DELETE"), 				KeyboardButton::DELETE);
        _keyboard_code_map.set(HASH("END"), 				KeyboardButton::END);
        _keyboard_code_map.set(HASH("PAGEDOWN"), 			KeyboardButton::PAGEUP);
        _keyboard_code_map.set(HASH("RIGHT"), 				KeyboardButton::RIGHT);
        _keyboard_code_map.set(HASH("LEFT"), 				KeyboardButton::LEFT);
        _keyboard_code_map.set(HASH("UP"), 					KeyboardButton::UP);
        _keyboard_code_map.set(HASH("DOWN"), 				KeyboardButton::DOWN);
        _keyboard_code_map.set(HASH("KP_NUMLOCK"),  		KeyboardButton::NUMLOCKCLEAR);
        _keyboard_code_map.set(HASH("KP_DIVIDE"), 			KeyboardButton::KP_DIVIDE);
        _keyboard_code_map.set(HASH("KP_MULTIPLY"), 		KeyboardButton::KP_MULTIPLY);
        _keyboard_code_map.set(HASH("KP_MINUS"), 			KeyboardButton::KP_MINUS);
        _keyboard_code_map.set(HASH("KP_PLUS"), 			KeyboardButton::KP_PLUS);
        _keyboard_code_map.set(HASH("KP_ENTER"),			KeyboardButton::KP_ENTER);
        _keyboard_code_map.set(HASH("KP_1"), 				KeyboardButton::KP_1);
        _keyboard_code_map.set(HASH("KP_2"), 				KeyboardButton::KP_2);
        _keyboard_code_map.set(HASH("KP_3"), 				KeyboardButton::KP_3);
        _keyboard_code_map.set(HASH("KP_4"), 				KeyboardButton::KP_4);
        _keyboard_code_map.set(HASH("KP_5"), 				KeyboardButton::KP_5);
        _keyboard_code_map.set(HASH("KP_6"), 				KeyboardButton::KP_6);
        _keyboard_code_map.set(HASH("KP_7"), 				KeyboardButton::KP_7);
        _keyboard_code_map.set(HASH("KP_8"), 				KeyboardButton::KP_8);
        _keyboard_code_map.set(HASH("KP_9"), 				KeyboardButton::KP_9);
        _keyboard_code_map.set(HASH("KP_0"), 				KeyboardButton::KP_0);
        _keyboard_code_map.set(HASH("KP_PERIOD"), 			KeyboardButton::KP_PERIOD);
        
        // mouse button codes
        _mouse_code_map.set(HASH("LEFT"), 					MouseButton::LEFT);
        _mouse_code_map.set(HASH("RIGHT"), 					MouseButton::RIGHT);
        _mouse_code_map.set(HASH("MIDDLE"), 				MouseButton::MIDDLE);
        
        // mouse axis codes
        _mouse_code_map.set(HASH("AXIS_X"), 				MouseAxis::X);
        _mouse_code_map.set(HASH("AXIS_Y"), 				MouseAxis::Y);
        _mouse_code_map.set(HASH("WHEEL"), 					MouseAxis::WHEEL);
        
        // gamepad button codes
        _gamepad_code_map.set(HASH("X"), 		   			GamepadButton::X);
        _gamepad_code_map.set(HASH("Y"), 		   			GamepadButton::Y);
        _gamepad_code_map.set(HASH("A"), 		   			GamepadButton::A);
        _gamepad_code_map.set(HASH("B"), 		   			GamepadButton::B);
        _gamepad_code_map.set(HASH("DPAD_LEFT"), 			GamepadButton::DPAD_LEFT);
        _gamepad_code_map.set(HASH("DPAD_RIGHT"), 			GamepadButton::DPAD_RIGHT);
        _gamepad_code_map.set(HASH("DPAD_UP"), 				GamepadButton::DPAD_UP);
        _gamepad_code_map.set(HASH("DPAD_DOWN"), 			GamepadButton::DPAD_DOWN);
        _gamepad_code_map.set(HASH("LEFT_STICK"), 			GamepadButton::LEFT_STICK);
        _gamepad_code_map.set(HASH("RIGHT_STICK"), 			GamepadButton::RIGHT_STICK);
        _gamepad_code_map.set(HASH("LEFT_SHOULDER"), 		GamepadButton::LEFT_SHOULDER);
        _gamepad_code_map.set(HASH("RIGHT_SHOULDER"), 		GamepadButton::RIGHT_SHOULDER);

        // gamepad axis codes
        _gamepad_code_map.set(HASH("AXIS_TRIGGER_LEFT"), 	GamepadAxis::TRIGGER_LEFT);
        _gamepad_code_map.set(HASH("AXIS_TRIGGER_RIGHT"), 	GamepadAxis::TRIGGER_RIGHT);
        _gamepad_code_map.set(HASH("AXIS_LEFT_STICK_X"), 	GamepadAxis::LEFT_STICK_X);
        _gamepad_code_map.set(HASH("AXIS_LEFT_STICK_Y"), 	GamepadAxis::LEFT_STICK_Y);
        _gamepad_code_map.set(HASH("AXIS_RIGHT_STICK_X"),	GamepadAxis::RIGHT_STICK_X);
        _gamepad_code_map.set(HASH("AXIS_RIGHT_STICK_Y"),	GamepadAxis::RIGHT_STICK_Y);
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

#if defined(TERMINUS_PLATFORM_WIN32)
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
                        
                        input_handler::process_keyboard_input(event.key.keysym.scancode, type);
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
