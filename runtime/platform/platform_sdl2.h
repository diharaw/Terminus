#pragma once

#include <platform/platform.h>
#include <SDL.h>

namespace terminus
{
	class PlatformSDL2 : public Platform
	{
	public:
		PlatformSDL2();
		virtual ~PlatformSDL2();
		virtual bool initialize() override;
		virtual void shutdown() override;
		virtual void update() override;
		virtual void set_cursor_visibility(bool visible) override;
		virtual void set_relative_mouse(bool relative) override;
		virtual void grab_mouse(bool grab) override;
		virtual void set_window_mode(WindowMode mode) override;
		virtual void set_window_size(uint width, uint height) override;

#if defined(TERMINUS_OPENGL)
		virtual void create_opengl_context() override;
		virtual void destroy_opengl_context() override;
		virtual void swap_buffers_opengl() override;
#endif

	private:
		bool create_platform_window();
        void create_input_map();

	private:
		SDL_Window*   _window;

#if defined(TERMINUS_OPENGL)
		SDL_GLContext _gl_context;
#endif
	};
}
