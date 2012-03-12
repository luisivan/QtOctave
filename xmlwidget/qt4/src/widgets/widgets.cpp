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

#include "widgets.h"

Widget *new_widget(const QString & qName, const QXmlAttributes & atts, const QHash<QString,Widget*> &widgets, Output *output, QWidget *parent)
{
	Widget *w=NULL;
	QString name=atts.value("id");
	if( !name.isEmpty() && widgets.contains(name) )
	{
		//printf("Extrayendo %s de la base QHash\n", name.toLocal8Bit().data());
		w=widgets[name];
		return w;
	}
	
	if( (w=new_window(qName,atts, parent))!=NULL );
	else if( (w=new_button(qName,atts, parent))!=NULL );
	else if( (w=new_hbox(qName,atts, parent))!=NULL );
	else if( (w=new_vbox(qName,atts, parent))!=NULL );
	else if( (w=new_label(qName,atts, parent))!=NULL );
	else if( (w=new_lineedit(qName,atts, parent))!=NULL );
	else if( (w=new_tree(qName,atts, parent))!=NULL );
	else if( (w=new_menu(qName,atts, parent))!=NULL );
	else if( (w=new_vseparator(qName,atts, parent))!=NULL );
	else if( (w=new_hseparator(qName,atts, parent))!=NULL );
	else if( (w=new_textedit(qName,atts, parent))!=NULL );
	else if( (w=new_combobox(qName,atts, parent))!=NULL );
	else if( (w=new_selectfile(qName,atts, parent))!=NULL );
	else if( (w=new_scroll(qName,atts, parent))!=NULL );
	
	if(w!=NULL)
	{
		w->output=output;
		//w->widget->show();
	}
	
	return w;
}

