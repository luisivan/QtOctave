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


#ifndef __PARSER_H__
#define __PARSER_H__
#include "xmlhandler.h"
#include <QObject>
#include "reader.h"
#include "output.h"
#include <QXmlSimpleReader>

class Parser:QObject
{
	Q_OBJECT
	public:
	Parser();
	void setReader(Reader *reader);
	void setOutput(Output *output);
	
	public slots:
	void parser_input(QString input);
	
	private:
	QXmlSimpleReader xml_parser;
	XmlHandler xml_handler;
	QXmlInputSource xml_input;
	bool new_data;
	
	Output *output;
};

#endif
