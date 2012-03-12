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

//#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QMap>
#include <QApplication>
#include <QHash>
#include <QDir>
#include <QXmlStreamReader>
#include <QInputDialog>
#include <QMdiSubWindow>

#include "mainwindow.h"
#include "navigator.h"
#include "editor.h"
#include "terminal.h"
#include "config.h"

MainWindow::MainWindow(OctaveConnection *oc, Session *session, QWidget *parent):BaseWidget(parent)
{
	widget_type=MAINWINDOW;
	setSession(session);
	setWindowIcon(QIcon(QString(ICON_PATH) + "/qtoctave.png"));

	menuBar()->clear();

	setWindowTitle("QtOctave ["+session->getProjectName()+"]");

    //QSize mainWinSize = minimumSizeHint();
    //resize(mainWinSize);
    //resize(QSize(800, 600).expandedTo(minimumSizeHint()));

	this->octave_connection=oc;
	setAttribute(Qt::WA_DeleteOnClose);

	createActions();

	closeMessage = new QMessageBox("QtOctave",
			tr("Your changes will have effect, when QtOctave reboot.\
			\n Do you want to do it?"), QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::NoButton
			, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);


	work_space = new QMdiArea(this);
	work_space->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
	work_space->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
	work_space->show();
	setCentralWidget(work_space);

	createMenus();
	createStatusBar();
	createToolBars();
}

