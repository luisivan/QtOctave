#ifndef __SESSION_H__
#define __SESSION_H__

#include <QObject>
#include <QVector>
#include <QHash>

/**Types of widgets.*/
enum WidgetType
{
MAIN,MAINWINDOW,TERMINAL,TABLE,HELP,DYNAMIC_HELP,
EDITOR,NAVIGATOR,VARIABLESLIST,COMMANDLIST,SVGCANVAS,
NONE
};

QString widgetName(WidgetType type);

/**Generic config of session.*/
class Session:public QObject
{
	Q_OBJECT
	QString project_name;
	QHash < WidgetType, QVector<QObject*> > tools;
	public:
	
	Session(QObject * parent = 0);
	
	/**Actual project name.*/
	inline QString getProjectName() {return project_name;};
	
	/**Sets actual project name.*/
	void setProjectName(QString project_name);
	
	/**Add new tool to session.*/
	void addTool(WidgetType type, QObject *tool);
	
	/**Get tools of type.*/
	QVector<QObject*> getTools(WidgetType type);
	
	/**Get all tools.*/
	QVector<QObject*> getTools();
	
	/**Get 1st tool of this type.*/
	QObject *getFirstTool(WidgetType type);
	
	/**Remove tool.*/
	void removeTool(QObject *tool);
	
	signals:
	
	/**New tool added.*/
	void newTool(WidgetType type, QObject *tool);
	
	/**Project changed.*/
	void projectChanged(QString project_name);
};

#endif
