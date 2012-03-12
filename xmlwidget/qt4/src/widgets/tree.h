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

#ifndef __TREE_H__
#define __TREE_H__

#include <QTreeWidget>
#include <QStack>
#include "widget.h"

Widget *new_tree(const QString & qName, const QXmlAttributes & atts, QWidget *parent);

class Tree: public Widget
{
	QString startmark, endmark, linestartmark, item_id;
	bool get_item_property_ok, signal_property_ok;
	public:
	Tree();
	bool xml_start(const QString & qName, const QXmlAttributes & atts);
	bool xml_end(const QString & qName, const QString &text, bool &property_ok);
	
	void add_widget(Widget *w);
	
	QStack<QTreeWidgetItem *> item_stack;
	QTreeWidgetItem *item;
	QHash<QString, QTreeWidgetItem* > items;
	int col_count;
	QStringList headers;
	bool new_headers;
};

class mTree:public QTreeWidget
{
	Q_OBJECT
	Widget *w;
	public:
	mTree( QWidget * parent = 0);
	void setWidget(Widget *w);
	QString startmark, endmark, linestartmark, item_id;
	
	public slots:
	void double_clicked_cb(QTreeWidgetItem *item , int co);
};

#endif
