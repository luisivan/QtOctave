#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "process.h"
#include <QScrollArea>

class Window:public Widget
{
	Q_OBJECT
	public:
	QWidget *scroll;
	public slots:
	void close_signal_callback();
};

class WsWindow:public QWidget
{
	Q_OBJECT
	private:
	Window *w;
	public:
	WsWindow(Window *w, QWidget *parent=0);
	protected:
	void closeEvent ( QCloseEvent * event );
	void hideEvent ( QHideEvent * event );
	signals:
	void close_signal();
};

#endif
