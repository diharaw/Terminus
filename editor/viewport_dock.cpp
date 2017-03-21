#include "viewport_dock.h"

#include <QKeyEvent>
#include <QtDebug>
#include <input/input_handler.h>

namespace terminus
{
	ViewportDock::ViewportDock(const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
	{
		setMouseTracking(true);
		//this->installEventFilter(new KeyEventFilter(this));
	}

	ViewportDock::~ViewportDock()
	{

	}

	void ViewportDock::keyPressEvent(QKeyEvent* e)
	{
		if (!e->isAutoRepeat())
		{
			input_handler::process_keyboard_input(e->nativeVirtualKey(), e->nativeScanCode(), InputActionType::KEY_DOWN);
		}
	}

	void ViewportDock::keyReleaseEvent(QKeyEvent *e)
	{
		if (!e->isAutoRepeat())
		{
			input_handler::process_keyboard_input(e->nativeVirtualKey(), e->nativeScanCode(), InputActionType::KEY_UP);
		}
	}

	void ViewportDock::mouseMoveEvent(QMouseEvent *event)
	{
		input_handler::process_cursor_input(event->pos().x, event->pos().y, event->pos().x, event->pos().y, false);
	}

	void ViewportDock::mousePressEvent(QMouseEvent *eventPress)
	{
		input_handler::process_mouse_button_input(eventPress->button(), InputActionType::MOUSE_BUTTON_DOWN);
	}

	void ViewportDock::mouseReleaseEvent(QMouseEvent *releaseEvent)
	{
		input_handler::process_mouse_button_input(releaseEvent->button(), InputActionType::MOUSE_BUTTON_UP);
	}
}