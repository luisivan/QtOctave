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


#ifndef __MAIN_H__
#define __MAIN_H__
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QMdiArea>
#include <QTextBrowser>
#include <QListWidget>
#include <QTimer>
#include <QVector>
#include "mainwindow.h"
#include "octave_connection.h"
#include "terminal.h"
#include "window_list.h"
#include "operations.h"
#include "basewidget.h"
#include "variables_list.h"

/**Keeps initial position of QWindows.
*/

struct InitialPosition
{
	int x, y, width, height;
	QWidget *widget;
	bool maximized, minimized;
};

/**Controls all modules. Exec application and interconnect modules.
*/

class Main:public QObject
{
	Q_OBJECT
	MainWindow *main_window;
	QMdiArea *work_space;
	WindowList *window_list;
	Operations *operations;
	OctaveConnection *oc;
	
	Session session;
public:
	Main(QObject * parent = 0);
	BaseWidget *active_widget;
	
	/**Open new tool of type type.*/
	BaseWidget *createTool(WidgetType type, QWidget *parent);
	/**Opens tools in config.*/
	void openTools(QXmlStreamReader &xml, QString config_name=QString() );
	void openFilesToEdit(QStringList files);

public slots:
	/**Shows Octave Help.*/
	void help_octave();
	/**Shows QtOctave Help.*/
	void help_qtoctave();
	/**Shows QtOctave About.*/
	void help_qtoctave_about();
	/**Shows Table.*/
	void table(QString text=QString());
	/**Run an octave script.*/
	void run_file();
	/**variables list visible*/
	void setVisibleVarList();
	void setVisibleNavigator();
	/**Init new variable list.*/
	void variable_list();
	/**New dynamic help*/
	void dynamic_help();
	/**New commands list*/
	void commands_list();
	/**New editor*/
	void editor_callback();
	/**Clear terminal*/
	void clear_terminal();
	/**New SvgCanvas.*/
	void svgcanvas_callback();
	/**Reads output from Octave to build svg_canvas if it is needed.*/
	void line_ready(QString line);

	/**Callback used for sets active_widget.
	 * @param w widget activated.
	*/
	void widget_activated(BaseWidget *w);
	
	/**Returns widget of main window*/
	QWidget *mainWindowWidget();
	
	/**Sets initial position and size of windows in initialPositionList.*/
	void initialPosition_callback();

private:
	VariableList *variableList;
	//Timer for starting tools
	QTimer timer;
	//List of initial position of Widgets
	QVector<InitialPosition> initialPositionList; 
};

#endif
