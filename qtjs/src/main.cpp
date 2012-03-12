/* Copyright (C) 2008 P.L. Lucas
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
#include <QPushButton>
#include <QtScript>
#include <QtUiTools>

int main(int argc, char *argv[])
{

    if(argc<3)
    {
    	printf("%s file.ui file.js\n",argv[0]);
    	return 0;
    }

    QApplication app(argc, argv);

    QScriptEngine engine;

	QUiLoader loader;
	QFile file(argv[1]);
     	file.open(QFile::ReadOnly);

     	QWidget *formWidget = loader.load(&file);
     	file.close();

    QScriptValue scriptWidget = engine.newQObject(formWidget);
    engine.globalObject().setProperty("widget", scriptWidget);
    
    QFile ifile(argv[2]);
    ifile.open(QFile::ReadOnly);
    QByteArray code=ifile.readAll();
    ifile.close();

	if( !engine.canEvaluate(code) )
	{
		printf("Error\n");
		return 1;
	}

    engine.evaluate("widget.show()");
    QScriptValue result = engine.evaluate(code);
    
    if (engine.hasUncaughtException())
    {
         int line = engine.uncaughtExceptionLineNumber();
         qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }

    return app.exec();
}