//init the actions
void MainWindow::createActions()
{
	actionRunFile = new QAction(this);
	actionRunFile->setIcon(QIcon(QString( ICON_PATH ) + "/run.png"));
	actionRunFile->setText(QApplication::tr("Run an Octave Script"));

	actionOctave_help = new QAction(this);
	actionOctave_help->setIcon(QIcon(QString( ICON_PATH )+"/help.png"));
	actionOctave_help->setText(QApplication::tr("Octave Help"));
	actionOctave_help->setShortcut(QKeySequence::HelpContents);

	actionTable = new QAction(this);
	actionTable->setIcon(QIcon(QString( ICON_PATH )+"/table.png"));
	actionTable->setText(QApplication::tr("Table"));

	actionEditor = new QAction(this);
	actionEditor->setIcon(QIcon(QString( ICON_PATH )+"/xedit.png"));
	actionEditor->setText(QApplication::tr("Editor"));

	actionProjects = new QAction(this);
	actionProjects->setIcon(QIcon(QString( ICON_PATH )+"/contents.png"));
	actionProjects->setText(QApplication::tr("Projects"));

	actionCompletionMatches= new QAction(this);
	actionCompletionMatches->setIcon(QIcon(QString( ICON_PATH )+"/search.png"));
	actionCompletionMatches->setText(QApplication::tr("<b>Completion matches:</b><br> Generates possible completions given in comand line."));

	actionDynamicHelp= new QAction(this);
	actionDynamicHelp->setIcon(QIcon(QString( ICON_PATH )+"/help_index.png"));
	actionDynamicHelp->setText(QApplication::tr("Dynamic Help: Shows help of completions matches."));

	actionStopProcess= new QAction(this);
	actionStopProcess->setIcon(QIcon(QString( ICON_PATH )+"/button_cancel.png"));
	actionStopProcess->setText(QApplication::tr("<b>Stops process:</b><br>Stops uncontroled process, infinite loops,..."));

	actionClearTerminal= new QAction(this);
	actionClearTerminal->setIcon(QIcon(QString( ICON_PATH )+ "/eraser.png"));
	actionClearTerminal->setText(QApplication::tr("Clear terminal"));

	actionVariableList= new QAction(this);
	actionVariableList->setIcon(QIcon(QString( ICON_PATH )+ "/taskbar.png"));
	actionVariableList->setText(QApplication::tr("Variable List"));

	actionNavigator= new QAction(this);
	actionNavigator->setIcon(QIcon(QString( ICON_PATH )+ "/new_folder.png"));
	actionNavigator->setText(QApplication::tr("Navigator"));

	createAction("actionCommandList", tr("Command List"), (QString( ICON_PATH )+ "/news_section.png"));

	//Toolbars actions
	//actionToolBarDocks = new QAction("Docks", this);
	//actionToolBarDocks->setCheckable(true);
	//actionToolBarDocks->setChecked(true);
	//actionToolBarRun = new QAction("Terminal", this);
	//actionToolBarRun->setCheckable(true);
	//actionToolBarRun->setChecked(true);
	//actionToolBarMatrix = new QAction("Matrix", this);
	//actionToolBarMatrix->setCheckable(true);
	//actionToolBarMatrix->setChecked(true);
	//actionToolBarHelp = new QAction("Help", this);
	//actionToolBarHelp->setCheckable(true);
	//actionToolBarHelp->setChecked(true);

	actionSaveWindowsLayout= new QAction(this);
	actionSaveWindowsLayout->setText(QApplication::tr("Save actual windows layout"));

	actionDeleteWindowsLayout= new QAction(this);
	actionDeleteWindowsLayout->setText(QApplication::tr("Delete some windows layout"));

	//Matrix actions
	actionInverse = new QAction(this);
	actionInverse->setText(QApplication::tr("Inverse"));

	actionDeterminant = new QAction(this);
	actionDeterminant->setText(QApplication::tr("Determinant"));

	actionEigenvalues = new QAction(this);
	actionEigenvalues->setText(QApplication::tr("Eigenvalues and eigenvectors"));

	actionTranspose = new QAction(this);
	actionTranspose->setText(QApplication::tr("Transpose"));

	actionSubmatrix = new QAction(this);
	actionSubmatrix->setText(QApplication::tr("Submatrix"));

	actionMean = new QAction(this);
	actionMean->setText(QApplication::tr("Mean"));

	actionMedian = new QAction(this);
	actionMedian->setText(QApplication::tr("Median"));

	actionStd = new QAction(this);
	actionStd->setText(QApplication::tr("Standard Deviation"));

	actionCov = new QAction(this);
	actionCov->setText(QApplication::tr("Covariance"));

	actionCorrcoef = new QAction(this);
	actionCorrcoef->setText(QApplication::tr("Correlation Coefficient"));

	//Plot actions
	actionPlot = new QAction(this);
	actionPlot->setText(QApplication::tr("Plot"));

	actionPolar = new QAction(this);
	actionPolar->setText(QApplication::tr("Polar"));

	actionSemilogy = new QAction(this);
	actionSemilogy->setText(QApplication::tr("Log scale for the y axis"));

	actionSemilogx = new QAction(this);
	actionSemilogx->setText(QApplication::tr("Log scale for the x axis"));

	actionLogLog = new QAction(this);
	actionLogLog->setText(QApplication::tr("Log scale for the x and y axis"));

	actionAxis = new QAction(this);
	actionAxis->setText(QApplication::tr("Axis scale"));

	actionTitleLabel = new QAction(this);
	actionTitleLabel->setText(QApplication::tr("Title and labels"));

	actionSvgCanvas = new QAction(this);
	actionSvgCanvas->setText(QApplication::tr("Svg Canvas"));

	//actionToEPS  = new QAction(this);
	//actionToEPS->setText(QApplication::tr("EPS"));

	//actionToPDF  = new QAction(this);
	//actionToPDF->setText(QApplication::tr("PDF"));

	//actionToPNG  = new QAction(this);
	//actionToPNG->setText(QApplication::tr("PNG"));

	//Config actions
	actionGeneralConfig  = new QAction(this);
	actionGeneralConfig->setText(QApplication::tr("General configuration"));
	createAction("qtoctave_pkg", tr("Install Octave packages"));


	//connect the actions
	//connect(actionToolBarDocks, SIGNAL(triggered()), this, SLOT(setVisibleToolBarDocks()));
	//connect(actionToolBarRun, SIGNAL(triggered()), this, SLOT(setVisibleToolBarRun()));
	//connect(actionToolBarMatrix, SIGNAL(triggered()), this, SLOT(setVisibleToolBarMatrix()));
	//connect(actionToolBarHelp, SIGNAL(triggered()), this, SLOT(setVisibleToolBarHelp()));
	//connect(actionModeWorkspace, SIGNAL(triggered()), this, SLOT(setModeWorkSpace()));
	//connect(actionModeDock, SIGNAL(triggered()), this, SLOT(setModeDock()));
	//connect(actionModeSDI, SIGNAL(triggered()), this, SLOT(setModeSDI()));
}

