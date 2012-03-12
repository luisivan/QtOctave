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

#include "vbox.h"

Widget *new_vbox(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "vbox") return NULL;
	
	VBox *w=new VBox();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=true;
	w->widget=new mVBox(parent);
	
	return (Widget *)w;
}

VBox::VBox():Widget()
{
	
}

bool VBox::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando vbox %s start %s\n", name.toLocal8Bit().data(),qName.toLocal8Bit().data());
	
	if(qName=="vbox" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		return true;
	}
	
	return false;
}

bool VBox::xml_end(const QString & qName, const QString &/*text*/, bool &property_ok)
{
	//printf("Procesando vbox end %s\n", qName.toLocal8Bit().data());
	if(qName=="vbox")
	{
		property_ok=false;
		return true;
	}
	return false;
}

#include <QVBoxLayout>

void VBox::add_widget(Widget *w)
{
	//w->widget->setParent(widget);
	widget->layout()->addWidget(w->widget);
}

mVBox::mVBox(QWidget *parent):QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	layout->setMargin(0);
}
