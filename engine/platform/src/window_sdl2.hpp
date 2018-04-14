#pragma once

#include <platform/include/window.hpp>

#if defined(TE_PLATFORM_SDL2) || defined(TE_PLATFORM_EMSCRIPTEN)

// Forward Declarations
struct SDL_Window;

enum WindowFlags
{
    TE_WINDOW_FULLSCREEN	  = 0x00000001,
    TE_WINDOW_OPENGL		  = 0x00000002,
    TE_WINDOW_SHOWN		  	  = 0x00000004,
    TE_WINDOW_HIDDEN		  = 0x00000008,
    TE_WINDOW_BORDERLESS	  = 0x00000010,
    TE_WINDOW_RESIZABLE	  	  = 0x00000020,
    TE_WINDOW_MINIMIZED	  	  = 0x00000040,
    TE_WINDOW_MAXIMIZED	  	  = 0x00000080,
    TE_WINDOW_INPUT_GRABBED   = 0x00000100,
    TE_WINDOW_INPUT_FOCUS	  = 0x00000200,
    TE_WINDOW_MOUSE_FOCUS	  = 0x00000400,
    TE_WINDOW_FOREIGN		  = 0x00000800,
    TE_WINDOW_ALLOW_HIGHDPI   = 0x00002000,
    TE_WINDOW_MOUSE_CAPTURE   = 0x00004000,
    TE_WINDOW_ALWAYS_ON_TOP   = 0x00008000,
    TE_WINDOW_SKIP_TASKBAR    = 0x00010000,
    TE_WINDOW_UTILITY		  = 0x00020000,
    TE_WINDOW_TOOLTIP		  = 0x00040000,
    TE_WINDOW_POPUP_MENU	  = 0x00080000,
    TE_WINDOW_VULKAN		  = 0x00100000
};

TE_BEGIN_TERMINUS_NAMESPACE

class Window : public IWindow
{
private:
	SDL_Window* m_sdl_window = nullptr;
    uint32_t	m_window_id = 0;
	uint32_t	m_width = 800;
	uint32_t	m_height = 600;
	uint32_t	m_drawable_width = 800;
	uint32_t	m_drawable_height = 600;
	int32_t		m_x_pos = 0;
	int32_t		m_y_pos = 0;
	char		m_title[TE_WINDOW_TITLE_LENGTH] = "Terminus Engine";
	uint32_t    m_flags = 0;
	bool		m_running = false;
    bool		m_shown = false;
    bool		m_minimized = false;
    bool		m_mouse_focus = false;
    bool		m_keyboard_focus = false;
    
public:
    Window();
    ~Window();
    bool        initialize(WindowCreateDesc& desc) override;
    void        shutdown() override;
    void        hide() override;
    void        show() override;
    uint32_t    id() override;
    void        hide_cursor() override;
    void        show_cursor() override;
    void*       native_handle() override;
    void*       handle() override;
    void        resize(uint32_t w, uint32_t h) override;
    void        make_windowed() override;
    void        make_fullscreen() override;
    void        remove_border() override;
    void        add_border() override;
    const char* title() override;
    void        set_title(const char* title) override;
    bool        shown() override;
};

TE_END_TERMINUS_NAMESPACE

#endif