void MainWindow::createMenus()
{
	//menubar = new QMenuBar(this);
	menubar = menuBar();
	menuFile = createMenu(QString("/File"),QApplication::tr("File"));
	menuHelp = createMenu("/Help", QApplication::tr("Help"));
	menuData = createMenu("/Data", QApplication::tr("Data"));
	menuMatrix = createMenu("/Matrix", QApplication::tr("Matrix"));
	menuTools = createMenu("/Tools", QApplication::tr("Tools"));
	menuStatistics = createMenu("/Statistics", QApplication::tr("Statistics"));
	menuPlot = createMenu("/Plot", QApplication::tr("Plot"));
	//menuExport = createMenu("/Plot/Export to...", QApplication::tr("Export to..."));
	menu2DPlot = createMenu("/Plot/2D...", QApplication::tr("2D..."));
	menuConfig = createMenu("/Config", QApplication::tr("Config"));
	menuView = createMenu("/View",QApplication::tr("View"));
	menuDocks = createMenu("/Docks", QApplication::tr("Dock Tools"));

	menuWindowsLayout = createMenu("/View/WindowsLayout",QApplication::tr("Windows Layout"));

	//Connects menuView with trigger to control menuWindowsLayout
	connect(menuView, SIGNAL(triggered ( QAction *)), this, SLOT(setWindowsLayout(QAction *)));

	//menuToolbars = createMenu("/Toolbars",QApplication::tr("Toolbars"));
	//Shows checks of visible toolbars
	//connect(menuToolbars, SIGNAL(triggered(QAction * )), this, SLOT(setVisibleToolBars(QAction * )));

	//setMenuBar(menubar);

	//Add actions to menubar
	menubar->addAction(menuFile->menuAction());
	menubar->addAction(menuData->menuAction());
	menubar->addAction(menuMatrix->menuAction());
	menubar->addAction(menuStatistics->menuAction());
	menubar->addAction(menuPlot->menuAction());
	menubar->addAction(menuView->menuAction());

	menuFile->addAction(actionProjects);
	menuFile->addAction(actionRunFile);
	menuFile->addAction(createAction("cd", tr("Change Directory")));
	menuFile->addAction(createAction("exit", tr("Quit"),
			       QString( ICON_PATH ) + "/exit.png"));

	//Builds/Destroys menu View
	connect(menuView,SIGNAL(aboutToShow ()), this, SLOT(showSetVisibleObjects()) );
	connect(menuView,SIGNAL(aboutToHide ()), this, SLOT(hideSetVisibleObjects()) );

	menuData->addAction(actionTable);

	menuMatrix->addAction(createAction("+","A+B"));
	menuMatrix->addAction(createAction("*","A*B"));
	menuMatrix->addAction(createAction("**", tr("A**n Exponential")));
	menuMatrix->addAction(actionDeterminant);
	menuMatrix->addAction(actionEigenvalues);
	menuMatrix->addAction(actionInverse);
	menuMatrix->addAction(actionTranspose);
	menuMatrix->addAction(actionSubmatrix);
	menuStatistics->addAction(actionMean);
	menuStatistics->addAction(actionMedian);
	menuStatistics->addAction(actionStd);
	menuStatistics->addAction(actionCov);
	menuStatistics->addAction(actionCorrcoef);
	menuStatistics->addAction(createAction("hist", tr("Histogram")));
	//menuPlot->addAction(menuExport->menuAction());
	menuPlot->addAction(menu2DPlot->menuAction());
	menuPlot->addAction(createMenu("menu3DPlot","3D...")->menuAction());
	menu2DPlot->addAction(actionPlot);
	menu2DPlot->addAction(actionPolar);
	menu2DPlot->addAction(actionLogLog);
	menu2DPlot->addAction(actionSemilogy);
	menu2DPlot->addAction(actionSemilogx);
	menu2DPlot->addAction(createAction("errorbar", tr("Error Bars")));
	menu2DPlot->addAction(createAction("hist", tr("Histogram")));
	menu2DPlot->addAction(createAction("bar", tr("Bar graph")));
	menus.value("menu3DPlot")->addAction(
			createAction("mesh", tr("Plot of three-dimensional surface")));
	menus.value("menu3DPlot")->addAction(
			createAction("contour", tr("Contour plot of three-dimensional surface")));
	menuPlot->addAction(actionAxis);
	menuPlot->addAction(actionTitleLabel);
	menuPlot->addAction(actionSvgCanvas);
}

