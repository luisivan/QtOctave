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

#include "terminal.h"
#include "config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QLineEdit>
#include <QIcon>
#include <QTextStream>
#include <QMap>
#include <QLabel>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QRegExp>
#include "projects/projects.h"
#include "pkg_bind.h"


Terminal::Terminal(QWidget * parent):BaseWidget(parent)
{
    widget_type = TERMINAL;
	
    octave_connection = NULL;

    diary_ok = false;
	
	init_regular_expresions();
	
	menuBar()->hide();
	
	//Se crea la ventana del terminal
	setWindowTitle(tr("Octave Terminal"));
	setWindowIcon( QIcon(QString( ICON_PATH )+"/konsole.png"));
	
	if(get_config("lines_in_terminal").isEmpty())
	{
		QMap<QString,QString> c;
		c["lines_in_terminal"]="1000";
		set_config(c);
	}
	if(get_config("cols_in_terminal").isEmpty())
	{
		QMap<QString,QString> c;
		c["cols_in_terminal"]="80";
		set_config(c);
	}
	
    lines_in_terminal = get_config("lines_in_terminal").toInt();

    cols_in_terminal = get_config("cols_in_terminal").toInt();

    text = new OctOutputTextEdit(this);

	text->setLineWrapMode (QTextEdit::NoWrap);

	//text->setLineWrapColumnOrWidth ( cols_in_terminal );
	text->setReadOnly (true);
	
	if(get_config("show_ide_commands").isEmpty())
	{
		show_ide_commands_ok=false;
		QMap<QString,QString> c;
		c["show_ide_commands"]="false";
		set_config(c);
	}
    else if(get_config("show_ide_commands")=="true")
        show_ide_commands_ok = true;
    else
        show_ide_commands_ok=false;
	
	
	if(get_config("terminal_font").isEmpty())
	{
		QMap<QString,QString> c;
		c["terminal_font"]="Liberation Mono";
		set_config(c);
	}
	
	//{
	//	QFont font;
	//	font.fromString(get_config("terminal_font"));
	//	text->setFont( font );
	//}
	if(get_config("terminal_foreground_color").isEmpty())
	{
		QMap<QString,QString> c;
		c["terminal_foreground_color"]="Black";
		set_config(c);
	}
	
	if(get_config("terminal_background_color").isEmpty())
	{
		QMap<QString,QString> c;
		c["terminal_background_color"]="White";
		set_config(c);
	}
	
	if(get_config("terminal_error_color").isEmpty())
	{
		QMap<QString,QString> c;
		c["terminal_error_color"]="Red";
		set_config(c);
	}
	
	if(get_config("ide_command_color").isEmpty())
	{
		QMap<QString,QString> c;
		c["ide_command_color"]="lightGray";
		set_config(c);
	}
	
	//text->setTextColor(get_config("terminal_foreground_color"));
	{
        QPalette p = text->viewport ()->palette();
		p.setColor(QPalette::Base, QColor(get_config("terminal_background_color")) );
		text->viewport()->setPalette(p);
	}
	
	QFont font;
	font.fromString(get_config("terminal_font"));
	normal_format.setFont(font);
	normal_format.setFontWeight(QFont::Normal);
	
    ide_command_format = normal_format;
	ide_command_format.setForeground(QColor(get_config("ide_command_color")) );
	ide_command_format.setFontPointSize(font.pointSize()-3);
	normal_format.setForeground(QColor(get_config("terminal_foreground_color")) );
    command_format = normal_format;
	command_format.setFontWeight(QFont::Bold);
    error_format = normal_format;
	error_format.setForeground(QColor(get_config("terminal_error_color")) );
	
    // Initialise the text in the output window
	text->setHtml("Starting Octave...<br>");
	text->show();
	
    /* The Autocomplete class inheirits QLineEdit, a one line text editor.
       Here we will use one to get input for octave from the user

       Se captura el "intro" para pasar comandos al octave
     */
    commandLineEdit = new Autocomplete(this);
    commandLineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    commandLineEdit->show();
    //commandLineEdit->setFocus();
	
    /* We set up the commandLineEdit to handle some signals such as the
       return key being pressed and text and selection changes */
    connect(commandLineEdit, SIGNAL(returnPressed()), this, SLOT(return_pressed()));
    connect(commandLineEdit, SIGNAL( textChanged ( const QString &) ) , this, SLOT( textChanged ( const QString &) ) );
    connect(commandLineEdit, SIGNAL(selectionChanged ()), this, SLOT(clear_command_line_first_time()));

    // Finally set the tooltip text and display a welcome message to the user
    commandLineEdit->setToolTip(tr("Insert your commands here. Use arrows and tab key to navigatate commands."));
    commandLineEdit->setText(tr("Insert your commands here..."));
    commandLineEdit->selectAll();
	
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *line_layout = new QHBoxLayout;
	layout->addWidget(text);
	layout->addLayout(line_layout);
	
	line_layout->addWidget(new QLabel("<b>Command line&gt;&gt;</b>",this));
    line_layout->addWidget(commandLineEdit);
	
	centralWidget()->setLayout(layout);
	
	setAcceptDrops(true);
}

