#include "projects.h"
#include "ui_newprojectdialog.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QHash>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QMenuBar>
#include <QDateTime>

/////////////////////////////////////////////////////////////

struct Project
{
	QStringList files;
	QString description;
	QString last_navigator_path, variables_list_path;
	QStringList command_history;
};

/////////////////////////////////////////////////////////////
// Utils

/**Gives projects.xml path*/
static inline QString _projects_path()
{
	QString home=QDir::home().path()+"/.qtoctave/projects.xml";
	return home;
}

/////////////////////////////////////////////////////////////

/**Load projects.xml*/
static QHash<QString, Project> _load_projects()
{
	QHash<QString, Project> projects;
	QFile file(_projects_path());
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return projects;
	
	QXmlStreamReader xml(&file);
	QString last_project;
	//Parse xml file
	while (!xml.atEnd())
	{
		xml.readNext();
		if( xml.isStartElement() )
		{
			QString name=xml.name().toString();
			QXmlStreamAttributes attrs=xml.attributes();
			if(name=="project")
			{
				last_project=attrs.value("name").toString().trimmed();
			}
			else if(name=="file")
				projects[last_project].files << xml.readElementText().trimmed();
			else if(name=="command_history")
				projects[last_project].command_history << xml.readElementText().trimmed();
			else if(name=="last_navigator_path")
				projects[last_project].last_navigator_path = xml.readElementText().trimmed();
			else if(name=="variables_list_path")
				projects[last_project].variables_list_path = xml.readElementText().trimmed();
			else if(name=="description")
			{
				projects[last_project].description = xml.readElementText();
			}
		}
		else if( xml.isEndElement() )
		{
			QString name=xml.name().toString();
			last_project.clear();
		}
	}
	//if (xml.hasError()){ }
	
	file.close();
	
	return projects;
}

/////////////////////////////////////////////////////////////

/**Save projects.xml*/
static void _save_projects(QHash<QString, Project> projects)
{
	QFile file(_projects_path());
	
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return ;
	
	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);
	
	xml.writeStartDocument();
	
	xml.writeStartElement("","projects");
	
	QString key;
	foreach(key, projects.keys())
	{
		xml.writeStartElement("","project");
		xml.writeAttribute("", "name", key);
		xml.writeTextElement("", "description", projects[key].description);
		xml.writeTextElement("", "last_navigator_path", projects[key].last_navigator_path);
		xml.writeTextElement("", "variables_list_path", projects[key].variables_list_path);
		QString path; QStringList files=projects[key].files;
		foreach(path,files)
		{
			xml.writeTextElement("", "file", path);
			//xml.writeStartElement("","file");
			//xml.writeAttribute("", "name", path);
			//xml.writeEndElement();
		}
		foreach(QString command, projects[key].command_history)
		{
			xml.writeTextElement("", "command_history", command);
		}
		xml.writeEndElement();
	}
	
	xml.writeEndElement();
	xml.writeEndDocument();
	
	file.close();
}

/////////////////////////////////////////////////////////////

Projects::Projects(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	
	ui.projects_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.open_pushButton, SIGNAL(clicked()), this, SLOT(activate_project_callback()));
	//connect(ui.projects_listWidget, SIGNAL( customContextMenuRequested ( const QPoint & )  ), this, SLOT( contextMenu_cb( const QPoint & ) ) );
	//connect(ui.projects_listWidget, SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem *)), this, SLOT(show_description_callback(QListWidgetItem *, QListWidgetItem *)));
	//connect(ui.projects_listWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(activate_project_callback(QListWidgetItem *)));
	
	context_menu=new QMenu("Projects",this);
	
	new_project=new QAction(tr("New Project"), this);
	connect(new_project, SIGNAL(triggered()), this, SLOT(new_button_callback()) );
	modify_project=new QAction(tr("Modify Project"), this);
	connect(modify_project, SIGNAL(triggered()), this, SLOT(modify_project_callback()) );
	delete_project=new QAction(tr("Delete Project"), this);
	connect(delete_project, SIGNAL(triggered()), this, SLOT(delete_project_callback()) );
	
	QList<QAction *> actions;
	actions.append(new_project);
	actions.append(modify_project);
	actions.append(delete_project);
	
	context_menu->addActions(actions);
	
	QHBoxLayout *layout=new QHBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	ui.menu_widget->setLayout(layout);
	
	QMenuBar *menu_bar=new QMenuBar(this);
	layout->addWidget(menu_bar);
	menu_bar->show();
	
	menu_bar->addMenu(context_menu);
	
	fill_projects_list();
}

/////////////////////////////////////////////////////////////

Projects::~Projects()
{
	//delete context_menu;
	//delete new_project;
}

/////////////////////////////////////////////////////////////

void Projects::fill_projects_list()
{
	QHash<QString, Project> projects=_load_projects();
	
	QString key;
	QStringList list;
	foreach(key, projects.keys())
	{
		list << key;
	}
	
	disconnect(ui.projects_listWidget, SIGNAL( customContextMenuRequested ( const QPoint & )  ), this, SLOT( contextMenu_cb( const QPoint & ) ) );
	disconnect(ui.projects_listWidget, SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem *)), this, SLOT(show_description_callback(QListWidgetItem *, QListWidgetItem *)));
	disconnect(ui.projects_listWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(activate_project_callback(QListWidgetItem *)));
	
	ui.projects_listWidget->clear();
	
	connect(ui.projects_listWidget, SIGNAL( customContextMenuRequested ( const QPoint & )  ), this, SLOT( contextMenu_cb( const QPoint & ) ) );
	connect(ui.projects_listWidget, SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem *)), this, SLOT(show_description_callback(QListWidgetItem *, QListWidgetItem *)));
	connect(ui.projects_listWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(activate_project_callback(QListWidgetItem *)));
	
	ui.projects_listWidget->addItems(list);
}