void MainWindow::createMenuTools()
{

	BaseWidget *w=(BaseWidget*)session->getFirstTool(COMMANDLIST);
	if(w!=NULL && w->get_menu()!=NULL) menuTools->addMenu(w->get_menu());

	w=(BaseWidget*)session->getFirstTool(VARIABLESLIST);
	if(w!=NULL && w->get_menu()!=NULL) menuTools->addMenu(w->get_menu());
}


void MainWindow::createMenuView()
{
	menuView->clear();
	menuView->addAction(actionClearTerminal);

	menuView->addMenu(menuDocks);
	menuDocks->clear();

	menuDocks->addAction(actionEditor);
	menuDocks->addAction(actionVariableList);
	menuDocks->addAction(actionNavigator);
	menuDocks->addAction(actions["actionCommandList"]);

	menuView->addMenu(menuWindowsLayout);
	menuWindowsLayout->clear();

	menuWindowsLayout->addAction(actionSaveWindowsLayout);
	menuWindowsLayout->addAction(actionDeleteWindowsLayout);

	menuWindowsLayout->addSeparator();

	QString home=QDir::home().path()+"/.qtoctave/";

	{
		QFile file(home+"windows_layout.xml");

		if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			QXmlStreamReader xml(&file);

			while (!xml.atEnd())
			{
				xml.readNext();
				if( xml.isStartElement() )
				{
					if(xml.name()=="tools_config")
					{
						QXmlStreamAttributes attr=xml.attributes();

						QString name=attr.value("name").toString();

						if( ! name.isEmpty() )
						{
							QAction *action=createAction("WindowsLayout "+name,name);
							action->setData(name);
							menuWindowsLayout->addAction(action);
						}
					}
				}
			}

			file.close();
		}
	}


	//Put checks  in View/Mode menus
	//actionModeWorkspace->setCheckable(true);
	//actionModeWorkspace->setChecked(true);

	QMenu *objects=createPopupMenu();
	objects->setTitle(tr("Show/Hide Objects"));
	menuView->addMenu(objects);
}


void MainWindow::createStatusBar()
{
	statusbar = new QStatusBar(this);
	//statusbar->setGeometry(QRect(0, 396, 503, 22));
	setStatusBar(statusbar);
}


