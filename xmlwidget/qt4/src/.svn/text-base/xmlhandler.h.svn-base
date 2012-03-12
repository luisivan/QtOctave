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


#ifndef __XMLHANDLER_H__
#define __XMLHANDLER_H__
#include <QtXml>
#include <QXmlDefaultHandler>
#include <QStack>
#include <QHash>

#include "output.h"

#include "widgets/widget.h"
#include "widgets/widgets.h"

class XmlHandler : public QXmlDefaultHandler
{
	public:
	XmlHandler();
	
	bool startElement(const QString &namespaceURI, const QString &localName,
				const QString &qName, const QXmlAttributes &attributes);
	bool endElement(const QString &namespaceURI, const QString &localName,
			const QString &qName);
	bool characters(const QString &str);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;
	
	void setOutput(Output *output);
	
	private:
	QString currentText;
	QString errorStr;
	QStack<Widget*> last_element;
	QHash<QString,Widget*> widgets;
	
	Output *output;
	
	
};

#endif
