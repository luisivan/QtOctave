/* Copyright (C) 2006 P.L. Lucas
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

#ifndef __VARIABLES_LIST_H__
#define __VARIABLES_LIST_H__

#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QContextMenuEvent>
#include <QMenuBar>
#include <QLineEdit>
#include "octave_connection.h"
#include "basewidget.h"


/** Shows variables list. It uses whos command.
*/

class VariableList:public BaseWidget
{
	Q_OBJECT
	public:
	VariableList( QWidget * parent = 0 );
	void setOctaveConnection(OctaveConnection *octave_connection);
	virtual QMenu *get_menu();
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	
	protected:
	void contextMenuEvent ( QContextMenuEvent * event );
	
	private:
	OctaveConnection *octave_connection;
	QPushButton *reload_button;
	QLineEdit *regexp_lineEdit;
	QMenu *popup_menu;
	QAction *popup_edit, *popup_save, *popup_clear;
	//QTextEdit *text;
	QTreeWidget *tree;
	QTreeWidgetItem *last_root_item, *dynamically_item, *local_item/*, *currently_item*/;
	QMenuBar *menu_bar;
	
	public slots:
	void windowActivated ( QWidget * w );
	void windowActivated();
	void send_whos_command_to_octave();
	/**Opens variable table widget*/
	void item_click(QTreeWidgetItem * item, int column);
	
	/** Callback to line_ready signal from OctaveConnection.
	 * @param line line from Octave.
	*/
	void line_ready(QString line);
	
	void popupEvent(QAction*);
	
	/**Activate/desactivate menu options. Return true if child item is selected.*/
	bool set_menu_options();
	
	signals:
	void open_table(QString table_name);
};


#endif
