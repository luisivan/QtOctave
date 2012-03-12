#include "window.h"

WsWindow::WsWindow(Window *w, QWidget *parent):QWidget(parent)
{
	this->w=w;
	connect(this, SIGNAL(close_signal()), w, SLOT(close_signal_callback()) );
}

void WsWindow::closeEvent ( QCloseEvent * event )
{
	emit close_signal();
}

void WsWindow::hideEvent ( QHideEvent * event )
{
	emit close_signal();
}

void Window::close_signal_callback()
{
	emit event("*close: "+name+"\n");
}