void MainWindow::createToolBars()
{
	toolBarRun = new QToolBar(tr("Terminal Run"), this) ;
	//toolBarRun->setOrientation(Qt::Horizontal);
	addToolBar(static_cast<Qt::ToolBarArea>(4), toolBarRun);
	toolBarRun->setObjectName("ToolBar: Terminal Run");

	toolBarMatrix = new QToolBar(tr("Matrix"), this);
	//toolBarMatrix->setOrientation(Qt::Horizontal);
	addToolBar(static_cast<Qt::ToolBarArea>(4), toolBarMatrix);
	toolBarMatrix->setObjectName("ToolBar: Matrix");

	toolBarHelp = new QToolBar(tr("Help"), this);
	addToolBar(static_cast<Qt::ToolBarArea>(4), toolBarHelp);
	toolBarHelp->setObjectName("ToolBar: Help");

	toolBarDocks = new QToolBar(tr("Docks"),this);
	toolBarDocks->setOrientation(Qt::Horizontal);
	addToolBar(static_cast<Qt::ToolBarArea>(4), toolBarDocks);
	toolBarDocks->setObjectName("ToolBar: Docks");


	//add to the toolbar docks
	toolBarDocks->addAction(actionEditor);
	toolBarDocks->addAction(actionVariableList);
	toolBarDocks->addAction(actionNavigator);
	toolBarDocks->addAction(actions["actionCommandList"]);

	//add to the toolbar Matrix
	toolBarMatrix->addAction(actionTable);
	//toolBar->addSeparator();

	//add to the toolbar Help
	toolBarHelp->addAction(actionOctave_help);
	toolBarHelp->addAction(actionDynamicHelp);
	toolBarHelp->addAction(actionCompletionMatches);
	//toolBar->addSeparator();

	//add to the toolbar run
	toolBarRun->addAction(actionRunFile);
	toolBarRun->addAction(actionStopProcess);
	toolBarRun->addAction(actionClearTerminal);
	//toolBar->addSeparator();


}

QAction *MainWindow::createAction(QString action_name, QString name, QString icon)
{
	return createAction(action_name, name.toLocal8Bit().data(), icon);
}

QAction *MainWindow::createAction(QString action_name, const char *name, QString icon)
{
	if( actions.contains(action_name) ) return actions[action_name];
	QAction *a = new QAction(this);
	if( !icon.isEmpty() ) a->setIcon(QIcon(icon));
	a->setText(QApplication::tr(name));
	actions[action_name]=a;
	return a;
}

QMenu *MainWindow::createMenu(QString menu_name, QString name, QString icon)
{
	return createMenu(menu_name, name.toLocal8Bit().data(), icon);
}

QMenu *MainWindow::createMenu(QString menu_name, const char *name, QString icon)
{
	if( menus.contains(menu_name) )
	       return menus[menu_name];

	QMenu *a = new QMenu(menubar);

	if(!icon.isEmpty())
	       a->setIcon(QIcon(icon));
	a->setTitle(QApplication::tr(name));
	menus[menu_name]=a;
	return a;
}

void MainWindow::clear_menu()
{
	menubar->clear();
}


void MainWindow::show_config_help_menus()
{
	menubar->addAction(menuConfig->menuAction());
	menubar->addAction(menuHelp->menuAction());

	menuConfig->addAction(actionGeneralConfig);
	menuConfig->addAction( createAction("qtoctave_pkg", tr("Install Octave packages"))  );

	menuHelp->addAction(actionOctave_help);
	menuHelp->addAction(actionDynamicHelp);
	menuHelp->addAction(createAction("qtoctave_help", tr("QtOctave Help")));
	menuHelp->addAction(createAction("qtoctave_about", tr("About")));
}

void MainWindow::createDockWindows()
{
	//Editor dock and Terminal , only if the mode from work area is dock

}