void Terminal::init_regular_expresions()
{
	//Used in remove_lines
	regexp_octave_prompt.setPattern("(((octave:[0-9]+>)|(octave:[0-9]+\\+>))|>>>)+"); //PS1 or PS2
	
	//Used in line_ready
	re.setPattern("~~Diary: (.*) '(.*)'\n");
}

void Terminal::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain"))
		event->acceptProposedAction();
}


void Terminal::dropEvent ( QDropEvent * event )
{
	QString path=event->mimeData()->text();
	
	QFileInfo fileInfo(path);
	
	QString cmd;
	
	// Change dir
	cmd = QString("cd \"") + fileInfo.dir().absolutePath() +"\"";
	octave_connection->command_enter(cmd);
	
	// Execute file
	cmd = fileInfo.baseName();
	octave_connection->command_enter(cmd);

	event->acceptProposedAction();
}

void Terminal::keyPressEvent(QKeyEvent * event)
{

    if ( event->key() == Qt::Key_F10 )
    {
        octave_connection->debugStep();
    }
    else if ( event->key() == Qt::Key_F11 )
    {
        octave_connection->debugStepIn();
    }
    else if ( event->key() == Qt::Key_F11 && event->modifiers() == Qt::ShiftModifier)
    {
        octave_connection->debugStepOut();
    }
    else if ( event->key() == Qt::Key_F5 )
    {
        octave_connection->debugContinue();
    }
    else
    {
        // switch focus to the command line editor
        commandLineEdit->setFocus(Qt::OtherFocusReason);

        // and send it the key press
        QApplication::sendEvent(commandLineEdit, event);
    }

}

void Terminal::return_pressed()
{
    //QLineEdit *line_edit=commandLineEdit/*->lineEdit()*/;
    //QString command(line_edit->text());
    //command_enter(command);
    command_enter(commandLineEdit->text());
}

//QTextEdit *Terminal::getTextEdit()
//{
//    return text;
//}

OctOutputTextEdit *Terminal::getTextEdit()
{
    return text;
}

Autocomplete *Terminal::getAutocomplete()
{
    return commandLineEdit;
}

void Terminal::setOctaveConnection(OctaveConnection *octave_connection)
{
	this->octave_connection=octave_connection;
	
	connect(octave_connection, SIGNAL(error_ready(QString)), this, SLOT(write_error(QString)));
	connect(octave_connection, SIGNAL(output_ready(QString)), this, SLOT(write_output(QString)));
	connect(octave_connection, SIGNAL(command_ready(QString)), this, SLOT(write_command(QString)));
	connect(octave_connection, SIGNAL(ide_command_ready(QString)), this, SLOT(write_ide_command(QString)));
	connect(octave_connection, SIGNAL(line_ready(QString)), this, SLOT(line_ready(QString)));
    commandLineEdit->set_octave_connection(octave_connection);
	//setProject(); //This line loads variables
}

OctaveConnection *Terminal::getOctaveConnection()
{
	return octave_connection;
}

