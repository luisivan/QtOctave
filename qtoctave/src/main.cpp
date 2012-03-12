/* Copyright (C) 2006,2007,2008 P.L. Lucas
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

#include <QDockWidget>

#include "main.h"
#include "window_list.h"
#include "help.h"
#include "table.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QXmlStreamReader>
#include <QList>
#include <QMessageBox>
#include <QHash>
#include <QMdiSubWindow>
#include "generate_menu.h"
#include "mainwindow.h"
#include "editor.h"
#include "svgcanvas/svgcanvas.h"



Main::Main(QObject * parent ):QObject (parent)
{

    // Add the Inconsolata font for the editor
    QFontDatabase::addApplicationFont (QDir::home().path()+QDir::separator()+"fonts/Inconsolata.otf");

	//Build Octave commands list
	{
		QString oc;

		if( get_config("octave_path").isEmpty() )
            oc = "octave";
		else
            oc = get_config("octave_path");


        QString home = QDir::home().path()+QDir::separator()+".qtoctave"+QDir::separator()+"commands.txt";

		QString command=

		"l=completion_matches('');"
		"[nrows,ncols]=size(l);"

		"out=fopen('"+home+"','w');"

		"for k=1:nrows;"
		"fprintf(out,'%s\\n',l(k,:));"
		"endfor;"

		"fclose(out);"
		;

        printf("[Main::Main] Building command list.\n");

		system(QString(oc+" --no-history -q  --eval \""+command+"\"").toLocal8Bit().data());

        printf("[Main::Main] Commands list built.\n");
	}

    // Initialize the octave connection
	oc = new OctaveConnection();

    QString session_name = get_config("session_name");

    if( !session_name.isEmpty() && session_name != "Empty")
		session.setProjectName(session_name);

	session.addTool(MAIN, this);

	main_window = new MainWindow(oc, &session);

    work_space = main_window->work_space;

    window_list = NULL;

	if( get_config("octave_path").isEmpty() )
		oc->setOctavePath("octave");
	else
		oc->setOctavePath(get_config("octave_path"));

	Terminal *terminal = (Terminal*)createTool(TERMINAL, work_space);
	terminal->work_space=work_space;
	terminal->setOctaveConnection(oc);

	oc->startOctave();

	terminal->setProject();

	//Show list of windows in workspace
	window_list = new WindowList(work_space,main_window->toolBarDocks);
	main_window->toolBarDocks->addWidget(window_list);
	window_list->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred));
	window_list->show();
	main_window->showMaximized();

	{//Open tools from config
		QString home=QDir::home().path()+QDir::separator()+".qtoctave"+QDir::separator();
		QString modeWorkArea=get_config("mode_work_area");

		QFile file(home+"last_windows_layout.xml");
		QFile wl_file(home+"windows_layout.xml");

		QXmlStreamReader xml;

		if(modeWorkArea=="last" && file.exists())
		{
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			xml.setDevice(&file);
			modeWorkArea.clear();
		}
		else if( !modeWorkArea.isEmpty() && wl_file.exists() )
		{
			wl_file.open(QIODevice::ReadOnly | QIODevice::Text);
			xml.setDevice(&wl_file);
		}
		else
		{
			QString xmlConfig=
			"<tools_config>"
				"<tool type='main_window'>"
					"<tool type='terminal' place='workspace'>"
						//"<tool type='table'>"
						//	"<matrix value='a'/>"
						//"</tool>"
					"</tool>"
					"<tool type='variables_list'/>"
					"<tool type='command_list'/>"
					"<tool type='navigator'/>"
				"</tool>"
				"<tool type='editor' place='window'>"
					//"<tool type='help' title='Octave Help'/>"
					//"<tool type='dynamic_help'/>"
				"</tool>"
			"</tools_config>"
			;
			xml.addData(xmlConfig);
			modeWorkArea.clear();
		}

		openTools(xml, modeWorkArea);

		file.close();
		wl_file.close();

		QMap<QString, QString> windowSettings;
		windowSettings["mode_work_area"]="last";
		set_config(windowSettings);
	}

	connect(oc, SIGNAL(clearScreen()), this, SLOT(clear_terminal()));


	connect(main_window->actionCompletionMatches, SIGNAL(triggered()), terminal, SLOT(completion_matches_callback()));
	connect(main_window->actionDynamicHelp, SIGNAL(triggered()), this, SLOT(dynamic_help()));
	connect(main_window->actionStopProcess, SIGNAL(triggered()), terminal, SLOT(stop_process_callback()));
	connect(main_window->actionClearTerminal, SIGNAL(triggered()), terminal, SLOT(clear_callback()));

	operations=new Operations(this,&active_widget,main_window);
	operations->setOctaveConnection(oc);
	operations->setSession(&session);

	//Build menus from files

	GenerateMenu generate_menu(main_window, operations);
	generate_menu.setPath( QString(CONFIG_PATH) +QDir::separator()+ "menus");
	generate_menu.load_menu();

	QString home=QDir::home().path()+QString(QDir::separator())+QString(".qtoctave")+QString(QDir::separator());

	generate_menu.setPath( QString(home+"menus") );
	generate_menu.load_menu();

	//generate_menu.setPath("./menus");
	//generate_menu.load_menu();

	main_window->show_config_help_menus();

	connect(main_window->actionOctave_help, SIGNAL(triggered()), this, SLOT(help_octave()));
	connect(main_window->actions.value("qtoctave_help"),  SIGNAL(triggered()), this, SLOT(help_qtoctave()));
	connect(main_window->actions.value("qtoctave_about"),  SIGNAL(triggered()), this, SLOT(help_qtoctave_about()));
	connect(main_window->actionTable, SIGNAL(triggered()), this, SLOT(table()));

	connect(main_window->actionVariableList, SIGNAL(triggered()), this, SLOT(variable_list()));

	//connect the Navigator
	connect(main_window->actionNavigator, SIGNAL(triggered()), this, SLOT(setVisibleNavigator()));

	connect(main_window->actionRunFile, SIGNAL(triggered()), this, SLOT(run_file()));

	connect(main_window->actionEditor, SIGNAL(triggered()), this, SLOT(editor_callback()));

	connect(main_window->actions["actionCommandList"], SIGNAL(triggered()), this, SLOT(commands_list()));

	connect(main_window->actionSvgCanvas, SIGNAL(triggered()), this, SLOT(svgcanvas_callback()));

	if(oc!=NULL)
	{
		connect(oc, SIGNAL(line_ready(QString)), this, SLOT(line_ready(QString)));
	}

	//main_window->showMaximized();
}

void Main::widget_activated(BaseWidget *w)
{
	active_widget=w;
}

void Main::line_ready(QString line)
{
	//Builds SvgCanvas if it's needed.
	QRegExp re("~~svgcanvas: *(\\d+) +(.+)\n");
	if( re.exactMatch(line) )
	{
		int number=re.cap(1).toInt();

		QVector<QObject*> tools=session.getTools(SVGCANVAS);
		int num;
		for(int i=0;i<tools.size();i++)
		{
			num=((SvgCanvas*)tools[i])->getCanvasNumber();
			if(num==number) return;
		}

		//SvgCanvas needed
		SvgCanvas *svgcanvas=(SvgCanvas*)createTool(SVGCANVAS, work_space);
		svgcanvas->show();
		svgcanvas->setCanvasNumber(number);
		svgcanvas->line_ready(line);
	}
}

void Main::help_octave()
{
	if(get_config("qtinfo_ok").isEmpty() || get_config("qtinfo_ok")=="false" )
	{
		 oc->command_enter("qtinfo");
	}
	else
	{
		Help *help=(Help*)createTool(HELP,work_space);
	
		if(get_config("help_path").isEmpty()) help->setSource( HELP_PATH );
		else help->setSource(get_config("help_path"));
		//printf("[Main::help_octave] %s\n", HELP_PATH);
		help->show();
	}
}

void Main::help_qtoctave()
{
	Help *help=(Help*)createTool(HELP,work_space);

	if(get_config("qtoctave_help_path").isEmpty()) help->setSource( QTOCTAVE_HELP_PATH );
	else help->setSource(get_config("qtoctave_help_path"));
	help->setWindowTitle("QtOctave Help");
	help->show();
}

void Main::help_qtoctave_about()
{
	Help *help=(Help*)createTool(HELP,work_space);

	QFileInfo path(QTOCTAVE_HELP_PATH);
	if(get_config("qtoctave_help_path").isEmpty()) help->setSource( path.absoluteDir().path()+QDir::separator()+"about.html");
	else help->setSource(get_config("qtoctave_help_path"),"about");
	help->setWindowTitle("QtOctave About");
	help->show();
}


void Main::table(QString text)
{
	bool ok=true;
	if(text.isEmpty())
	{
		text = QInputDialog::getText(main_window, tr("Select table"),
			tr("Matrix name:"), QLineEdit::Normal, "", &ok);
	}
	if (ok && !text.isEmpty())
	{
		Table *table=(Table*)createTool(TABLE, work_space);

		table->setMatrix(text);
		table->show();
		table->windowActivated();
	}
}

void Main::run_file()
{
  QFileDialog openDialog(NULL, tr("Open"), ".");
  QStringList filters;

  filters << "Octave (*.m; *.M)";

  openDialog.setFilters(filters);
  openDialog.setAcceptMode(QFileDialog::AcceptOpen);
  openDialog.setDefaultSuffix("m");

  if(openDialog.exec() == QDialog::Accepted)
  {
    QFileInfo fileInfo(openDialog.selectedFiles().first());
    QString cmd;
    //OctaveConnection *oc = terminal->getOctaveConnection();

    // Change dir
    cmd = QString("cd \"") + fileInfo.dir().absolutePath() + QString("\"");
    oc->command_enter(cmd);

    // Execute file
    cmd = fileInfo.baseName();
    oc->command_enter(cmd);
  }
}

void Main::variable_list()
{
	VariableList *variableList=(VariableList*)createTool(VARIABLESLIST, work_space);
	variableList->show();
}

void Main::dynamic_help()
{
	Dynamic_help *dynamic_help=(Dynamic_help *)createTool(DYNAMIC_HELP, work_space);
	dynamic_help->show();
}

void Main::commands_list()
{
	CommandList *command_list=(CommandList*)createTool(COMMANDLIST, work_space);
	command_list->show();
}

void Main::editor_callback()
{
  if( get_config("external_editor") != "true")
  {
	Editor *editor=(Editor*)createTool(EDITOR, work_space);
	editor->show();
  }
  else
  {
    const QString editor=get_config("editor");
    if(editor.isEmpty()) return;

    QString ed(editor);

    QProcess::startDetached (ed);
  }
}

void Main::svgcanvas_callback()
{
	SvgCanvas *svgcanvas=(SvgCanvas*)createTool(SVGCANVAS, work_space);
	svgcanvas->show();
}

/** set visible or not the dock of var list*/
void Main::setVisibleVarList()
{
	main_window->dockListVar->
		setVisible(!main_window->dockListVar->isVisible());
}