/////////////////////////////////////////////////////////////

QStringList Projects::listFiles(QString project_name)
{
	QHash<QString, Project> projects=_load_projects();
	return projects[project_name].files;
}

/////////////////////////////////////////////////////////////

void Projects::saveListFiles(const QString & project_name, const QStringList & files)
{
	QHash<QString, Project> projects=_load_projects();
	projects[project_name].files=files;
	_save_projects(projects);
}

/////////////////////////////////////////////////////////////

QStringList Projects::listCommandHistory(QString project_name)
{
	QHash<QString, Project> projects=_load_projects();
	return projects[project_name].command_history;
}

/////////////////////////////////////////////////////////////

void Projects::saveListCommandHistory(const QString & project_name, const QStringList & command_history)
{
	QHash<QString, Project> projects=_load_projects();
	projects[project_name].command_history=command_history;
	_save_projects(projects);
}

/////////////////////////////////////////////////////////////

QString Projects::navigatorPath(QString project_name)
{
	QHash<QString, Project> projects=_load_projects();
	return projects[project_name].last_navigator_path;
}

/////////////////////////////////////////////////////////////

QString Projects::variablesListPath(QString project_name)
{
	QHash<QString, Project> projects=_load_projects();
	return projects[project_name].variables_list_path;
}

/////////////////////////////////////////////////////////////

void Projects::saveVariablesListPath(QString project_name, QString path)
{
	QHash<QString, Project> projects=_load_projects();
	projects[project_name].variables_list_path=path;
	_save_projects(projects);
	
	QString home=QDir::home().path()+"/.qtoctave/";
	QDir dir(home);
	
	QStringList filter;
	filter << "*.variables";
	QStringList list=dir.entryList(filter);
	
	for(int i=0;i<list.size();i++)
	{
		QString file=list[i];
		
		bool delete_ok=true;
		
		Project p;
		foreach(p, projects)
		{
			if( p.variables_list_path==file ) delete_ok=false;
		}
		
		if(delete_ok)
			dir.remove(file);
	}
}

/////////////////////////////////////////////////////////////

void Projects::saveNavigatorPath(QString project_name, QString path)
{
	QHash<QString, Project> projects=_load_projects();
	projects[project_name].last_navigator_path=path;
	_save_projects(projects);
}

/////////////////////////////////////////////////////////////

QString Projects::projectName()
{
	return project_name;
}

/////////////////////////////////////////////////////////////

void Projects::new_button_callback()
{
	QDialog dialog(this);
	Ui_NewProjectDialog ui_np;
	ui_np.setupUi(&dialog);
	
	if( QDialog::Accepted==dialog.exec() )
	{
		QString name=ui_np.name_lineEdit->text();
		if(name.isEmpty()) return;
		QString description=ui_np.description_textEdit->toPlainText();
		QHash<QString, Project> projects=_load_projects();
		projects[name].description=description;
		_save_projects(projects);
		project_name=name;
		accept();
	}
}

/////////////////////////////////////////////////////////////

void Projects::modify_project_callback()
{
	QDialog dialog(this);
	Ui_NewProjectDialog ui_np;
	ui_np.setupUi(&dialog);
	dialog.setWindowTitle(tr("Modify Project"));
	
	QHash<QString, Project> projects=_load_projects();
	QString name=ui.projects_listWidget->currentItem()->text();
	
	ui_np.description_textEdit->setPlainText(projects[name].description);
	ui_np.name_lineEdit->setText(name);
	
	if( QDialog::Accepted==dialog.exec() )
	{
		QString name2=ui_np.name_lineEdit->text().trimmed();
		if(name2.isEmpty()) return;
		projects[name2]=projects[name];
		QString description=ui_np.description_textEdit->toPlainText();
		projects[name2].description=description;
		if(name2!=name)
		{
			projects.remove(name);
		}
		
		_save_projects(projects);
		
		fill_projects_list();
	}
}

/////////////////////////////////////////////////////////////

void Projects::delete_project_callback()
{
	QString name=ui.projects_listWidget->currentItem()->text();
	
	QString message="Project "+name+" will be erased. Continue?" ;
	
	 int ret = QMessageBox::warning(this, tr("Delete Project"),
		tr( message.toLocal8Bit().data() ),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No);
	
	if(ret!=QMessageBox::Yes) return;
	
	QHash<QString, Project> projects=_load_projects();
	projects.remove(name);
	_save_projects(projects);
	
	fill_projects_list();
}

/////////////////////////////////////////////////////////////

void Projects::activate_project_callback(QListWidgetItem * item)
{
	if(item==NULL)
	{
		item=ui.projects_listWidget->currentItem();
		if(item==NULL) return;
	}
	QString name=item->text();
	QHash<QString, Project> projects=_load_projects();
	project_name=name;
	accept();
}

/////////////////////////////////////////////////////////////

void Projects::show_description_callback(QListWidgetItem * current, QListWidgetItem * /*previous*/ )
{
	QString name=current->text();
	QHash<QString, Project> projects=_load_projects();
	ui.description_textEdit->setPlainText(projects[name].description);
}

/////////////////////////////////////////////////////////////

void Projects::contextMenu_cb( const QPoint & /*pos*/)
{
	context_menu->popup(QCursor::pos () );
}

