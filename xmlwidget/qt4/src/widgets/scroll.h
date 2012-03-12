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

#ifndef __SCROLL_H__
#define __SCROLL_H__

#include <QScrollArea>
#include <QVBoxLayout>
#include "widget.h"

Widget *new_scroll(const QString & qName, const QXmlAttributes & atts, QWidget *parent);

class Scroll: public Widget
{
	public:
	Scroll();
	bool xml_start(const QString & qName, const QXmlAttributes & atts);
	bool xml_end(const QString & qName, const QString &text, bool &property_ok);
	
	void add_widget(Widget *w);
};

class mScroll:public QScrollArea
{
	Q_OBJECT
	public:
	mScroll(QWidget * parent = 0);
};

#endif
