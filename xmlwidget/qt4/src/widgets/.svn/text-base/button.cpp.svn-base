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

#include "button.h"

#include <QMessageBox>

Widget *new_button(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "button") return NULL;
	
	Button *w=new Button();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	w->widget=new mButton(parent);
	w->type=BUTTON;
	(( mButton*)(w->widget))->setWidget((Widget*)w);
	return (Widget *)w;
}

Button::Button():Widget()
{
	
}

bool Button::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando botón %s\n",qName.toLocal8Bit().data() );
	//mButton *w=(mButton *)widget;
	if(qName=="button" && check_id(atts) )
	{
		//QString icon=atts.value("icon");
		//if(!title.isEmpty()) widget->setWindowTitle ( atts.value("title") );
		
		
		widget_properties(atts,this);
		
		return true;
	}
	else if(qName=="text")
	{
		return true;
	}
	else if(qName=="icon")
	{
		return true;
	}
	else if(qName=="signal")
	{
		QString signal=atts.value("name");
		if(signal=="clicked")
			QObject::connect((mButton*)widget, SIGNAL(clicked()), (mButton*)widget, SLOT(clicked_cb()) );
		return true;
	}
	
	return false;
}

bool Button::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(type!=BUTTON)
	{
		return false;
	}
	mButton *b=(mButton *)widget;
	if(qName=="button")
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
	else if(qName=="icon")
	{
		b->setIcon(QIcon(text));
		QPixmap pix(text);
		QSize size(pix.width(),pix.height());
		b->setIconSize(size);
		property_ok=true;
		return true;
	}
	else if(qName=="signal")
	{
		if(!text.isEmpty())
			b->setOutputFormat(text);
		property_ok=true;
		return true;
	}
	
	return false;
}


void Button::add_widget(Widget */*w*/)
{
	;
}



mButton::mButton( QWidget * parent):QPushButton(parent)
{
}


void mButton::setWidget(Widget *w)
{
	this->w=w;
}

void mButton::setOutputFormat(QString text)
{
	output_format=text;
}

void mButton::clicked_cb()
{
	if(output_format.isEmpty())
		w->output->write("<button id=\""+w->name+"\" signal=\"clicked\"/>");
	else
		w->output->write(output_format);
}

