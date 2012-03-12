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

#include "parser.h"

Parser::Parser():QObject()
{
	xml_parser.setContentHandler(&xml_handler);
	xml_parser.setErrorHandler(&xml_handler);
	new_data=true;
}

void Parser::setReader(Reader *reader)
{
	connect(reader,SIGNAL(data_ready(QString)), this, SLOT(parser_input(QString)));
}

void Parser::setOutput(Output *output)
{
	this->output=output;
	xml_handler.setOutput(output);
}

void Parser::parser_input(QString input)
{
	QString buffer=xml_input.data();
	//fprintf(stderr,"Buffer: %s\n",(buffer/*+input*/).toLocal8Bit().data());
	xml_input.setData(/*buffer+*/input);
	bool ok;
	if(new_data)
	{
		ok = xml_parser.parse(&xml_input, true);
		new_data=false;
	}
	else
		ok = xml_parser.parseContinue();
	
	if(!ok)
	{
		fprintf(stderr,"Error en la entrada.\n");
	}
}