void Terminal::command_enter (const QString &command)
{
	octave_connection->command_enter(command);
    commandLineEdit->setText("");
}

#include <QTextBlock>

void Terminal::remove_lines(QTextCursor &cursor)
{
	//Se cortan las líneas que sean demasiado largas
	cursor.movePosition(QTextCursor::Start);
	int lines=0;
	while(true)
	{
		QTextBlock block=cursor.block();
		if(block.length()>cols_in_terminal)
		{
			cursor.movePosition(QTextCursor::StartOfBlock);
			cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,cols_in_terminal-1);
			cursor.insertText("\n");
		}
		else
		{
			cursor.movePosition(QTextCursor::EndOfBlock);
			cursor.movePosition(QTextCursor::Right);
		}
		cursor.movePosition(QTextCursor::EndOfBlock);
		lines++;
		if(cursor.atEnd())
		{
			QTextBlock block=cursor.block();
			if(block.length()<cols_in_terminal) break;
		}
	}
	
	//Se borran las líneas iniciales si hay más de lines_in_terminal líneas
	//printf("lines=%d %d\n",lines,lines_in_terminal);
	lines=text->toPlainText().count('\n');
	while(lines>lines_in_terminal)
	{
		cursor.movePosition(QTextCursor::Start);
		cursor.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
		//printf("line %d: %s\n",lines, cursor.selectedText().toLocal8Bit().data());
		if(cursor.hasSelection()) cursor.removeSelectedText();
		else cursor.deleteChar();
		cursor.deleteChar();
		lines--;
	}
	//printf("lines=%d %d\n",lines,lines_in_terminal);
	
	//Merge all octave:X> in only one
	//QRegExp regexp_octave_prompt("((octave:[0-9]+>)|(octave:[0-9]+\\+>))"); //PS1 or PS2
	
	cursor.movePosition(QTextCursor::Start);
	if( !text->isVisible() ) return;
	cursor = text->document()->find(regexp_octave_prompt,cursor);
	while( cursor.position()>=0 )
	{
		QTextCharFormat format=cursor.charFormat();
		if(format.fontWeight()!=QFont::Bold)
		{
			format.setFontWeight(QFont::Bold);
			//cursor.setCharFormat(format);
			cursor.insertText(">>>", format);
		}
		cursor = text->document()->find(regexp_octave_prompt,cursor);
	}
	
	cursor=text->textCursor();
	
	return;
	
	/*
	//cursor.movePosition(QTextCursor::Start);
	//while(!cursor.atEnd())
	//{
	//	int prompt_no=0;
	//	
	//	cursor.movePosition(QTextCursor::Right);
	//	cursor.movePosition(QTextCursor::StartOfBlock);
	//	cursor.movePosition( QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
	//	
	//	QString line=cursor.selectedText();
	//	QString last_prompt;
	//	int pos = 0;

	//	while ((pos = regexp_octave_prompt.indexIn(line, pos)) != -1)
	//	{
	//		last_prompt=regexp_octave_prompt.cap(1);
	//		pos += regexp_octave_prompt.matchedLength();
	//		prompt_no++;
	//	}
		
	//	if(prompt_no>=1)
	//	{
	//		line.remove(regexp_octave_prompt);
	//		QTextCharFormat format=text->currentCharFormat();
	//		format.setFontWeight(QFont::Bold);
	//		cursor.insertText(last_prompt,format);
	//		format.setFontWeight(QFont::Normal);
	//		cursor.insertText(line,format);
	//	}
		
	//	cursor.movePosition(QTextCursor::EndOfBlock);
	//}
	*/
}

void Terminal::write_output(QString output)
{
	QTextCursor cursor=text->textCursor();
	cursor.beginEditBlock();
	cursor.movePosition(QTextCursor::End);
	
	//QTextCharFormat format=cursor.charFormat();
	//format.setFontWeight(QFont::Normal);
	//format.setForeground(QColor(get_config("terminal_foreground_color")) );
	//cursor.setCharFormat(format);
	cursor.insertText(output, normal_format);
	//text->setTextCursor( cursor );
	//text->setTextColor( get_config("terminal_foreground_color") );
	//text->setFontWeight ( QFont::Normal );
	//text->insertPlainText(output);
	remove_lines(cursor);
	cursor.movePosition(QTextCursor::End);
	cursor.endEditBlock();
	text->setTextCursor( cursor );
	if(diary_ok)
	{
		diary_file.write(output.toLocal8Bit());
	}
}

