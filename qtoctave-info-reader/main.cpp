/* Copyright (C) 2009 P.L. Lucas
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

#include <QApplication>
#include "infomainwindow.h"



/*! \mainpage QtInfo
 *
 * \section intro_sec Introduction
 *
 * <p>This tool is a simple reaer for info files. UNIX tools has their documentation in
 * info format. This files can be read with <a href='http://www.gnu.org/software/texinfo/'>info tool</a>.
 * Help of Octave is in info format.</p>
 * <p>This tool helps you to read info files easyly.</p>
 *
 */




int main(int argn, char *argv[])
{
	QApplication app(argn,argv);
	
	if(argn<2)
	{
		printf("\nQtInfo. This tool is a info file reader.\n"
			"%s info_file_path\n\n"
			,argv[0]);
		return 0;
	}
	
	InfoMainWindow mainWindow;
	
	mainWindow.setInfoPath(argv[1]);
	
	mainWindow.show();
	
	app.exec();
}
