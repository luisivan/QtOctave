/* Copyright (C) 2007 P.L. Lucas
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


#include "tree.h"


Widget *new_tree(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "tree") return NULL;
	
	Tree *w=new Tree();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	w->widget=new mTree(parent);
	w->type=TREE;
	(( mTree*)(w->widget))->setWidget((Widget*)w);
	w->item=NULL;
	w->col_count=0;
	w->new_headers=false;
	return (Widget *)w;
}

Tree::Tree():Widget()
{
	get_item_property_ok=false;
	signal_property_ok=false;
}

bool Tree::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando tree %s\n", name.toLocal8Bit().data());
	mTree *w=(mTree *)widget;
	
	if(signal_property_ok)
	{//Process signal contents
		QString value=atts.value("value");
		if(!value.isEmpty())
		{
			if(qName=="startmark") w->startmark=value;
			else if(qName=="endmark") w->endmark=value;
			else if(qName=="linestartmark") w->linestartmark=value;
		}
		return true;
	}
	else if(get_item_property_ok)
	{//Process get_item contents
		QString value=atts.value("value");
		if(!value.isEmpty())
		{
			if(qName=="startmark") startmark=value;
			else if(qName=="endmark") endmark=value;
			else if(qName=="linestartmark") linestartmark=value;
		}
		return true;
	}
	else if(qName=="tree" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		return true;
	}
	else if(qName=="header")
	{
		QString header=atts.value("name");
		if( !header.isEmpty() )
		{
			headers << header.trimmed();
			new_headers=true;
		}
		return true;
	}
	else if(qName=="item")
	{
		if(new_headers)
		{
			w->setHeaderLabels(headers);
			new_headers=false;
			for(int i=0;i<headers.size();i++)
			{
				w->setColumnWidth(i,100);
			}
		}
		item_stack.push(item);
		QString item_id=atts.value("id");
		if( !item_id.isEmpty() )
		{
			if( items.contains(item_id) )
				item=items[item_id];
			else
			{
				if(item==NULL) item=new QTreeWidgetItem( (QTreeWidget*)widget );
				else item=new QTreeWidgetItem( item );
				item->setData(0,4,QVariant(item_id) );
				items[item_id]=item;
			}
			col_count=0;
		}
		else
		{
			if(item==NULL) item=new QTreeWidgetItem( (QTreeWidget*)widget );
			else item=new QTreeWidgetItem( item );
		}
		col_count=0;
		return true;
	}
	else if(qName=="col")
	{
		QString value=atts.value("value");
		if( !value.isEmpty() )
		{
			item->setText(col_count,value.trimmed());
			col_count++;
		}
		return true;
	}
	else if(qName=="get_item")
	{
		get_item_property_ok=true;
		item_id=atts.value("id");
		return true;
	}
	else if(qName=="signal")
	{
		signal_property_ok=true;
		w->startmark="";
		w->endmark="";
		w->linestartmark="";
		QString signal=atts.value("name");
		if(signal=="rowclicked")
			QObject::connect((mTree*)widget, SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int ) ), (mTree*)widget, SLOT(double_clicked_cb(QTreeWidgetItem * , int)) );
		return true;
	}
	else if(qName=="clear")
	{
		w->clear();
		items.clear();
		return true;
	}
	
	return false;
}

bool Tree::xml_end(const QString & qName, const QString &/*text*/, bool &property_ok)
{
	if(type!=TREE)
	{
		return false;
	}
	//mTree *w=(mTree *)widget;
	if(qName=="tree")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="item")
	{
		if( item_stack.size()>0 )
			item=item_stack.pop();
		else
			item=NULL;
		property_ok=true;
		return true;
	}
	else if(qName=="col")
	{
		property_ok=true;
		return true;
	}
	else if(qName=="get_item")
	{
		if( !item_id.isEmpty() )
		{
			if( items.contains(item_id) )
			{
				QTreeWidgetItem *_item=items[item_id];
				if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
				{
					output->write("<tree id=\""+name+"\">");
					output->write("<item id=\""+item_id+"\">");
					for(int i=0;i<headers.size();i++)
					{
						output->write("<col value=\""+_item->text(i)+"\"/>");
					}
					output->write("</item>\n</tree>");
				}
				else
				{
					output->write(startmark);
					for(int i=0;i<headers.size();i++)
					{
						output->write(linestartmark+_item->text(i));
					}
					output->write(endmark);
					startmark=linestartmark=endmark="";
				}
			}
		}
		get_item_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(qName=="header")
	{
		property_ok=true;
		return true;
	}
	else if(qName=="signal")
	{
		signal_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(qName=="clear")
	{
		property_ok=true;
		return true;
	}
	else if(get_item_property_ok || signal_property_ok)
	{
		if(qName=="startmark" || qName=="endmark" || qName=="linestartmark")
		{
			property_ok=true;
			return true;
		}
	}
	
	return false;
}


void Tree::add_widget(Widget */*w*/)
{
	;
}



mTree::mTree( QWidget * parent):QTreeWidget(parent)
{
}


void mTree::setWidget(Widget *w)
{
	this->w=w;
}


void mTree::double_clicked_cb(QTreeWidgetItem *item , int /*col*/)
{
	QString item_id=item->data(0,4).toString();
	if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
	{
		w->output->write("<tree id=\""+w->name+"\">");
		w->output->write("<item id=\""+item_id+"\">");
		for(int i=0;i<((Tree*)w)->headers.size();i++)
		{
			w->output->write("<col value=\""+item->text(i)+"\"/>");
		}
		w->output->write("</item>\n</tree>");
	}
	else
	{
		w->output->write(startmark);
		w->output->write(linestartmark+item_id);
		for(int i=0;i<((Tree*)w)->headers.size();i++)
		{
			w->output->write(linestartmark+item->text(i));
		}
		w->output->write(endmark);
	}
}

