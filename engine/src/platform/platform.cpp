#include <platform/platform.h>
#include <core/context.h>

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

}