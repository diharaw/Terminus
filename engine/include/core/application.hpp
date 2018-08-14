#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <core/platform_macros.hpp>
#include <stl/string_buffer.hpp>

// Forward Declarations
struct SDL_Window;

enum WindowFlags
{
	TE_WINDOW_FULLSCREEN = 0x00000001,
	TE_WINDOW_OPENGL = 0x00000002,
	TE_WINDOW_SHOWN = 0x00000004,
	TE_WINDOW_HIDDEN = 0x00000008,
	TE_WINDOW_BORDERLESS = 0x00000010,
	TE_WINDOW_RESIZABLE = 0x00000020,
	TE_WINDOW_MINIMIZED = 0x00000040,
	TE_WINDOW_MAXIMIZED = 0x00000080,
	TE_WINDOW_INPUT_GRABBED = 0x00000100,
	TE_WINDOW_INPUT_FOCUS = 0x00000200,
	TE_WINDOW_MOUSE_FOCUS = 0x00000400,
	TE_WINDOW_FOREIGN = 0x00000800,
	TE_WINDOW_ALLOW_HIGHDPI = 0x00002000,
	TE_WINDOW_MOUSE_CAPTURE = 0x00004000,
	TE_WINDOW_ALWAYS_ON_TOP = 0x00008000,
	TE_WINDOW_SKIP_TASKBAR = 0x00010000,
	TE_WINDOW_UTILITY = 0x00020000,
	TE_WINDOW_TOOLTIP = 0x00040000,
	TE_WINDOW_POPUP_MENU = 0x00080000,
	TE_WINDOW_VULKAN = 0x00100000
};


TE_BEGIN_TERMINUS_NAMESPACE

class Application
{
protected:
	SDL_Window * m_sdl_window = nullptr;
	uint32_t	   m_width = 800;
	uint32_t	   m_height = 600;
	uint32_t	   m_window_flags = 0;
	uint32_t	   m_drawable_width = 800;
	uint32_t	   m_drawable_height = 600;
	int32_t		   m_x_pos = 0;
	int32_t		   m_y_pos = 0;
	StringBuffer64 m_title = "Terminus Engine";
	uint32_t	   m_flags = 0;
	bool		   m_running = false;

public:
	Application();
	~Application();

	int run(int argc, char *argv[]);

	virtual bool initialize() { return true;  }
	virtual void update() {}
	virtual void shutdown() {}
	virtual void window_resized() {}

	bool		   initialize_engine();
	void		   pre_update_engine();
	void		   post_update_engine();
	void		   window_resized_engine();
	void		   shutdown_engine();
    void		   hide();
    void		   show();
    void		   hide_cursor();
    void		   show_cursor();
    void*		   native_handle();
    void*		   handle();
	StringBuffer64 title();
    void		   set_title(const StringBuffer64& title);
    bool           shown();
	void		   exit();
	bool		   is_running();
	uint32_t	   width();
	uint32_t	   height();
	uint32_t	   drawable_width();
	uint32_t	   drawable_height();
};

TE_END_TERMINUS_NAMESPACE
