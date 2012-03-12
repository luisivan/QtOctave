#ifndef __MENUBAR_H__
#define __MENUBAR_H__

#include "process.h"
#include <QStringList>
#include <QMenuBar>

class MenuBar:public Widget
{
		Q_OBJECT
	public:
		QList<QMenu *> menu;
};

class ActionEvent: public QObject
{
	Q_OBJECT
	public:
		QString name;
	public slots:
		void triggered();
	signals:
		void triggered_event(QString e);
};

#endif
