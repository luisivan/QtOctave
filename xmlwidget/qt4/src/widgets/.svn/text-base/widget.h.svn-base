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

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QtXml>
#include <QXmlDefaultHandler>
#include <QStack>
#include <QHash>

#include "../output.h"

enum Type {WINDOW, BUTTON,LABEL,LINEEDIT,TREE,VBOX, HBOX, MENU, VSEPARATOR, HSEPARATOR, TEXTEDIT, COMBOBOX, SELECTFILE};

class Widget
{
	public:
	
	virtual bool xml_start(const QString & qName, const QXmlAttributes & atts)=0;
	virtual bool xml_end(const QString & qName, const QString &text, bool &property_ok)=0;
	
	virtual void add_widget(Widget *w)=0;
	
	bool check_id(const QXmlAttributes & atts);
	
	Type type;
	QString name;
	bool container;
	QWidget *widget;
	Output *output;
};

void widget_properties(const QXmlAttributes &atts, Widget *w);


/**Translates <, > and & to &lt; ,&gt; and &amp;
*/
QString toXML(QString text);

#endif

