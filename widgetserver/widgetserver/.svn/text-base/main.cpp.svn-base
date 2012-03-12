/* Copyright (C) 2006 P.L. Lucas
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
#include "process.h"
#include "input.h"
#include <string.h>

int main(int argn, char *argv[])
{
	QApplication a(argn,argv);
	Process process;
	
	{
		int i=1;
		bool std_in_out_ok=true;
		while(i<argn)
		{
			if(strcmp(argv[i],"--help")==0)
			{
				printf("%s [--help] [-debug] [-exec path]\n",argv[0]);
				printf(
						"Starts WidgetServer\n"
						"--help Shows this help\n"
						"-debug shows debug output\n"
						"-exec path Executes path in WidgetServer. All input and output are redirected by WidgetServer\n"
				      );
				return 0;
			}
			else if(strcmp(argv[i],"-exec")==0)
			{
				i++;
				ExternProcess *p;
				p=new ExternProcess(argv[i],&process);
				std_in_out_ok=false;
			}
			else if(strcmp(argv[i],"-debug")==0)
			{
				process.setDebug(true);
			}
			i++;
		}
		if(std_in_out_ok)
		{
			Input *in=new Input(stdin,stdout,&process);
			in->start();
		}
	}
	
	return a.exec();
}
