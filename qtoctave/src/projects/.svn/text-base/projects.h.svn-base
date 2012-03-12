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

#ifndef __PROJECTS_H__
#define __PROJECTS_H__

#include "ui_projects.h"
#include "../config.h"

using namespace Ui;

/** Shows config window for select or create a new project. Project is a set of ".m" files.
 */
class Projects: public QDialog
{
	Q_OBJECT
	QString project_name;
	Ui_Projects ui;
	QMenu *context_menu;
	QAction *new_project, *modify_project, *delete_project;
	
	/**Displays list of projects name in ListView.*/
	void fill_projects_list();
	public:
	
	Projects(QWidget *parent=0);
	
	~Projects();
	
	/**Last navigator path of project project_name.*/
	static QString navigatorPath(QString project_name);
	
	/**Save last navigator path of project project_name.*/
	static void saveNavigatorPath(QString project_name, QString path);
	
	/**Save variables list. Returns path of file saved.*/
	static void saveVariablesListPath(QString project_name, QString path);
	
	/**Variables list path.*/
	static QString variablesListPath(QString project_name);
	
	/**List of files of project project_name.*/
	static QStringList listFiles(QString project_name);
	
	/**Save list of files of project project_name.*/
	static void saveListFiles(const QString & project_name, const QStringList & files);
	
	/**History of commands of project project_name.*/
	static QStringList listCommandHistory(QString project_name);
	
	/**Save history of commands of project project_name.*/
	static void saveListCommandHistory(const QString & project_name, const QStringList & command_history);
	
	/**Name of selected project.*/
	QString projectName();
	
	public slots:
	/**Show dialog for a new project.*/
	void new_button_callback();
	
	/**Show dialog for modify project.*/
	void modify_project_callback();
	
	/**Delete selected project.*/
	void delete_project_callback();
	
	/**Sets project as active.*/
	void activate_project_callback(QListWidgetItem * item=NULL);
	
	/**Show description of selected project.*/
	void show_description_callback(QListWidgetItem * current, QListWidgetItem * previous );
	
	/**Show context menu.*/
	void contextMenu_cb( const QPoint & pos);
};

#endif