/**this function show or not the Navigator dock*/
void Main::setVisibleNavigator()
{
	Navigator *nav=(Navigator *)createTool(NAVIGATOR, work_space);
	nav->show();
}

QWidget *Main::mainWindowWidget()
{
	return main_window;
}

void Main::clear_terminal()
{
	Terminal *terminal = (Terminal*)(session.getFirstTool(TERMINAL));
	terminal->clear_callback();
}

BaseWidget *Main::createTool(WidgetType type, QWidget *parent)
{
	BaseWidget *w;
	switch(type)
	{
		case TERMINAL:
		{
			Terminal *terminal=new Terminal(parent);
			terminal->setSession(&session);
			w=terminal;
		}
		break;
		case HELP:
		{
			Help *help=new Help(parent);
			help->setSession(&session);
			w=help;
			break;
		}
		case TABLE:
		{
			Table *table=new Table(parent);
			table->setOctaveConnection(oc);
			table->setSession(&session);
			w=table;
			break;
		}
		case VARIABLESLIST:
		{
			VariableList *variableList=new VariableList(parent);
			variableList->setOctaveConnection(oc);
			connect(variableList, SIGNAL(open_table(QString)), this, SLOT(table(QString)) );
			variableList->setSession(&session);
			variableList->send_whos_command_to_octave();
			w=variableList;
			break;
		}
		case DYNAMIC_HELP:
		{
			Dynamic_help *dynamic_help=new Dynamic_help(oc->getOctavePath(), parent);
			dynamic_help->setSession(&session);
			w=dynamic_help;
			break;
		}
		case EDITOR:
		{
			//if( get_config("external_editor") != "true")
			//{
				Editor *editor = new Editor(parent);
				editor->setSession(&session);
				editor->setOctaveConnection(oc);
				w=editor;
			//}
			break;
		}
		case NAVIGATOR:
		{
			Navigator *nav=new Navigator(parent);
			nav->setOctaveConnection(oc);
			nav->setSession(&session);
			w=nav;
			break;
		}
		case COMMANDLIST:
		{
			CommandList *command_list=new CommandList(parent);
			Autocomplete *line=((Terminal*)session.getFirstTool(TERMINAL))->getAutocomplete();
			connect(line, SIGNAL(new_command_entered(QStringList)), command_list, SLOT(set_list(QStringList)) );
			command_list->setSession(&session);
			command_list->setLineEdit(line);
			command_list->set_list(line->commands());
			w=command_list;
			break;
		}
		case SVGCANVAS:
		{
			SvgCanvas *svgcanvas = new SvgCanvas(parent);
			svgcanvas->setOctaveConnection(oc);
			svgcanvas->setSession(&session);
			QVector<QObject*> tools=session.getTools(SVGCANVAS);
			int min=1, num;
			for(int i=0;i<tools.size();i++)
			{
				for(int j=0;j<tools.size();j++)
				{
					num=((SvgCanvas*)tools[j])->getCanvasNumber();
					if(num==min) min++;
				}
			}
			svgcanvas->setCanvasNumber(min);
			w=svgcanvas;
			break;
		}
		default:
			return NULL;
	}

	if(parent==work_space)
	{//Add window in workspace
		QMdiSubWindow *mdi=work_space->addSubWindow(w);
		mdi->setAttribute(Qt::WA_DeleteOnClose);
		mdi->setWindowIcon(w->windowIcon());
	}

	return w;
}

