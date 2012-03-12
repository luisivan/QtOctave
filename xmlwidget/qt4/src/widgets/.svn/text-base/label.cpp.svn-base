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

#include "label.h"

#include <QMessageBox>

Widget *new_label(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "label") return NULL;
	
	Label *w=new Label();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	w->widget=new mLabel(parent);
	return (Widget *)w;
}

Label::Label():Widget()
{
	
}

bool Label::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando botón %s\n",qName.toLocal8Bit().data() );
	//mLabel *w=(mLabel *)widget;
	if(qName=="label" && check_id(atts) )
	{
		
		widget_properties(atts,this);
		
		return true;
	}
	else if(qName=="text")
	{
		return true;
	}
	
	
	return false;
}

bool Label::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	
	mLabel *b=(mLabel *)widget;
	if(qName=="label")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="text")
	{
		b->setText(text);
		property_ok=true;
		return true;
	}
	
	return false;
}


void Label::add_widget(Widget */*w*/)
{
	;
}



mLabel::mLabel( QWidget * parent):QLabel(parent)
{
}




