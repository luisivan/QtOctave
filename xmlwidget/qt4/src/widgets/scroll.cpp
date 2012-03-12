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

#include "scroll.h"

Widget *new_scroll(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "scroll") return NULL;
	
	Scroll *w=new Scroll();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=true;
	w->widget=new mScroll(parent);
	
	return (Widget *)w;
}

Scroll::Scroll():Widget()
{
	
}

bool Scroll::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando scroll %s start %s\n", name.toLocal8Bit().data(),qName.toLocal8Bit().data());
	
	if(qName=="scroll" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		return true;
	}
	
	return false;
}

bool Scroll::xml_end(const QString & qName, const QString &/*text*/, bool &property_ok)
{
	//printf("Procesando scroll end %s\n", qName.toLocal8Bit().data());
	if(qName=="scroll")
	{
		property_ok=false;
		return true;
	}
	return false;
}

void Scroll::add_widget(Widget *w)
{
	//w->widget->setParent(widget);
	mScroll *scroll=((mScroll*)widget);
	scroll->widget()->layout()->addWidget(w->widget);
	w->widget->show();
}

mScroll::mScroll(QWidget *parent):QScrollArea(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	QWidget *inner_widget=new QWidget(this);
	inner_widget->setLayout(layout);
	layout->setMargin(0);
	setWidget(inner_widget);
	inner_widget->show();
	setWidgetResizable ( true );
}
