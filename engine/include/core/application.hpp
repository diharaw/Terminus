#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <core/platform_macros.hpp>
#include <stl/string_buffer.hpp>

// Forward Declarations
struct SDL_Window;

TE_BEGIN_TERMINUS_NAMESPACE

class Application
{
protected:
	SDL_Window*	   m_sdl_window = nullptr;
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

    virtual bool initialize() = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;

	bool		   initialize_engine();
	void		   pre_update_engine();
	void		   post_update_engine();
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
