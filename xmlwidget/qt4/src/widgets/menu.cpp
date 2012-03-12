#include "menu.h"
#include <QStringList>

Widget *new_menu(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "menu") return NULL;
	
	Menu *w=new Menu();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	w->widget=new mMenu(parent);
	w->type=MENU;
	(( mMenu*)(w->widget))->setWidget((Widget*)w);
	w->item=NULL;
	return (Widget *)w;
}

//////////////////////////////////////////////////////////////////////////////////////////

Menu::Menu():Widget()
{
	last_item=NULL;
}


bool Menu::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando Menu %s\n", name.toLocal8Bit().data());
	mMenu *w=(mMenu *)widget;
	if(qName=="menu" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		return true;
	}
	
	else if(qName=="item")
	{
		
		QString item_id=atts.value("id");
		if( !item_id.isEmpty() )
		{
			MenuItem *parent=item;
			
			if( items.contains(item_id) )
				item=items[item_id];
			else
			{
				item=new MenuItem;
				item->action=new QAction(w);
				parent->menu->addAction(item->action);
				items[item_id]=item;
			}
			
			QString name=atts.value("name");
			if( !name.isEmpty() )
			{
				item->action->setText(name);
				QStringList data;
				data << item_id;
				item->action->setData(QVariant(data));
			}
			
			last_item=item;
			item=parent;
			//item_stack.push(item);
		}
		else
		{
			return false;
		}
		return true;
	}
	
	else if(qName=="menuitem")
	{
		last_item=NULL;
		QString item_id=atts.value("id");
		if( !item_id.isEmpty() )
		{
			if( items.contains(item_id) )
				item=items[item_id];
			else
			{
				MenuItem *parent=item;
				item=new MenuItem;
				item->menu=new QMenu(w);
				if(parent==NULL)
				{
					
					w->addMenu(item->menu);
				}
				else
				{
					parent->menu->addMenu(item->menu);
				}
				
				//item->menu->connect(item->menu, SIGNAL(triggered ( QAction * ) ), w, SLOT(clicked_cb(QAction *)) );
				items[item_id]=item;
			}
			
			
			QString name=atts.value("name");
			if( !name.isEmpty() )
			{
				item->menu->setTitle(name);
			}
			
			item_stack.push(item);
		}
		else
		{
			return false;
		}
		return true;
	}
	
	return false;
}

bool Menu::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(type!=MENU)
	{
		return false;
	}
	
	if(qName=="menu")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="item")
	{
		if(last_item!=NULL && !text.isEmpty())
		{
			QStringList data=last_item->action->data().toStringList();
			if(data.size()<2) data << text;
			else
			{
				QString name=data[0];
				data.clear();
				data << name;
				data << text;
			}
			last_item->action->setData(QVariant(data));
			last_item=NULL;
		}
		property_ok=true;
		return true;
	}
	else if(qName=="menuitem")
	{
		if(item_stack.size()>0)
		{
			item_stack.pop();
			if(item_stack.size()>0) item=item_stack.top();
			else item=NULL;
		}
		property_ok=true;
		return true;
	}
	
	return false;
}

void Menu::add_widget(Widget */*w*/)
{
}

Menu::~Menu()
{
}

mMenu::mMenu( QWidget * parent):QMenuBar(parent)
{
	connect(this, SIGNAL(triggered ( QAction * ) ), this, SLOT(clicked_cb(QAction *)) );
}


void mMenu::setWidget(Widget *w)
{
	this->w=w;
}


void mMenu::clicked_cb(QAction *action)
{
	QStringList data=action->data().toStringList();
	QString item_id=data[0];
	if(data.size()<2)
		w->output->write("<menu id=\""+w->name+"\"><item id=\""+item_id+"\"/></menu>");
	else
		w->output->write(data[1]);
}
