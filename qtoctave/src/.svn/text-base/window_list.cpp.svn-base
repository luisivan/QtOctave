/* Copyright (C) 2006 P.L. Lucas
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

#include "window_list.h"
#include <QMdiSubWindow>
#include "basewidget.h"
#include <QMenuBar>

WindowList::WindowList(QMdiArea *work_space, QWidget * parent ):QComboBox(parent)
{
	connect( this, SIGNAL(activated ( int  ) ), this, SLOT(itemClicked_callback ( int  ) ) );
	connect(work_space,SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(window_activated_callback(QMdiSubWindow *) ));
	this->work_space=work_space;
	setIconSize(QSize(32,32));
	resize(100,32);
}

void WindowList::itemClicked_callback ( int item ) 
{
	if(QVariant::Invalid==itemData(item)) return;
	QMdiSubWindow *widget=(QMdiSubWindow *)itemData(item).value<void*>();
	work_space->setActiveSubWindow(widget);
}

void WindowList::showPopup()
{
	clear();
	QList<QMdiSubWindow *> widget_list=work_space->subWindowList(QMdiArea::StackingOrder);
	for(int i=0;i<widget_list.size();i++)
	{
		QMdiSubWindow *widget=widget_list.at(i);
		addItem(widget->windowIcon(),widget->windowTitle(),qVariantFromValue((void*)widget));
	}
	QComboBox::showPopup();
}

void WindowList::window_activated_callback(QMdiSubWindow *widget)
{
	if(widget==NULL) return;
	clear();
	addItem(widget->windowIcon(),widget->windowTitle(),qVariantFromValue((void*)widget));
	/*
	BaseWidget *w=(BaseWidget*)(widget->widget());
	w->menuBar()->show();
	
	QList<QMdiSubWindow *> list=work_space->subWindowList();
	for(int i=0;i<list.size();i++)
	{
		if(list[i]!=widget)
		{
			((BaseWidget*)(list[i]->widget()))->menuBar()->hide();
		}
	}*/
}


