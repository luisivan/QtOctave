#ifndef __LIST_H__
#define __LIST_H__

#include "process.h"
#include <QStringList>
#include <QTreeWidgetItem>
#include <QList>

class List:public Widget
{
		Q_OBJECT
	public:
		QStringList headers, items;
		QList<QTreeWidgetItem *> item;
		QList<QStringList> items_list;
	public slots:
		void selection_changed();
	signals:
		void selection_changed_event(QString e);
};

#endif