void MainWindow::setWindowsLayout(QAction *action)
{
	printf("[MainWindow::setWindowsLayout] \n");
	if(action==actionSaveWindowsLayout)
	{
		printf("[MainWindow::setWindowsLayout] Starting saving\n");
		bool ok;
		QString config_name = QInputDialog::getText(this, tr("Name for layout"),
							tr("Name:"), QLineEdit::Normal,
							"", &ok);
		if (ok && !config_name.isEmpty())
		{
			config_name="/View/WindowsLayout/"+config_name;
			//Add last_windows_layout.xml to windows_layout.xml
			QString home=QDir::home().path()+"/.qtoctave/";

			{
				QFile file(home+"last_windows_layout.xml");

				file.open(QIODevice::WriteOnly | QIODevice::Text);

				QXmlStreamWriter xml(&file);
				xml.setAutoFormatting(true);
				xmlWindowsPositions("last_windows_layout",xml);

				file.close();
			}

			QByteArray result;
			QXmlStreamWriter xml(&result);
			xml.setAutoFormatting(true);
			xml.writeStartDocument();

			QFile wl_file(home+"windows_layout.xml");
			if(wl_file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QXmlStreamReader wl_xml(&wl_file);

				while (!wl_xml.atEnd())
				{
					wl_xml.readNext();
					if( wl_xml.isStartElement() )
					{
						if(wl_xml.name()=="tools_config")
						{
							QXmlStreamAttributes attr=wl_xml.attributes();

							QString name=attr.value("name").toString();

							if(name==config_name)
							{
								while (!wl_xml.atEnd())
								{
									wl_xml.readNext();
									if( wl_xml.isEndElement() )
									{
										if(wl_xml.name()=="tools_config") break;
									}
								}
							}
							else
							{
								xml.writeStartElement(wl_xml.name().toString());
								xml.writeAttribute("name", name);
							}
						}
						else if(wl_xml.name()=="state")
						{
							xml.writeStartElement(wl_xml.name().toString());
							xml.writeCharacters(wl_xml.readElementText());
							xml.writeEndElement();
						}
						else
						{
							xml.writeStartElement(wl_xml.name().toString());
							QXmlStreamAttributes attr=wl_xml.attributes();
							xml.writeAttributes(attr);
						}
					}
					else if( wl_xml.isEndElement() )
					{
						if(wl_xml.name()=="windows_layouts")
						{
						}
						else xml.writeEndElement();
					}
				}

				wl_file.close();
			}
			else xml.writeStartElement("windows_layouts");

			QFile last_wl_file(home+"last_windows_layout.xml");
			if(!last_wl_file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				printf("last_windows_layout.xml couldn't be opened\n");
				return;
			}
			QXmlStreamReader last_wl_xml(&last_wl_file);

			while (!last_wl_xml.atEnd())
			{
				last_wl_xml.readNext();
				if( last_wl_xml.isStartElement() )
				{
					if(last_wl_xml.name()=="tools_config")
					{
						xml.writeStartElement(last_wl_xml.name().toString());
						xml.writeAttribute("name", config_name);
					}
					else if(last_wl_xml.name()=="state")
					{
						xml.writeStartElement(last_wl_xml.name().toString());
						xml.writeCharacters(last_wl_xml.readElementText());
						xml.writeEndElement();
					}
					else
					{
						xml.writeStartElement(last_wl_xml.name().toString());
						QXmlStreamAttributes attr=last_wl_xml.attributes();
						xml.writeAttributes(attr);
					}
				}
				else if( last_wl_xml.isEndElement() )
				{
					xml.writeEndElement();
				}
			}

			last_wl_file.close();

			xml.writeEndElement();

			xml.writeEndDocument();

			//printf("Layout %s\n", result.data());

			QFile file(home+"windows_layout.xml");
			if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
			file.write(result);
			file.close();
		}
	}
	else if(action==actionDeleteWindowsLayout)
	{
		QDialog delete_wl_dialog(this);
		delete_wl_dialog.setWindowTitle(tr("Delete windows' layout"));

		QVBoxLayout *vbox=new QVBoxLayout;
		delete_wl_dialog.setLayout(vbox);

		QLabel *label=new QLabel("Select layout to delete:",&delete_wl_dialog);
		vbox->addWidget(label);
		label->show();

		QListWidget *listWidget = new QListWidget(&delete_wl_dialog);
		vbox->addWidget(listWidget);
		listWidget->show();

		QHBoxLayout *hbox=new QHBoxLayout;
		vbox->addLayout(hbox);

		QPushButton *ok_button=new QPushButton(tr("Delete"), &delete_wl_dialog );
		hbox->addWidget(ok_button);
		ok_button->show();

		QPushButton *cancel_button=new QPushButton(tr("Cancel"), &delete_wl_dialog );
		hbox->addWidget(cancel_button);
		cancel_button->show();
		cancel_button->setDefault(true);

		connect(ok_button, 	SIGNAL(clicked()), &delete_wl_dialog, SLOT(accept()) );
		connect(cancel_button, 	SIGNAL(clicked()), &delete_wl_dialog, SLOT(reject()) );

		QString home=QDir::home().path()+"/.qtoctave/";

		{ //Fills list of windows' layouts
			QFile wl_file(home+"windows_layout.xml");
			if(wl_file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QXmlStreamReader wl_xml(&wl_file);

				while (!wl_xml.atEnd())
				{
					wl_xml.readNext();
					if( wl_xml.isStartElement() )
					{
						if(wl_xml.name()=="tools_config")
						{
							QXmlStreamAttributes attr=wl_xml.attributes();

							QString name=attr.value("name").toString();

							new QListWidgetItem(name, listWidget);
						}
					}
				}

				wl_file.close();
			}
		}

		int ok=delete_wl_dialog.exec();

		if(ok==QDialog::Accepted)
		{
			QListWidgetItem *item=listWidget->currentItem();
			QString config_name=item->data(Qt::DisplayRole).toString();

			QByteArray result;
			QXmlStreamWriter xml(&result);
			xml.setAutoFormatting(true);
			xml.writeStartDocument();

			QFile wl_file(home+"windows_layout.xml");
			if(wl_file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QXmlStreamReader wl_xml(&wl_file);

				while (!wl_xml.atEnd())
				{
					wl_xml.readNext();
					if( wl_xml.isStartElement() )
					{
						if(wl_xml.name()=="tools_config")
						{
							QXmlStreamAttributes attr=wl_xml.attributes();

							QString name=attr.value("name").toString();

							if(name==config_name)
							{
								while (!wl_xml.atEnd())
								{
									wl_xml.readNext();
									if( wl_xml.isEndElement() )
									{
										if(wl_xml.name()=="tools_config") break;
									}
								}
							}
							else
							{
								xml.writeStartElement(wl_xml.name().toString());
								xml.writeAttribute("name", name);
							}
						}
						else if(wl_xml.name()=="state")
						{
							xml.writeStartElement(wl_xml.name().toString());
							xml.writeCharacters(wl_xml.readElementText());
							xml.writeEndElement();
						}
						else
						{
							xml.writeStartElement(wl_xml.name().toString());
							QXmlStreamAttributes attr=wl_xml.attributes();
							xml.writeAttributes(attr);
						}
					}
					else if( wl_xml.isEndElement() )
					{
						xml.writeEndElement();
					}
				}

				wl_file.close();
			}

			xml.writeEndDocument();

			QFile file(home+"windows_layout.xml");
			if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
			file.write(result);
			file.close();
		}
	}
	else
	{
		QString config_name=action->data().toString();
		if(config_name.startsWith("/View/WindowsLayout/"))
		{

			QMap<QString, QString> windowSettings;

			windowSettings["mode_work_area"]=config_name;

			set_config(windowSettings);

			if(closeMessage->exec() == QMessageBox::Yes)
				close();
		}
	}
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	int ok=QMessageBox::question(this, tr("Close"), tr("You are going to close all QtOctave's windows. Are you sure?"), QMessageBox::Yes, QMessageBox::No );

	if(ok!=QMessageBox::Yes)
	{
		event->ignore ();
		return;
	}

	QString home=QDir::home().path()+"/.qtoctave/";

	{
		QFile file(home+"last_windows_layout.xml");

		file.open(QIODevice::WriteOnly | QIODevice::Text);

		QXmlStreamWriter xml(&file);
		xml.setAutoFormatting(true);
		xmlWindowsPositions("last_windows_layout",xml);

		file.close();
	}


	Terminal *terminal=(Terminal *)session->getFirstTool(TERMINAL);
	terminal->saveVariables();

	ok=QMessageBox::information (this, tr("Close"), tr("Octave is saving data. Press ok at end.") );


	foreach (QWidget *widget, QApplication::topLevelWidgets())
	{
		if(widget!=this) widget->close();
	}

	set_config(windowSettings);



	QApplication::quit();
	//exit(0);
}

