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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <QPushButton>
#include "widget.h"

Widget *new_button(const QString & qName, const QXmlAttributes & atts, QWidget *parent);

class Button: public Widget
{
	public:
	Button();
	bool xml_start(const QString & qName, const QXmlAttributes & atts);
	bool xml_end(const QString & qName, const QString &text, bool &property_ok);
	
	void add_widget(Widget *w);
};

class mButton:public QPushButton
{
	Q_OBJECT
	Widget *w;
	QString output_format;
	public:
	mButton( QWidget * parent = 0);
	void setWidget(Widget *w);
	void setOutputFormat(QString text);
	
	public slots:
	void clicked_cb();
};

#endif

