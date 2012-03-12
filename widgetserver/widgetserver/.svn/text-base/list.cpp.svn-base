#include "list.h"

void List::selection_changed()
{
	QList<QTreeWidgetItem *> items_selected=((QTreeWidget*)widget)->selectedItems ();
	for(int i=0;i<items_selected.size();i++)
	{
		QTreeWidgetItem *is=items_selected.at(i);
		QString item_name=is->data(0,1).toString();
		emit selection_changed_event("*selection_changed:"+name+":"+item_name+"\n");
	}
}
