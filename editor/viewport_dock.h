#pragma once

#include <QDockWidget>

namespace terminus
{
	class ViewportDock : public QDockWidget
	{
	public:
		ViewportDock(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
		~ViewportDock();
		void keyReleaseEvent(QKeyEvent *);
		void keyPressEvent(QKeyEvent* e);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *eventPress);
		void mouseReleaseEvent(QMouseEvent *releaseEvent);
	};
}