void MainWindow::showSetVisibleObjects()
{
	createMenuView();
	createMenuTools();
}

void MainWindow::hideSetVisibleObjects()
{
	menuView->clear();
	menuWindowsLayout->clear();
	menuDocks->clear();
}

BaseWidget *MainWindow::copyBaseWidget( QWidget * parent )
{
	return NULL;
}

static void writeXMLBaseWidget(BaseWidget *bw, QString parent, QXmlStreamWriter &xml)
{
	xml.writeStartElement("tool");

	xml.writeAttribute("type", widgetName(bw->widgetType()));
	
	if(parent == "workspace")
	{
		xml.writeAttribute("xPosition", QString::number(bw->parentWidget()->pos().x()) );
		xml.writeAttribute("yPosition", QString::number(bw->parentWidget()->pos().y()) );
		xml.writeAttribute("width", QString::number(bw->parentWidget()->rect().width()) );
		xml.writeAttribute("height", QString::number(bw->parentWidget()->rect().height()) );
		xml.writeAttribute("maximized", (bw->parentWidget()->isMaximized())?QString("true"):QString("false") );
		xml.writeAttribute("minimized", (bw->parentWidget()->isMinimized())?QString("true"):QString("false") );
	}

	if( !parent.isEmpty() ) xml.writeAttribute("place", parent);

	QVector<QObject*> docks=bw->getDocks();

		for(int i=0;i<docks.size();i++)
		{
			BaseWidget *_bw=(BaseWidget *) ((QDockWidget*)(docks[i]))->widget();
			if(bw!=NULL) writeXMLBaseWidget(_bw, "", xml);
		}

	bw->toXML(xml);

	xml.writeStartElement("state");
	QString state_string( bw->saveState().toHex() );
	xml.writeCharacters(state_string);

	xml.writeEndElement();

	xml.writeEndElement();
}

