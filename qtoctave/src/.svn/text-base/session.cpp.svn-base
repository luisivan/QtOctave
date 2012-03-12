#include "session.h"


QString widgetName(WidgetType type)
{
	QHash <WidgetType, QString> _widgetName;
	_widgetName[MAIN]="main";
	_widgetName[TERMINAL]="terminal";
	_widgetName[HELP]="help";
	_widgetName[TABLE]="table";
	_widgetName[DYNAMIC_HELP]="dynamic_help";
	_widgetName[EDITOR]="editor";
	_widgetName[NAVIGATOR]="navigator";
	_widgetName[VARIABLESLIST]="variables_list";
	_widgetName[COMMANDLIST]="command_list";
	_widgetName[SVGCANVAS]="svgcanvas";
	_widgetName[MAINWINDOW]="main_window";
	
	return _widgetName[type];
}

Session::Session(QObject*parent):QObject (parent)
{
	//for(int i=0;i<=NONE;i++)
	//{
	//	QVector<QObject*> tool;
	//	tools.append(tool);
	//}
	
	project_name=tr("Empty");
}


void Session::setProjectName(QString project_name)
{
	this->project_name=project_name;
	emit projectChanged(project_name);
}


void Session::addTool(WidgetType type, QObject *tool)
{
	//if(type>NONE) printf("Type #%d doesn't exits.\n", type);
	if( !tools.contains(type) )
	{
		QVector<QObject*> tool;
		tools[type]=tool;
	}
	if(!tools[type].contains(tool)) tools[type].append(tool);
	emit newTool(type,tool);
}


void Session::removeTool(QObject *tool)
{
	int pos;
	QList<WidgetType> tool_type=tools.keys();
	for(int i=0;i<tool_type.size();i++)
	{
		pos=tools[tool_type[i]].indexOf(tool);
		if(-1<pos) tools[tool_type[i]].remove(pos);
	}
}

QVector<QObject*> Session::getTools(WidgetType type)
{
	return tools[type];
}


QVector<QObject*> Session::getTools()
{
	QList<QVector<QObject*> > tool_type=tools.values();
	QVector<QObject*> all_tools;
	for(int i=0;i<tool_type.size();i++)
	{
		for(int j=0;j<tool_type[i].size();j++)
			all_tools.append(tool_type[i][j]);
	}
	return all_tools;
}

QObject *Session::getFirstTool(WidgetType type)
{
	QObject *tool=NULL;
	if(tools.contains(type) && tools[type].size()>0)
	{
		tool=tools[type][0];
	}
	
	return tool;
}

