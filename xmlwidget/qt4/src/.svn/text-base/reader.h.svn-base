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


#ifndef __READER_H__
#define __READER_H__
#include <QThread>

class Reader:public QThread
{
	Q_OBJECT
	
	public:
	Reader();
	
	void run();
	
	signals:
	void data_ready(QString data);
	
	protected:
	/**Read available input.*/
	virtual QString read_available_data()=0;
	virtual bool end_of_input()=0;
};

#endif