void Main::openTools(QXmlStreamReader &xml, QString config_name)
{
	QList<BaseWidget*> tools;
	QHash<QString, WidgetType> tools_type;
	QWidget *parent;
	BaseWidget *bw;

	tools_type["terminal"]=TERMINAL;
	tools_type["help"]=HELP;
	tools_type["table"]=TABLE;
	tools_type["dynamic_help"]=DYNAMIC_HELP;
	tools_type["editor"]=EDITOR;
	tools_type["navigator"]=NAVIGATOR;
	tools_type["variables_list"]=VARIABLESLIST;
	tools_type["command_list"]=COMMANDLIST;
	tools_type["main_window"]=MAINWINDOW;
	tools_type["svgcanvas"]=SVGCANVAS;

	while (!xml.atEnd())
	{
		xml.readNext();
		if( xml.isStartElement() )
		{
			if(xml.name()=="tool")
			{
				QXmlStreamAttributes attr=xml.attributes();

				QString type=attr.value("type").toString();
				if(!tools_type.contains(type))
				{
					printf("Type %s isn't in database\n", type.toLocal8Bit().data());
					return;
				}

				QString place=attr.value("place").toString();

				if(place=="window") parent=NULL;
				else parent=work_space; // place=="workspace"

				if(type=="terminal")
				{
					bw=(BaseWidget*)session.getFirstTool(TERMINAL);
				}
				else if(type=="main_window")
				{
					bw=(BaseWidget*)session.getFirstTool(MAINWINDOW);
				}
				else if(tools.isEmpty())
				{
					bw=createTool(tools_type[type], parent);
				}
				else
				{
					bw=createTool(tools_type[type], tools.last());
				}

				QString title=attr.value("title").toString();
				if( !title.isEmpty() ) bw->setWindowTitle(title);
				
				if(place!="window") //place=="workspace"
				{
					int x=0;
					if( attr.hasAttribute("xPosition") )
						x=attr.value("xPosition").toString().toInt();
					int y=0;
					if( attr.hasAttribute("yPosition") )
						y=attr.value("yPosition").toString().toInt();
					int width=300;
					if( attr.hasAttribute("width") )
						width=attr.value("width").toString().toInt();
					int height=300;
					if( attr.hasAttribute("height") )
						height=attr.value("height").toString().toInt();
					bool maximized=false;
					if( attr.hasAttribute("maximized") )
						maximized=(attr.value("maximized").toString()=="true");
					bool minimized=false;
					if( attr.hasAttribute("minimized") )
						minimized=(attr.value("minimized").toString()=="true");
					if(bw!=NULL)
					{
						InitialPosition initPos;
						initPos.x=x;
						initPos.y=y;
						if(width<=0) width=100;
						if(height<=0) height=100;
						initPos.width=width;
						initPos.height=height;
						initPos.maximized=maximized;
						initPos.minimized=minimized;
						initPos.widget=bw->parentWidget();
						if(initPos.widget!=NULL)
							initialPositionList.append(initPos);
						//bw->show();
						//bw->move(x,y);
						//bw->resize(width, height);
					}
				}

				tools.append(bw);

				switch( bw->widgetType() )
				{
					case HELP:
					{
						{
							Help *help=(Help*)bw;
							if(get_config("help_path").isEmpty()) help->setSource( HELP_PATH );
							else help->setSource(get_config("help_path"));
						}
						break;
					}
					default:
					;
				}
			}
			else if(!tools.isEmpty())
			{
				QXmlStreamAttributes attr=xml.attributes();
				switch( tools.last()->widgetType() )
				{
					case TABLE:
					{
						Table* table=(Table*)tools.last();
						if(xml.name()=="matrix")
						{
							QString value=attr.value("value").toString();
							table->setMatrix(value);
							table->windowActivated();
						}
						break;
					}
					default:
					;
				}

				if(xml.name()=="state")
				{
					QString state=xml.readElementText();
					tools.last()->restoreState( QByteArray::fromHex(state.toAscii()) );
				}
			}
			else if(xml.name()=="tools_config" && !config_name.isEmpty() )
			{
				QXmlStreamAttributes attr=xml.attributes();

				QString name=attr.value("name").toString();

				if(name!=config_name)
				{
					while (!xml.atEnd())
					{
						xml.readNext();
						if( xml.isStartElement() )
						{
							if(xml.name()=="tools_config")
							{
								QXmlStreamAttributes attr=xml.attributes();

								QString name=attr.value("name").toString();

								if(name==config_name) break;
							}
						}
					}
				}
			}
		}
		else if( xml.isEndElement() )
		{
			if( xml.name()=="tool" /*&& !tools.isEmpty()*/ )
			{
				BaseWidget *bw=tools.last();
				tools.removeLast();
				if(!tools.isEmpty() && bw->widgetType()!=TERMINAL && bw->widgetType()!=MAINWINDOW)
					tools.last()->addDock(bw);
				bw->show();
			}
		}
	}
	connect(&timer, SIGNAL(timeout()), this, SLOT(initialPosition_callback()));
	timer.setSingleShot(true);
	timer.start(5000);
}


