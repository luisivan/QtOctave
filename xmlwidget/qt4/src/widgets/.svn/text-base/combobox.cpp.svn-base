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

#include "combobox.h"

#include <QMessageBox>

Widget *new_combobox(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "combobox") return NULL;
	
	ComboBox *w=new ComboBox();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	mComboBox *combo=new mComboBox(parent);
	w->widget=combo;
	combo->w=w;
	w->type=COMBOBOX;
	
	return (Widget *)w;
}

ComboBox::ComboBox():Widget()
{
	text_property_ok=signal_property_ok=false;
}

bool ComboBox::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando botón %s\n",qName.toLocal8Bit().data() );
	mComboBox *w=(mComboBox *)widget;
	if(qName=="combobox" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		return true;
	}
	else if(qName=="item")
	{
		return true;
	}
	else if(qName=="signal")
	{
		QString value=atts.value("name");
		if(value=="activated")
		{
			signal_property_ok=true;
			w->startmark=w->linestartmark=w->endmark="";
			w->disconnect(w,SIGNAL(activated(const QString &)), w, SLOT(activated_cb( const QString & )));
			w->connect(w,SIGNAL(activated(const QString &)), w, SLOT(activated_cb( const QString & )));
		}
		return true;
	}
	else if(qName=="get_text")
	{
		text_property_ok=true;
		return true;
	}
	else if(text_property_ok)
	{
		QString value=atts.value("value");
		if(!value.isEmpty())
		{
			if(qName=="startmark") startmark=value;
			else if(qName=="endmark") endmark=value;
			else if(qName=="linestartmark") linestartmark=value;
		}
		return true;
	}
	else if(signal_property_ok)
	{
		QString value=atts.value("value");
		if(!value.isEmpty())
		{
			if(qName=="startmark") w->startmark=value;
			else if(qName=="endmark") w->endmark=value;
			else if(qName=="linestartmark") w->linestartmark=value;
		}
		return true;
	}
	
	return false;
}

bool ComboBox::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(type!=COMBOBOX)
	{
		return false;
	}
	mComboBox *l=(mComboBox *)widget;
	if(qName=="combobox")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="item")
	{
		l->insertItem(-1, text);
		property_ok=true;
		return true;
	}
	else if(qName=="get_text")
	{
		QString text=l->currentText();
		if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
		{
			text.replace("<","&lt;");
			text.replace(">","&gt;");
			output->write("<combobox id=\""+name+"\">\n<text>"+toXML(text)+"</text>\n</combobox>\n");
		}
		else
		{
			output->write(startmark);
			output->write(linestartmark+text);
			output->write(endmark);
			startmark=linestartmark=endmark="";
		}
		text_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(qName=="signal")
	{
		signal_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(text_property_ok || signal_property_ok)
	{
		if(qName=="startmark" || qName=="endmark" || qName=="linestartmark")
		{
			property_ok=true;
			return true;
		}
	}
	
	
	return false;
}


void ComboBox::add_widget(Widget */*w*/)
{
	;
}



mComboBox::mComboBox( QWidget * parent):QComboBox(parent)
{
}

void mComboBox::activated_cb ( const QString & text_item )
{
	QString text=text_item;
	if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
	{
		text.replace("<","&lt;");
		text.replace(">","&gt;");
		w->output->write("<combobox id=\""+w->name+"\" signal=\"activated\">\n<text>"+toXML(text)+"</text>\n</combobox>\n");
	}
	else
	{
		w->output->write(startmark);
		w->output->write(linestartmark+text);
		w->output->write(endmark);
	}
}




