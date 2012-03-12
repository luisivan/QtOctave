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

#include "xmlhandler.h"
#include <QApplication>

XmlHandler::XmlHandler()
{
}

bool XmlHandler::startElement( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
	//Is it a widget or element of widget
	
	Widget *w=NULL;
	Widget *wparent=NULL; //Parent widget
	
	if(qName=="widgetserver") return true;
	else if(qName=="quit")
	{
		QApplication::exit();
		return false;
	}
	
	if( last_element.isEmpty() )
	{
		w=new_widget(qName,atts,widgets,output, NULL);
		
		if(w==NULL)
		{
			errorStr="Widget "+qName+" unknown.";
			return false;
		}
	}
	else //last_element is not empty
	{
		wparent=last_element.top();
		
		bool ok=wparent->xml_start(qName,atts);
		
		//printf("Se ha procesado el padre\n");
		
		if(!ok && wparent->container)
		{
			w=new_widget(qName,atts,widgets,output, wparent->widget);
			if(w==NULL)
			{
				errorStr="Widget "+qName+" unknown.";
				return false;
			}
		}
		else if( !ok && !(wparent->container) )
		{
			errorStr="Property "+qName+" unknown.";
			return false;
		}
	}
	
	if(w!=NULL) w->xml_start(qName,atts);
	
	if(w!=NULL) last_element.push(w);
	
	if( w!=NULL )
	{
		QString name=w->name;
		if( !(widgets.contains(name)) )
		{
			//printf("Insertando %s en la base QHash\n", name.toLocal8Bit().data());
			widgets.insert(name,(Widget*)w);
		}
	}
	
	currentText.clear();
	
	//printf("Se ha limpiado el texto\n");
	
	return true;
}



bool XmlHandler::endElement(const QString & /* namespaceURI */,
                                 const QString & /* localName */,
                                 const QString &qName)
{
	//fprintf(stderr,"Texto entrada %s: %s\n", qName.toLocal8Bit().data(), currentText.toLocal8Bit().data());
	
	if(qName=="widgetserver") return true;
	
	Widget *w=last_element.top();
	
	bool ok, property;
	
	ok=w->xml_end(qName, currentText, property);
	
	
	if ( ok && !property )
	{
		w=last_element.pop();
		//w->widget->show();
		if(!last_element.isEmpty())
		{
			Widget *wparent=last_element.top();
			wparent->add_widget(w);
		}
	}
	else if( !ok )
	{
		errorStr="Error in "+qName+" end.";
		return false;
	}
	
	return true;
}

bool XmlHandler::characters(const QString &str)
{
	currentText += str;
	return true;
}

QString XmlHandler::errorString() const
{
	return errorStr;
}

#include <QMessageBox>
bool XmlHandler::fatalError(const QXmlParseException &exception)
{
	QMessageBox::information(NULL, QObject::tr("SAX Error"),
				QObject::tr("XML Parse error at line %1, column %2:\n"
					"%3")
				.arg(exception.lineNumber())
				.arg(exception.columnNumber())
				.arg(exception.message()));
	return false;
}


void XmlHandler::setOutput(Output *output)
{
	this->output=output;
}








