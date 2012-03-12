/* Copyright (C) 2007 P.L. Lucas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */

#include "window.h"

Widget *new_window(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "window") return NULL;
	
	Window *w=new Window();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=true;
	w->widget=new mWindow();
	
	return (Widget *)w;
}

Window::Window():Widget()
{
}

bool Window::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando window\n");
	if(qName=="window")
	{
		QString title=atts.value("title");
		if(!title.isEmpty()) widget->setWindowTitle ( atts.value("title") );
		
		QString maximize=atts.value("maximize");
		if( !maximize.isEmpty() )
		{
			if(maximize=="true") ((mWindow*)widget)->window_maximized_ok=true;
			else ((mWindow*)widget)->window_maximized_ok=false;
		}
		
		QString minimize=atts.value("minimize");
		if( !minimize.isEmpty() )
		{
			if(minimize=="true") widget->showMinimized();
			else widget->showNormal();
		}
		
		widget_properties(atts,this);
		
		return true;
	}
	
	return false;
}

bool Window::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(((mWindow*)widget)->window_maximized_ok) ((mWindow*)widget)->showMaximized();
	else ((mWindow*)widget)->showNormal();
	property_ok=false;
	return true;
}

#include <QVBoxLayout>

void Window::add_widget(Widget *w)
{
	//w->widget->setParent(widget);
	widget->layout()->addWidget(w->widget);
}

mWindow::mWindow(QWidget *parent):QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	window_maximized_ok=false;
	layout->setMargin(0);
}