void Main::initialPosition_callback()
{
	foreach(InitialPosition i, initialPositionList)
	{
		i.widget->move(i.x,i.y);
		i.widget->resize(i.width, i.height);
		if(i.maximized) i.widget->showMaximized();
		if(i.minimized) i.widget->showMinimized();
	}
	
	main_window->showMaximized();
}


void Main::openFilesToEdit(QStringList files)
{
	if(files.isEmpty()) return;
	BaseWidget *bw=(BaseWidget*)session.getFirstTool(EDITOR);
	if(bw==NULL) bw=createTool(EDITOR, NULL);
	Editor *editor=(Editor*)bw;
	editor->loadFiles(files);
	editor->show();
}

#include <QSplashScreen>
#include <QTimer>

/** Process comand line looking for files to open.
 */
static QStringList command_line_find_files(int argn, char *argv[])
{
	QStringList files;
	
	for(int i=1;i<argn;i++)
	{
		files.append(argv[i]);
	}
	
	return files;
}

int main(int argn, char *argv[])
{
	QApplication a(argn,argv);
	//Se inicializa la configuración
	get_config("");
	//Show splash screen
	QSplashScreen *splash = new QSplashScreen( QPixmap(QString(ICON_PATH) +QDir::separator()+ "splash.png")/*,  Qt::WindowStaysOnTopHint*/ );
	splash->showMessage ("Starting QtOctave",Qt::AlignBottom|Qt::AlignLeft);
	splash->show();

	// Translations
	QString transFile;
	QTranslator qtTranslator, qtoctaveTranslator;

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	// Qt translations
	QString locales=QLocale::system().name();
	//if(locales.length()>2)
	//{
	//	locales.resize(2);
	//}
		
	if(qtTranslator.load("qt_" + QLocale::system().name(),
			     //"/usr/share/qt4/translations"
			     QLibraryInfo::location(QLibraryInfo::TranslationsPath)
			     ) )
	{
	  a.installTranslator(&qtTranslator);
	  printf("[main()] Loaded QT Translation file for locale '%s'.\n",
		 QLocale::system().name().toLocal8Bit().data());
	}
	else
	{
	  printf("[main()] Error loading the QT Translation file for locale '%s'.\n",
		 QLocale::system().name().toLocal8Bit().data());
	}

	// QtOctave translations
	if(qtoctaveTranslator.load("qtoctave_" + locales, LANG_PATH))
	{
	  a.installTranslator(&qtoctaveTranslator);
	  printf("[main()] Loaded translation file for locale '%s'.\n",
		 QLocale::system().name().toLocal8Bit().data());
	}
	else
	{
	  printf("[main()] Error loading the translation file for locale '%s'. Not found in %s \n",
		 locales.toLocal8Bit().data(), LANG_PATH);
	}

	// Load
	a.processEvents();
	Main m;
	
	//Open command line files
	m.openFilesToEdit(command_line_find_files(argn,argv));
	
	//Destroy splash screnn
	splash->finish(m.mainWindowWidget());
	delete splash;
	return a.exec();
}
