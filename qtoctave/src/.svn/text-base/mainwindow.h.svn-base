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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtGui/QListWidget> 
#include <QMdiArea>
#include <QMessageBox>
#include <QMap>
#include "config.h"
#include "octave_connection.h"
#include "variables_list.h"  
#include "navigator.h"

/*
 * Represents and creates main window. This class is only for creates main window interface, 
 * menus and menus actions. Menus callbacks are implemented in Operations class.<br>
 * Menus and actions can be added to this class or using createAction and createMenu methods.
 * @see GenerateMenu for user menus.
 * @see Operations class for actions callbacks.
 * @see createAction, createMenu
 */

class MainWindow:public BaseWidget
{
	Q_OBJECT
public:
	//General actions
	QAction *actionRunFile;
	QAction *actionOctave_help;
	QAction *actionTable;
	QAction *actionEditor;
	QAction *actionProjects;
	QAction *actionOpenMFile;
	QAction *actionCompletionMatches;
	QAction *actionDynamicHelp;
	QAction *actionStopProcess;
	QAction *actionClearTerminal;
	QAction *actionVariableList;
	QAction *actionNavigator;
	//Matrix actions
	QAction *actionInverse;
	QAction *actionDeterminant;
	QAction *actionEigenvalues;
	QAction *actionTranspose;
	QAction *actionSubmatrix;
	//Statistics actions
	QAction *actionMean;
	QAction *actionMedian;
	QAction *actionStd;
	QAction *actionCov;
	QAction *actionCorrcoef;
	//Plot actions
	QAction *actionPlot;
	QAction *actionPolar;
	QAction *actionSemilogy;
	QAction *actionSemilogx;
	QAction *actionLogLog;
	QAction *actionAxis;
	QAction *actionTitleLabel;
	QAction *actionSvgCanvas;
	QMenu *menu2DPlot;
	//QMenu *menuExport;
	//QAction *actionToEPS;
	//QAction *actionToPDF;
	//QAction *actionToPNG;
	//Config actions
	QAction *actionGeneralConfig;
	/**Main window work space.*/
	QMdiArea *work_space;
	/**Main window menu bar.*/
	QMenuBar *menubar;
	QMenu *menuFile;
	//QMenu *menuEdit;
	QMenu *menuData;
	QMenu *menuMatrix;
	QMenu *menuStatistics;
	QMenu *menuPlot;
	QMenu *menuConfig;
	QMenu *menuHelp;

	QMenu *menuView;
	QMenu *menuDocks;
	QMenu *menuWindowsLayout;
	QMenu *menuTools;
	//QMenu *menuToolbars;
	
	//actions to set view toolbars
	//QAction *actionToolBarDocks;
	//QAction *actionToolBarRun;
	//QAction *actionToolBarMatrix;
	//QAction *actionToolBarHelp;

	QAction *actionSaveWindowsLayout;
	QAction *actionDeleteWindowsLayout;
	

	/**tool bars*/
	QToolBar *toolBarDocks;
	QToolBar *toolBarRun;
	QToolBar *toolBarMatrix;
	QToolBar *toolBarHelp;

	/**Main window status bar.*/
	QStatusBar *statusbar;
	
	//navigator
	Navigator *nav;
	//Docks
	QDockWidget *dockListVar;
	QDockWidget *dockNavigator;
	QDockWidget *dockEditor;
	QDockWidget *dockTerminal;
	QDockWidget *dockCommandHistory;
	
	
	/**Creates new MainWindow.
	 * @param oc OctaveConnection created in terminal.
	 * @param session Session of this session.
	 * @param parent parent widget.
	 */
	MainWindow(OctaveConnection *oc, Session *session, QWidget *parent=0);
	/**Clears all menus.
	 */
	void clear_menu();
	
	
	/**Show config and help menus*/
	void show_config_help_menus();
	
	/**This a QMap of menu actions. You can create menu actions using createAction method.
	 * @see createAction.
	 */
	QMap<QString, QAction *> actions;
	
	/**Creates new action. Actions are added to actions variable.
	 * @param action_name Name of action. Only for internal use.
	 * @param name Label of action in menues.
	 * @param icon Icon path of action in menues.
	 * @return New action.
	 * @see actions.
	 */
	QAction *createAction(QString action_name, const char *name, QString icon=QString());
	QAction *createAction(QString action_name, QString name, QString icon=QString());
	/**This a QMap of menus. You can create menu using createMenu method.
	 * @see createMenu.
	 */
	QMap<QString, QMenu *> menus;
	/**Creates new menu. Menus are added to menus variable.
	 * @param action_name Name of menu. Only for internal use.
	 * @param name Label of menu.
	 * @param icon Icon path of menu.
	 * @return New menu.
	 * @see menus.
	 */
	QMenu *createMenu(QString menu_name, const char *name, QString icon=QString());
	QMenu *createMenu(QString menu_name, QString name, QString icon=QString());
	
	/** Connection to Octave terminal. Useful for send commands to Octave and see
	 * results in terminal.
	 */
	OctaveConnection *octave_connection;
	
	//Session *session;
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	
	/**Windows positions of tools and their docks.*/
	void xmlWindowsPositions(QString config_name, QXmlStreamWriter &xml);
	
private:
	QMessageBox *closeMessage;
	QMap<QString, QString> windowSettings;
	
	void createActions();
	void createMenus();
	void createMenuView();
	void createMenuTools();
	void createStatusBar();
	void createToolBars();
	void createDockWindows();

protected:
	void closeEvent(QCloseEvent *event);
	
private slots:
	//void setVisibleToolBarDocks();
	//void setVisibleToolBarRun();
	//void setVisibleToolBarMatrix();
	//void setVisibleToolBarHelp();
	//void setModeWorkSpace();
	//void setModeDock();
	//void setModeSDI();
	
	void setWindowsLayout(QAction *);
	
	void showSetVisibleObjects();
	void hideSetVisibleObjects();
};

#endif
