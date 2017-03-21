#include "platform_qt5.h"
#include <qnamespace.h>
#include <utility/murmur_hash.h>
#include <input/input_handler.h>

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
	PlatformQT5::PlatformQT5()
	{

	}

	PlatformQT5::~PlatformQT5()
	{

	}

	bool PlatformQT5::initialize()
	{
		int argc = 0;
		_app = new QApplication(argc, nullptr);
		_window = new MainWindow();

		QSettings settings("Terminus", "Terminus-Editor");
		_window->restoreGeometry(settings.value("geometry").toByteArray());
		_window->restoreState(settings.value("windowState").toByteArray());

		QFile styleFile(":/qdarkstyle/style.qss");
		styleFile.open(QFile::ReadOnly);

		QString style(styleFile.readAll());
		_app->setStyleSheet(style);
		_window->show();
	}

	void PlatformQT5::shutdown()
	{
		QSettings settings("Terminus", "Terminus-Editor");
		settings.setValue("geometry", _window->saveGeometry());
		settings.setValue("windowState", _window->saveState());

		T_SAFE_DELETE(_window);
		T_SAFE_DELETE(_app);
	}

	void PlatformQT5::update()
	{
		_app->processEvents();
	}

	void PlatformQT5::set_cursor_visibility(bool visible)
	{

	}

	void PlatformQT5::set_relative_mouse(bool relative)
	{

	}

	void PlatformQT5::grab_mouse(bool grab)
	{

	}

	void PlatformQT5::set_window_mode(WindowMode mode)
	{

	}

	void PlatformQT5::set_window_size(uint width, uint height)
	{

	}

#if defined(TERMINUS_OPENGL)

	void PlatformQT5::create_opengl_context()
	{

	}

	void PlatformQT5::destroy_opengl_context()
	{

	}

	void PlatformQT5::swap_buffers_opengl()
	{

	}

