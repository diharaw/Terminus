#include <platform/platform.h>
#include <core/context.h>

#if !defined(TERMINUS_PLATFORM_IOS)
    #include <nfd.h>
    #include <boxer/boxer.h>
#endif

namespace terminus
{
	Platform::Platform()
	{

	}

	Platform::~Platform()
	{

	}

	void Platform::request_shutdown()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		global::get_context()._shutdown = true;
	}

	int Platform::get_width()
	{
		return _width;
	}

	int Platform::get_height()
	{
		return _height;
	}

	int Platform::get_drawable_width()
	{
		return _drawable_width;
	}

	int Platform::get_drawable_height()
	{
		return _drawable_height;
	}

	void Platform::begin_frame()
	{
		_timer.start();
	}

	void Platform::end_frame()
	{
		_timer.stop();
		_delta = _timer.get_elapsed_time_sec();
	}

	double Platform::get_delta_time()
	{
		return _delta;
	}

#if defined(TERMINUS_PLATFORM_WIN32)
	HWND Platform::get_handle_win32()
	{
		return _handle;
	}
#endif

    namespace platform
    {
                Selection show_message_box(String _Message, String _Title, Style _Style, Buttons _Buttons)
                {
        #if !defined(TERMINUS_PLATFORM_IOS)
                    Selection result = (Selection)boxer::show(_Message.c_str(), _Title.c_str(), (boxer::Style)_Style, (boxer::Buttons)_Buttons);
                    return result;
        #else
                    return Selection::OK;
        #endif
                }
        
                String open_file_dialog(String _Extensions, String _DefaultPath)
                {
        #if !defined(TERMINUS_PLATFORM_IOS)
                    char* openPath = NULL;
                    nfdresult_t result = NFD_OpenDialog(_Extensions.c_str(), _DefaultPath.c_str(), &openPath);
                    if (result == NFD_OKAY)
                    {
                        return String(openPath);
                    }
                    else
                        return String("");
        #else
                    return String("");
        #endif
                }
        
                String save_file_dialog(String _Extensions, String _DefaultPath)
                {
        #if !defined(TERMINUS_PLATFORM_IOS)
                    char* savePath = NULL;
                    nfdresult_t result = NFD_SaveDialog(_Extensions.c_str(), _DefaultPath.c_str(), &savePath);
                    if (result == NFD_OKAY)
                    {
                        return String(savePath);
                    }
                    else
                        return String("");
        #else
                    return String("");
        #endif
                }
    }
}
