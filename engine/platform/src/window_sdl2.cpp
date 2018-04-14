#if defined(TE_PLATFORM_SDL2) || defined(TE_PLATFORM_EMSCRIPTEN)

#include <platform/src/window_sdl2.hpp>
#include <SDL.h>
#include <SDL_syswm.h>
#include <string.h>

TE_BEGIN_TERMINUS_NAMESPACE

Window::Window()
{
    
}

Window::~Window()
{
    
}

bool Window::initialize(WindowCreateDesc& desc)
{
    m_width = desc.w;
    m_height = desc.h;
    
    uint32_t window_flags = 0;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_RESIZABLE, desc.flags))
        window_flags = SDL_WINDOW_RESIZABLE;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_FULLSCREEN, desc.flags))
        window_flags |= SDL_WINDOW_FULLSCREEN;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_BORDERLESS, desc.flags))
        window_flags |= SDL_WINDOW_BORDERLESS;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_OPENGL, desc.flags))
        window_flags |= SDL_WINDOW_OPENGL;
    
    if (TE_HAS_BIT_FLAG(TE_WINDOW_ALLOW_HIGHDPI, desc.flags))
        window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    
    if (desc.x == TE_WINDOW_POS_CENTERED)
        m_x_pos = SDL_WINDOWPOS_CENTERED;
    
    if (desc.y == TE_WINDOW_POS_CENTERED)
        m_y_pos = SDL_WINDOWPOS_CENTERED;
    
    set_title(&desc.title[0]);
    
    m_sdl_window = SDL_CreateWindow(&m_title[0],
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
    m_keyboard_focus = true;
    m_mouse_focus = true;
    m_shown = true;
    m_minimized = false;
    m_window_id = SDL_GetWindowID(m_sdl_window);
    
    return true;
}

void Window::shutdown()
{
    if(m_sdl_window)
    {
        SDL_DestroyWindow(m_sdl_window);
        m_sdl_window = nullptr;
    }
}

void Window::hide()
{
    
}

void Window::show()
{
    
}

uint32_t Window::id()
{
    return m_window_id;
}

void Window::hide_cursor()
{
    SDL_ShowCursor(SDL_FALSE);
}

void Window::show_cursor()
{
    SDL_ShowCursor(SDL_TRUE);
}

void* Window::handle()
{
    return (void*)m_sdl_window;
}

void* Window::native_handle()
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

void Window::resize(uint32_t w, uint32_t h)
{
    m_width = w;
    m_height = h;
    
    SDL_SetWindowSize(m_sdl_window, m_width, m_height);
}

const char* Window::title()
{
    return &m_title[0];
}

void Window::set_title(const char* title)
{
    if (title)
        strcpy(&m_title[0], title);
}

void Window::make_windowed()
{
    m_flags |= 0;
}

void Window::make_fullscreen()
{
    m_flags |= SDL_WINDOW_FULLSCREEN;
}

void Window::remove_border()
{
    
}

void Window::add_border()
{
    
}

bool Window::shown()
{
    return m_shown;
}

TE_END_TERMINUS_NAMESPACE

#endif
