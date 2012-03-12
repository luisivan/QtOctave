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

#include "hseparator.h"

Widget *new_hseparator(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "hseparator") return NULL;
	
	HSeparator *w=new HSeparator();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=true;
	w->widget=new mHSeparator(parent);
	w->type=HSEPARATOR;
	return (Widget *)w;
}

HSeparator::HSeparator():Widget()
{
	child=0;
}

bool HSeparator::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando hseparator %s start %s\n", name.toLocal8Bit().data(),qName.toLocal8Bit().data());
	
	if(qName=="hseparator" && check_id(atts) )
	{
		widget_properties(atts,this);
		child=0;
		return true;
	}
	else if(qName=="first")
	{
		child=1;
		return true;
	}
	else if(qName=="second")
	{
		child=2;
		return true;
	}
	
	return false;
}

bool HSeparator::xml_end(const QString & qName, const QString &/*text*/, bool &property_ok)
{
	//printf("Procesando hseparator end %s\n", qName.toLocal8Bit().data());
	if(qName=="hseparator")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="first")
	{
		property_ok=true;
		return true;
	}
	else if(qName=="second")
	{
		property_ok=true;
		return true;
	}
	return false;
}

void HSeparator::add_widget(Widget *w)
{
	((mHSeparator*)widget)->insertWidget ( child-1, w->widget);
}

mHSeparator::mHSeparator(QWidget *parent):QSplitter(parent)
{
	//setOrientation(Qt::Vertical);
}