#endif

	void PlatformQT5::create_input_map()
	{
		// setup keyboard codes
		_keyboard_code_map.set(HASH("A"), Qt::Key::Key_A);
		_keyboard_code_map.set(HASH("B"), Qt::Key::Key_B);
		_keyboard_code_map.set(HASH("C"), Qt::Key::Key_C);
		_keyboard_code_map.set(HASH("D"), Qt::Key::Key_D);
		_keyboard_code_map.set(HASH("E"), Qt::Key::Key_E);
		_keyboard_code_map.set(HASH("F"), Qt::Key::Key_F);
		_keyboard_code_map.set(HASH("G"), Qt::Key::Key_G);
		_keyboard_code_map.set(HASH("H"), Qt::Key::Key_H);
		_keyboard_code_map.set(HASH("I"), Qt::Key::Key_I);
		_keyboard_code_map.set(HASH("J"), Qt::Key::Key_J);
		_keyboard_code_map.set(HASH("K"), Qt::Key::Key_K);
		_keyboard_code_map.set(HASH("L"), Qt::Key::Key_L);
		_keyboard_code_map.set(HASH("M"), Qt::Key::Key_M);
		_keyboard_code_map.set(HASH("N"), Qt::Key::Key_N);
		_keyboard_code_map.set(HASH("O"), Qt::Key::Key_O);
		_keyboard_code_map.set(HASH("P"), Qt::Key::Key_P);
		_keyboard_code_map.set(HASH("Q"), Qt::Key::Key_Q);
		_keyboard_code_map.set(HASH("R"), Qt::Key::Key_R);
		_keyboard_code_map.set(HASH("S"), Qt::Key::Key_S);
		_keyboard_code_map.set(HASH("T"), Qt::Key::Key_T);
		_keyboard_code_map.set(HASH("U"), Qt::Key::Key_U);
		_keyboard_code_map.set(HASH("V"), Qt::Key::Key_V);
		_keyboard_code_map.set(HASH("W"), Qt::Key::Key_W);
		_keyboard_code_map.set(HASH("X"), Qt::Key::Key_X);
		_keyboard_code_map.set(HASH("Y"), Qt::Key::Key_Y);
		_keyboard_code_map.set(HASH("Z"), Qt::Key::Key_Z);
		_keyboard_code_map.set(HASH("1"), Qt::Key::Key_1);
		_keyboard_code_map.set(HASH("2"), Qt::Key::Key_2);
		_keyboard_code_map.set(HASH("3"), Qt::Key::Key_3);
		_keyboard_code_map.set(HASH("4"), Qt::Key::Key_4);
		_keyboard_code_map.set(HASH("5"), Qt::Key::Key_5);
		_keyboard_code_map.set(HASH("6"), Qt::Key::Key_6);
		_keyboard_code_map.set(HASH("7"), Qt::Key::Key_7);
		_keyboard_code_map.set(HASH("8"), Qt::Key::Key_8);
		_keyboard_code_map.set(HASH("9"), Qt::Key::Key_9);
		_keyboard_code_map.set(HASH("0"), Qt::Key::Key_0);
		_keyboard_code_map.set(HASH("RETURN"), Qt::Key::Key_Return);
		_keyboard_code_map.set(HASH("ESCAPE"), Qt::Key::Key_Escape);
		_keyboard_code_map.set(HASH("BACKSPACE"), Qt::Key::Key_Backspace);
		_keyboard_code_map.set(HASH("TAB"), Qt::Key::Key_Tab);
		_keyboard_code_map.set(HASH("SPACE"), Qt::Key::Key_Space);
		_keyboard_code_map.set(HASH("LSHIFT"), Qt::Key::Key_Shift);
		_keyboard_code_map.set(HASH("RSHIFR"), Qt::Key::Key_Shift);
		_keyboard_code_map.set(HASH("LCTRL"), Qt::Key::Key_Control);
		_keyboard_code_map.set(HASH("RCTRL"), Qt::Key::Key_Control);
		_keyboard_code_map.set(HASH("INSERT"), Qt::Key::Key_Insert);
		_keyboard_code_map.set(HASH("HOME"), Qt::Key::Key_Home);
		_keyboard_code_map.set(HASH("PAGEUP"), Qt::Key::Key_PageUp);
		_keyboard_code_map.set(HASH("DELETE"), Qt::Key::Key_Delete);
		_keyboard_code_map.set(HASH("END"), Qt::Key::Key_End);
		_keyboard_code_map.set(HASH("PAGEDOWN"), Qt::Key::Key_PageDown);
		_keyboard_code_map.set(HASH("RIGHT"), Qt::Key::Key_Right);
		_keyboard_code_map.set(HASH("LEFT"), Qt::Key::Key_Left);
		_keyboard_code_map.set(HASH("UP"), Qt::Key::Key_Up);
		_keyboard_code_map.set(HASH("DOWN"), Qt::Key::Key_Down);
		_keyboard_code_map.set(HASH("KP_NUMLOCK"), Qt::Key::Key_NumLock);
		_keyboard_code_map.set(HASH("KP_DIVIDE"), Qt::Key::Key_division);
		_keyboard_code_map.set(HASH("KP_MULTIPLY"), Qt::Key::Key_multiply);
		_keyboard_code_map.set(HASH("KP_MINUS"), Qt::Key::Key_Minus);
		_keyboard_code_map.set(HASH("KP_PLUS"), Qt::Key::Key_Plus);
		_keyboard_code_map.set(HASH("KP_ENTER"), Qt::Key::Key_Enter);
		_keyboard_code_map.set(HASH("KP_1"), Qt::Key::Key_1);
		_keyboard_code_map.set(HASH("KP_2"), Qt::Key::Key_2);
		_keyboard_code_map.set(HASH("KP_3"), Qt::Key::Key_3);
		_keyboard_code_map.set(HASH("KP_4"), Qt::Key::Key_4);
		_keyboard_code_map.set(HASH("KP_5"), Qt::Key::Key_5);
		_keyboard_code_map.set(HASH("KP_6"), Qt::Key::Key_6);
		_keyboard_code_map.set(HASH("KP_7"), Qt::Key::Key_7);
		_keyboard_code_map.set(HASH("KP_8"), Qt::Key::Key_8);
		_keyboard_code_map.set(HASH("KP_9"), Qt::Key::Key_9);
		_keyboard_code_map.set(HASH("KP_0"), Qt::Key::Key_0);
		_keyboard_code_map.set(HASH("KP_PERIOD"), Qt::Key::Key_Period);

		// mouse codes
		_mouse_code_map.set(HASH("MOUSE_LEFT"), Qt::MouseButton::LeftButton);
		_mouse_code_map.set(HASH("MOUSE_RIGHT"), Qt::MouseButton::RightButton);
		_mouse_code_map.set(HASH("MOUSE_MIDDLE"), Qt::MouseButton::MiddleButton);
		_mouse_code_map.set(HASH("MOUSE_AXIS_X"), MOUSE_AXIS_X);
		_mouse_code_map.set(HASH("MOUSE_AXIS_Y"), MOUSE_AXIS_Y);
		_mouse_code_map.set(HASH("MOUSE_WHEEL"), MOUSE_WHEEL);
	}
}