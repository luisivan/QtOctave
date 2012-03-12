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

#ifndef __COMMAND_LIST_H__
#define __COMMAND_LIST_H__

#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QMenu>
#include <QMenuBar>
#include "basewidget.h"
#include "config.h"


/**Model for non editable QStringListModel*/
class StringListModel: public QStringListModel
{
	public:
	StringListModel();
	Qt::ItemFlags flags(const QModelIndex &index) const;
};

/** Builds windows with List of entered commands.
 * 
 */

class CommandList:public BaseWidget
{
	Q_OBJECT
	private:
	QListView *list;
	StringListModel *model;
	QLineEdit *lineEdit;
	QMenu *context_menu;
	QMenuBar *menu_bar;
	QStringList project_history;
	public:
	CommandList( QWidget * parent = 0 );
	~CommandList();
	void setLineEdit(QLineEdit *lineEdit);
	void setSession(Session *session);
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	
	virtual QMenu *get_menu();
	
	public slots:
	void set_list(QStringList list);
	void itemClicked (const QModelIndex & item );
	void copy_clipboard_callback();
	void contextMenu_cb( const QPoint & /*pos*/);
	void clean_history_callback();
	void setProject(QString project);
};


#endif
