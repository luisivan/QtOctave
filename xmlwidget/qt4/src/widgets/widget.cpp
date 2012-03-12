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

#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>

void widget_properties(const QXmlAttributes &atts, Widget *w)
{
	QString property=atts.value("x");
	if( !property.isEmpty() )
	{
		QRect size=w->widget->geometry();
		int x=property.toInt();
		w->widget->setGeometry( x, size.y(), size.width(), size.height() );
	}
	
	property=atts.value("y");
	if( !property.isEmpty() )
	{
		QRect size=w->widget->geometry();
		int y=property.toInt();
		w->widget->setGeometry( size.x(), y, size.width(), size.height() );
	}
	
	property=atts.value("width");
	if( !property.isEmpty() )
	{
		QRect size=w->widget->geometry();
		int width=property.toInt();
		if(width<0)
		{
			width=QApplication::desktop()->screenGeometry ().width();
		}
		w->widget->setGeometry( size.x(), size.y(), width, size.height() );
		w->widget->setMinimumWidth(width);
	}
	
	property=atts.value("height");
	if( !property.isEmpty() )
	{
		QRect size=w->widget->geometry();
		int height=property.toInt();
		if(height<0)
		{
			height=QApplication::desktop()->screenGeometry ().height();
		}
		w->widget->setGeometry( size.x(), size.y(), size.width(), height );
		w->widget->setMinimumHeight(height);
	}
	
	property=atts.value("expand");
	if( !property.isEmpty() )
	{
		if(property=="true")
			w->widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
		else
			w->widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	}
	
	property=atts.value("font");
	if( !property.isEmpty() )
	{
		QFont font= w->widget->font ();
		font.setFamily(property);
		w->widget->setFont(font);
	}
	
	property=atts.value("size");
	if( !property.isEmpty() )
	{
		QFont font= w->widget->font ();
		font.setPixelSize(property.toInt());
		w->widget->setFont(font);
	}
	
	property=atts.value("bold");
	if( !property.isEmpty() )
	{
		if(property=="true")
		{
			QFont font= w->widget->font ();
			font.setBold(true);
			w->widget->setFont(font);
		}
		else
		{
			QFont font= w->widget->font ();
			font.setBold(false);
			w->widget->setFont(font);
		}
	}
	
	property=atts.value("italic");
	if( !property.isEmpty() )
	{
		if(property=="true")
		{
			QFont font= w->widget->font ();
			font.setItalic(true);
			w->widget->setFont(font);
		}
		else
		{
			QFont font= w->widget->font ();
			font.setItalic(false);
			w->widget->setFont(font);
		}
	}
}

bool Widget::check_id(const QXmlAttributes & atts)
{
	QString id=atts.value("id");
	if( !id.isEmpty() )
		return id==name;
	return false;
}

QString toXML(QString text)
{
	text.replace("<","&lt;");
	text.replace(">","&gt;");
	text.replace("&","&amp;");
	return text;
}