static QVector<BaseWidget*> parentBaseWidgets(Session *session)
{
	QVector<BaseWidget*> parents;

	QVector<QObject*> tools=session->getTools();
	QVector<QObject*> main_tools=session->getTools(MAIN);

	for(int i=0;i<tools.size();i++)
	{
		if(tools[i]!=NULL && !main_tools.contains(tools[i]) && tools[i]->parent()==NULL)
			parents << (BaseWidget*)tools[i];
	}

	return parents;
}

void MainWindow::xmlWindowsPositions(QString config_name, QXmlStreamWriter &xml)
{
	QVector<BaseWidget*> tools;

	xml.writeStartDocument();

	xml.writeStartElement("tools_config");

	xml.writeAttribute("name", config_name);

		QList<QMdiSubWindow *> work_space_tools=work_space->subWindowList();

		for(int i=0;i<work_space_tools.size();i++)
		{
			BaseWidget *bw=(BaseWidget*)(work_space_tools[i]->widget());
			if(bw!=NULL) writeXMLBaseWidget(bw, "workspace", xml);
		}

		QVector<BaseWidget*> parents=parentBaseWidgets(session);

		for(int i=0;i<parents.size();i++)
		{
			writeXMLBaseWidget(parents[i], "window",xml);
		}

	xml.writeEndElement();

	xml.writeEndDocument();
}