void Terminal::write_error(QString error)
{
	QTextCursor cursor=text->textCursor();
	cursor.beginEditBlock();
	cursor.movePosition(QTextCursor::End);
	//QTextCharFormat format=cursor.charFormat();
	//format.setFontWeight(QFont::Normal);
	//format.setForeground(QColor(get_config("terminal_error_color")) );
	//cursor.setCharFormat(format);
	cursor.insertText(error, error_format);
	//text->setTextCursor( cursor );
	//text->setTextColor( get_config("terminal_error_color") );
	//text->setFontWeight ( QFont::Normal );
	//text->insertPlainText(error);
	remove_lines(cursor);
	cursor.movePosition(QTextCursor::End);
	cursor.endEditBlock();
	text->setTextCursor( cursor );

#ifdef PKG_ENABLED
	// Symbol not defined. ¿Quizá en un paquete?
	QRegExp undefinedSymbol("error: `(\\w+)' undefined near line \\d+ column \\d+");
	if(undefinedSymbol.indexIn(error) > -1)
	{
	  QString symbol(undefinedSymbol.capturedTexts()[1]);
	  if(PkgBind::getInstance()->checkSymbol(symbol))
	    PkgBind::getInstance()->invokePackageManager(symbol);
	}
#endif
}

void Terminal::write_command(QString command)
{
	QTextCursor cursor=text->textCursor();
	cursor.beginEditBlock();
	cursor.movePosition(QTextCursor::End);
	//QTextCharFormat format=cursor.charFormat();
	//format.setFontWeight(QFont::Bold);
	//format.setForeground(QColor(get_config("terminal_foreground_color")) );
	//cursor.setCharFormat(format);
	cursor.insertText(command, command_format);
	//text->setTextCursor( cursor );
	//text->setTextColor( get_config("terminal_foreground_color") );
	//text->setFontWeight ( QFont::Bold );
	//text->insertPlainText(command);
	//text->setFontWeight ( QFont::Normal );
	remove_lines(cursor);
	cursor.movePosition(QTextCursor::End);
	cursor.endEditBlock();
	text->setTextCursor( cursor );
	if(diary_ok)
	{
		diary_file.write(command.toLocal8Bit());
	}
}

void Terminal::write_ide_command(QString command)
{
	if(!show_ide_commands_ok) return;
	QTextCursor cursor=text->textCursor();
	cursor.beginEditBlock();
	cursor.movePosition(QTextCursor::End);
	//QTextCharFormat format=cursor.charFormat();
	//format.setFontWeight(QFont::Bold);
	//format.setForeground(Qt::lightGray);
	//cursor.setCharFormat(format);
	cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
	cursor.setCharFormat(ide_command_format);
	cursor.movePosition(QTextCursor::End);
	cursor.insertText(command, ide_command_format);
	//text->setTextCursor( cursor );
	//text->setTextColor( get_config("terminal_foreground_color") );
	//text->setFontWeight ( QFont::Bold );
	//text->insertPlainText(command);
	//text->setFontWeight ( QFont::Normal );
	remove_lines(cursor);
	cursor.movePosition(QTextCursor::End);
	cursor.endEditBlock();
	text->setTextCursor( cursor );
}

void Terminal::closeEvent(QCloseEvent *event)
{
	if(session->getTools(TERMINAL).size()<2)
	{
		//Ignorar este evento
		event->ignore();
	}
}

void Terminal::completion_matches_callback()
{
	QString command;
    QTextStream(&command) << "completion_matches(\"" << commandLineEdit/*->lineEdit()*/->text() << "\")";
	
	octave_connection->command_enter(command,false);
}


#include <sys/types.h>
#include <signal.h>

