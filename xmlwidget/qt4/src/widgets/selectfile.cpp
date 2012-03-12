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

#include "selectfile.h"

#include <QMessageBox>

Widget *new_selectfile(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "selectfile") return NULL;
	
	SelectFile *w=new SelectFile();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	mSelectFile *fs=new mSelectFile(parent);
	w->widget=fs;
	fs->w=w;
	w->type=SELECTFILE;
	
	return (Widget *)w;
}

SelectFile::SelectFile():Widget()
{
	signal_property_ok=false;
}

bool SelectFile::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando botón %s\n",qName.toLocal8Bit().data() );
	mSelectFile *w=(mSelectFile *)widget;
	if(qName=="selectfile" && check_id(atts) )
	{
		widget_properties(atts,this);
		
		QString title=atts.value("title");
		if(!title.isEmpty()) widget->setWindowTitle ( atts.value("title") );
		
		return true;
	}
	else if(qName=="file")
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

bool SelectFile::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(type!=SELECTFILE)
	{
		return false;
	}
	mSelectFile *l=(mSelectFile *)widget;
	if(qName=="selectfile")
	{
		l->show();
		property_ok=false;
		return true;
	}
	else if(qName=="file")
	{
		l->selectFile(text);
		property_ok=true;
		return true;
	}
	else if(qName=="signal")
	{
		signal_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(signal_property_ok)
	{
		if(qName=="startmark" || qName=="endmark" || qName=="linestartmark")
		{
			property_ok=true;
			return true;
		}
	}
	
	
	return false;
}


void SelectFile::add_widget(Widget */*w*/)
{
	;
}



mSelectFile::mSelectFile( QWidget * parent):QFileDialog(parent)
{
	setFileMode(QFileDialog::AnyFile);
	connect(this,SIGNAL(finished (int)), this, SLOT(activated_cb(int)));
}

void mSelectFile::activated_cb ( int result )
{
	if(result!=QDialog::Accepted) return;
	QStringList list=selectedFiles ();
	if(list.isEmpty()) return;
	QString text=list.at(0);
	if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
	{
		text.replace("<","&lt;");
		text.replace(">","&gt;");
		w->output->write("<selectfile id=\""+w->name+"\" signal=\"activated\">\n<file>"+toXML(text)+"</file>\n</selectfile>\n");
	}
	else
	{
		w->output->write(startmark);
		w->output->write(linestartmark+text);
		w->output->write(endmark);
	}
}




