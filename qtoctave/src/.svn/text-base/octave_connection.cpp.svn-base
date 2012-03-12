/* Copyright (C) 2006, 2007, 2008 P.L. Lucas
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

#include "octave_connection.h"
#include "config.h"
#include <QThread>
#include <QTextCursor>
#include <QMessageBox>
#include <QFile>
#include <QDir>

void Sleep::micro_sleep(unsigned long u_seconds)
{
	usleep(u_seconds);
}

OctaveConnection::OctaveConnection(QObject * parent ):QProcess(parent)
{
    octave_path = "";

    filestr = "";

    debugging = false;

	init_regular_expresions();
	//text=NULL;
	error_buffer.open(QBuffer::ReadOnly);

	QObject::connect(this, SIGNAL(readyReadStandardError()), this, SLOT(octaveErrorOutputSlot()));

	QObject::connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(octaveOutputSlot()));

	QObject::connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(octaveFinished(int, QProcess::ExitStatus)) );
}

void OctaveConnection::init_regular_expresions()
{
	//Used in octaveOutputSlot
	regexp_octave_prompt.setPattern("octave:[0-9]+>");

	regexp_octave_prompt2.setPattern("octave:[0-9]+\\+>");

    // Extracts the current file name, line no and column number
    // during debugging
	regexp_actual_debug_line_column.setPattern("^([a-zA-Z_]\\w*:)+ line ([0-9]+), column ([0-9]+)$");

    // Used to help determine when octave is in debug mode
    hasDebugPromptRegExp.setPattern("debug>");
}

void OctaveConnection::setOctavePath(const QString path)
{
	octave_path=path;
}

QString OctaveConnection::getOctavePath()
{
	return octave_path;
}


void OctaveConnection::startOctave(bool quiet)
{
	QString extra_args = get_config("octave_arguments");

    instructions_left_no = 0;

	debugging = false;

    bool ok = false;

    QString version = "Not found";

	{
		QProcess in(this);
		printf("[OctaveConnection::startOctave] Octave path: %s\n", octave_path.toLocal8Bit().data() );
		in.start("\""+octave_path+"\" --version", QIODevice::ReadOnly);
		if( in.waitForStarted() && in.waitForFinished() )
		{
			QByteArray buf=in.readAll();

			QRegExp rx("GNU Octave, version ([0-9]*)\\.([0-9]*)\\.([0-9]*)");

			int pos = rx.indexIn(buf);

            if(pos >= 0)
			{
                version = rx.cap(1)+"."+rx.cap(2)+"."+rx.cap(3);

				int ver0,ver1,ver2;

                ver0 = rx.cap(1).toInt();

                ver1 = rx.cap(2).toInt();

                ver2 = rx.cap(3).toInt();

				printf("[OctaveConnection::startOctave] Octave version: %s (%d.%d.%d)\n", version.toLocal8Bit().data(), ver0, ver1, ver2 );

                if( (ver0 == 3 && ver1 >= 2) || (ver0 > 3) )
                    ok = true;
			}
			in.close();
		}
	}

	if(!ok)
	{
		printf("Exit code %d\n",ok);
		QMessageBox::critical (NULL, tr("Warning"),
                               tr("QtOctave requires Octave version 3.2.0 or better.\n"
                                  "You can select your Octave executable in\n"
                                  "Config/General Configuration/Octave/Octave Path\n"
                                  "Your version is:\n")+version
                               );
		return;
	}

	if(quiet)
	{
		start("\""+octave_path+"\" --eval \"PS1(\'\');PS2(\'\');\" --persist --no-history -q -i "+extra_args);
	}
	else
	{
		QString command("\""+octave_path+"\"  --eval \"PS1(\'octave:\\#>\');PS2(\'octave:\\#+>\');addpath('"+QDir(QString(CONFIG_PATH)).absolutePath()+QDir::separator()+"scripts_octave"+QDir::separator()+"')\"  --persist --no-history -i "+extra_args);
		printf("[OctaveConnection::startOctave] Starting octave: %s\n",command.toLocal8Bit().data() );
		start(command);
	}

	//printf("Waiting octave\n");

	if(!waitForStarted())
	{
		QMessageBox::critical (NULL, tr("Warning"), tr("QtOctave in:\n")+octave_path+tr("\nCouldn't be opened."));
	}

	printf("[OctaveConnection::startOctave] Octave running\n");

    if( get_config("easy_plot_active") == "true" )
	{
        QString path = get_config("easy_plot_path");

		if( path.isEmpty() )
			command_enter("gnuplot_binary(\"easy_plot\")");
		else
			command_enter("gnuplot_binary(\""+path+"\")");
	}

	//Load scripts
	loadScripts();
}


void OctaveConnection::loadScripts()
{
	QDir dir(QString(CONFIG_PATH) + "/scripts/");

	QStringList filters;

	filters << "*.m";

	QFileInfoList list = dir.entryInfoList(filters);

	QString _command;

    _command += "__qtoctave_ps2=PS2();PS2('');__qtoctave_ps1=PS1();PS1('');\n";

    for (int i = 0; i < list.size(); ++i)
    {
		QFileInfo fileInfo = list.at(i);

		QFile file(fileInfo.absoluteFilePath());

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

        while (!file.atEnd())
        {
			QByteArray line = file.readLine();

            _command += line;
		}
	}

    _command += "PS2(__qtoctave_ps2); clear __qtoctave_ps2;PS1(__qtoctave_ps1); clear __qtoctave_ps1;\n";
	
	write( _command.toLocal8Bit() );
}


void OctaveConnection::command_enter (const QString &command, bool show)
{
	QString _command(command+"\n");

	if(show)
	{
		//Count number of instructions
        if( !debugging )
		{
            if( instructions_left_no < 0 )
                instructions_left_no = _command.count('\n');
            else
                instructions_left_no += _command.count('\n');
		}

		//printf("instructions_left_no=%d añadida\n",instructions_left_no);

		emit command_ready(" "+_command);
	}
	else
		emit ide_command_ready(" "+_command);

	write(_command.toLocal8Bit() );
}


void OctaveConnection::octaveOutputSlot()
{
    QString buffer = QString::fromLocal8Bit( readAllStandardOutput().data() );
	//QRegExp regexp_octave_prompt("octave:[0-9]+>");
	//QRegExp regexp_octave_prompt2("octave:[0-9]+\\+>");
	//QRegExp regexp_actual_debug_line_column("^([a-zA-Z_]\\w*:)+ line ([0-9]+), column ([0-9]+)$");

	Sleep::micro_sleep(200);

	emit output_ready(buffer);

	// Partir en líneas
	QStringList lines = buffer.split("\n", QString::SkipEmptyParts);

	if(lines.empty())
        lines.push_back("");

	// Al primer elemento se le debe concatenar el buffer de línea
	lines.first() = line_buffer + lines.first();

	line_buffer.clear();

	// En caso de que line_buffer sea debug> u octave:X>, no guardarlo
	if(line_buffer == "debug> ")
        lines.push_back(line_buffer);

    // Procesar las líneas (process each output from octave)
    for(QStringList::iterator strIterator = lines.begin(); strIterator != lines.end(); strIterator++)
	{

        // Debug
        if ( hasDebugPromptRegExp.indexIn(*strIterator) != -1)
        {
            // set debugging to true
            debugging = true;
        }

        // remove the unwanted debug prompt if possible
        if(strIterator->startsWith("debug> "))
        {
            strIterator->remove(0, 7);

            if ( lineno > 0 && colno > 0)
            {
                // announce that we are in debug mode and the current line number
                emit debug(lineno, colno, filestr);
            }
        }

        //printf("line >%s<\n", strIterator->toLocal8Bit().data());

        // PS1 = octave:X>
        if(regexp_octave_prompt.indexIn(*strIterator) > -1)
        {

            instructions_left_no -= strIterator->count(regexp_octave_prompt);

            //i->remove(0, regexp_octave_prompt.matchedLength());
            strIterator->remove(regexp_octave_prompt);

            //printf("instructions_left_no=%d eliminada\n",instructions_left_no);
            //printf("line eliminada >%s<\n", strIterator->toLocal8Bit().data());

            if(debugging && !(hasDebugPromptRegExp.indexIn(*strIterator) != -1))
            {
                debugging = false;
                filestr = "";
                emit endDebug();
            }
            else if(instructions_left_no==0)
            {
                //printf("Comando terminado\n");
                instructions_left_no=0;
                emit command_finished();
            }
        }

        // PS2 = octave:X+>
        if(regexp_octave_prompt2.indexIn(*strIterator) > -1)
        {

            instructions_left_no -= strIterator->count(regexp_octave_prompt2);

            //i->remove(0, regexp_octave_prompt.matchedLength());
            strIterator->remove(regexp_octave_prompt2);

            //printf("instructions_left_no=%d eliminada PS2\n",instructions_left_no);
            //printf("line eliminada PS2 >%s<\n", strIterator->toLocal8Bit().data());
        }

        // Línea y columna cuando va a parar
        if(regexp_actual_debug_line_column.exactMatch(*strIterator))
        {
            int start;

            //QStringList list = regexp_actual_debug_line_column.capturedTexts();
            //foreach(QString item, list)
            //{
            //	printf("Captura %s %d\n", item.toLocal8Bit().data(), regexp_actual_debug_line_column.numCaptures()-2 );
            //}

            // Extract the function name
//            start = regexp_actual_debug_line_column.numCaptures()-2;

//            if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
//            {
//                fcnstr = regexp_actual_debug_line_column.cap(start);

//                fcnstr = fcnstr.left(fcnstr.indexOf(":"));
//            }

            // Extraer la línea
            start = regexp_actual_debug_line_column.numCaptures() - 1;

            lineno = -1;

            if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
            {
                lineno = regexp_actual_debug_line_column.cap(start).toInt();
            }

            // Extraer la columna
            start = regexp_actual_debug_line_column.numCaptures();

            colno = -1;

            if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
            {
                colno = regexp_actual_debug_line_column.cap(start).toInt();
            }


        }
		// Code of clc (clean screen)
        if(*strIterator == QString("\033[H\033[2J"))
		{
			emit clearScreen();
		}
	}

	// Meter en el buffer de línea el último elemento si la cadena
	// no terminaba en \n
	if(!buffer.endsWith("\n"))
        line_buffer = lines.last();
}

void OctaveConnection::octaveErrorOutputSlot()
{	
    QString buffer = error_buffer.data();

	error_buffer.close();

	error_buffer.setData( buffer.toUtf8()+readAllStandardError());

	error_buffer.open(QBuffer::ReadOnly);

	Sleep::micro_sleep(200);

	QString line, error("");

	do
	{
        line = error_buffer.readLine();

		if(line.endsWith("\n") )
		{
			if(line.startsWith("~~"))
			{
				// Code of clc (clean screen)
                if(line == "~~ClearScreen\n")
                    emit clearScreen();

				else emit line_ready(line);
			}
            else
            {
                error += line;
            }
			
			if( regexp_actual_debug_line_column.exactMatch(line.trimmed()) )
            {
                int start;

				//printf("[OctaveConnection::octaveErrorOutputSlot]\n");
				//QStringList list = regexp_actual_debug_line_column.capturedTexts();
				//foreach(QString item, list)
				//{
				//	printf("Captura %s %d\n", item.toLocal8Bit().data(), regexp_actual_debug_line_column.numCaptures()-2 );
				//}

                // Extract the function name
//                start = regexp_actual_debug_line_column.numCaptures()-2;

//                if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
//                {
//                    fcnstr = regexp_actual_debug_line_column.cap(start);

//                    fcnstr = fcnstr.left(fcnstr.indexOf(":"));
//                }

                // Extraer la línea (extract the line number)
                start = regexp_actual_debug_line_column.numCaptures()-1;

                if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
                {
                    lineno = regexp_actual_debug_line_column.cap(start).toInt();
                }

                // Extraer la columna (extract the column number)
                start = regexp_actual_debug_line_column.numCaptures();

                if( !regexp_actual_debug_line_column.cap(start).isEmpty() )
                {
                    colno = regexp_actual_debug_line_column.cap(start).toInt();
                }

                emit debug(lineno, colno, filestr);
            }

            if(line.startsWith("keyboard: stopped in "))
            {

                //int temp = line.indexOf(" at line") - 21;

                filestr = line.mid(21, line.indexOf(" at line") - 21);

                emit debug(lineno, colno, filestr);
            }

			continue;
		}
		else
		{
			line=line+error_buffer.readAll();
			error_buffer.close();
			error_buffer.setData(line.toUtf8());
			error_buffer.open(QBuffer::ReadOnly);
			break;
		}
		emit line_ready(line);
	}
	while(error_buffer.bytesAvailable());
	emit error_ready(error);
	if(!error_buffer.bytesAvailable())
	{
		error_buffer.close();
		error_buffer.setData(QString("").toUtf8());
		error_buffer.open(QBuffer::ReadOnly);
	}

	//Se procesa el buffer para buscar las líneas diponibles
	//QStringList lines=buffer.split("\n");
	//QStringList::const_iterator i;
	//for (i = lines.constBegin(); i != lines.constEnd();)
	//{
	//	QString line=*i;
	//	++i;
	//	if(i==lines.constEnd() && !buffer.endsWith("\n"))
	//		emit error_ready(line);
	//	else
	//		emit error_ready(line+"\n");
	//	emit line_ready(line);
	//}
}

void OctaveConnection::octaveFinished(int /*exitCode*/, QProcess::ExitStatus exitStatus)
{
	if(exitStatus == QProcess::CrashExit)
	{
		QMessageBox::critical(NULL,tr("Octave crashed"), tr("Octave has crashed. Restarting..."));
	}
	else
	{
		QMessageBox::critical(NULL,tr("Octave finished"), tr("Octave has finished. Restarting..."));
	}
	//Relanzar
	startOctave();
}

bool OctaveConnection::isDebugging()
{
    return debugging;
}

void OctaveConnection::debugStep()
{
    if (debugging)
        command_enter("dbstep", false);
}

void OctaveConnection::debugStepIn()
{
    if (debugging)
        command_enter("dbstep in", false);
}

void OctaveConnection::debugStepOut()
{
    if (debugging)
        command_enter("dbstep out", false);
}

void OctaveConnection::debugContinue()
{
    if (debugging)
        command_enter("dbcont", false);
}

void OctaveConnection::debugQuit()
{
    if (debugging)
        command_enter("dbquit", false);
}

void OctaveConnection::addBreakPoint( QString filename, int lineno )
{
    command_enter(QString("dbstop('")
                  + filename
                  + "','" + QString::number(lineno) + "')", false);
}

void OctaveConnection::removeBreakPoint( QString filename, int lineno )
{
    command_enter(QString("dbclear('")
                  + filename
                  + "','" + QString::number(lineno) + "')", false);
}