void Terminal::stop_process_callback()
{
	#if defined Q_OS_WIN32
		//system(QString("sh kill -s SIGINT "+QString("%d").arg(octave_connection->pid()) ).toLocal8Bit().data() );
		octave_connection->terminate();
	#else
		kill(octave_connection->pid(),SIGINT);
	#endif
}

void Terminal::clear_callback()
{
	text->clear();
	octave_connection->command_enter("",false);
}

void Terminal::clear_command_line_first_time()
{
    commandLineEdit->setText("");
    disconnect(commandLineEdit, SIGNAL(selectionChanged ()), this, SLOT(clear_command_line_first_time()));
}

CommandList * Terminal::show_commands_callback()
{
	CommandList *command_list=new CommandList(work_space);
    connect(commandLineEdit, SIGNAL(new_command_entered(QStringList)), command_list, SLOT(set_list(QStringList)) );
	command_list->setSession(session);
    command_list->setLineEdit(commandLineEdit);
    command_list->set_list(commandLineEdit->commands());
	if(work_space!=NULL) work_space->addSubWindow(command_list);
	command_list->show();
	return command_list;
}

void Terminal::textChanged( const QString & text)
{
	emit dynamic_help_required(text);
}

void Terminal::setSession(Session *session)
{
	BaseWidget::setSession(session);
	connect(session, SIGNAL(projectChanged(QString)), this, SLOT(setProject(QString)));
}

void Terminal::setProject(QString /*project*/)
{
	if(octave_connection!=NULL)
	{
		//Load variables
		QString file_name=Projects::variablesListPath(session->getProjectName());
		if(file_name.isEmpty()) return;
		QString command("load(\"-text\",\""+file_name+"\"); cd(qtoctave_pwd); clear qtoctave_pwd;");
		octave_connection->command_enter(command, false);
		
        commandLineEdit->setProject(session->getProjectName());
	}
}

Terminal::~Terminal()
{
	//saveVariables();
}

void Terminal::saveVariables()
{
	if(octave_connection!=NULL)
	{
		//Save variables
		QDateTime date=QDateTime::currentDateTime();
		QString file_name=QString::number( date.toTime_t() );
		
		QString home=QDir::home().path()+"/.qtoctave/";
		file_name=home+file_name+".variables";
		
		QString command("qtoctave_pwd=pwd; save(\"-text\",\""+file_name+"\");");
		octave_connection->command_enter(command);
		octave_connection->command_enter("disp(\"\\n\\nAll data has been saved.\");",false);
		
		//Octave has 1 seconds to save data
		Sleep::micro_sleep(1000000);
		
		Projects::saveVariablesListPath(session->getProjectName(), file_name);
	}
}

BaseWidget *Terminal::copyBaseWidget(QWidget * parent )
{
	Terminal *bw=new Terminal(parent);
	bw->setSession(session);
	bw->setProject();
	bw->setOctaveConnection(octave_connection);
	bw->text->setHtml(text->toHtml());
	QTextCursor cursor=bw->text->textCursor();
	cursor.movePosition(QTextCursor::End);
	bw->text->setTextCursor( cursor );
	
	return bw;
}

void Terminal::line_ready(QString line)
{
	//QRegExp re("~~Diary: (.*) '(.*)'\n");
	
	if( re.exactMatch(line) )
	{
		QString command=re.cap(1);
		QString file_name=re.cap(2);
		
		if( command=="change" )
			diary_ok=!diary_ok;
		else if( command=="on" )
			diary_ok=true;
		else if( command=="off" )
			diary_ok=false;
		else if( command=="file" )
		{
			diary_ok=true;
			if(diary_file.isOpen()) diary_file.close();
			diary_file.setFileName(file_name);
			if (!diary_file.open(QIODevice::Append | QIODevice::Text)) return;
		}
		
		if(diary_ok)
		{
			if(!diary_file.isOpen())
			{
				if(diary_file.fileName().isEmpty())
					diary_file.setFileName(file_name+"/diary");
				diary_file.open(QIODevice::Append | QIODevice::Text);
			}
		}
		else
			diary_file.close();
	}
}



