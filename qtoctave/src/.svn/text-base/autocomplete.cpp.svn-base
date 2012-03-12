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

#include "autocomplete.h"
#include "terminal.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <iostream>
#include <QDateTime>

using namespace std;

Autocomplete::Autocomplete(QWidget *parent): QLineEdit(parent), search_string(""),
		tab_flag(false)
{
	current_match = first_match = word_list.end();
	
	actual_command_entered=-1;
	
	setCompleter(&completer);
	completion_model=new QStringListModel(&completer);
	completer.setModel(completion_model);

    // get the terminal Widget which created the Autocomplete
    callingTerminal = (Terminal*)parent;
}

void Autocomplete::setProject(QString project)
{
	commands_entered << Projects::listCommandHistory( project );
	
	commands_entered.append(QDateTime::currentDateTime().toString("%% dddd MMMM d yyyy -- hh:mm:ss %%"));
	
	actual_command_entered=commands_entered.size()-1;
	
	completion_list=commands_entered;
	completion_list.removeDuplicates();
	completion_model->setStringList(completion_list);
}

void Autocomplete::load_from_file(const char *file)
{
	QFile input(file);

	if(input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in_stream(&input);
		QString word;

		while(!in_stream.atEnd())
		{
			in_stream >> word;
			word_list.push_back(word);
		}

		word_list.sort();
	}else
		cerr << "No se ha podido cargar " << file << endl;
}

void Autocomplete::add(QString word)
{
	word_list.push_back(word);
}

void Autocomplete::remove(QString word)
{
	int i = word_list.indexOf(QRegExp(word, Qt::CaseInsensitive));
	word_list.removeAt(i);
}

void Autocomplete::clear()
{
	word_list.clear();
	current_match = first_match = word_list.end();
}

QString Autocomplete::search(QString start)
{
	QStringList::iterator i;
	QString word;

	search_string = start;

	for(i = word_list.begin(); i != word_list.end(); i++)
	{
		if(i->startsWith(start))
		{
			current_match = first_match = i;
			return *i;
		}
	}

	current_match = first_match = word_list.end();
	return QString("");
}

QString Autocomplete::get_next()
{
	if(first_match != word_list.end())
	{
		current_match++;

		if(!current_match->startsWith(search_string, Qt::CaseInsensitive))
			current_match = first_match;
		
		return *current_match;
	}else
		return QString("");
}


static bool is_variable(QChar ch)
{
	if(ch.isLetterOrNumber() || ch=='_' ) return true;
	return false;
}

/*Search last word entered*/
static QString last_word(QString _text, int pos=-1)
{
	QString start;

	int i;
	
	if(pos<0) i = _text.size() - 1;
	else i=pos;

	//Get the last word being writted
	for(; i >= 0 && is_variable(_text[i]); i--);

	if(i == -1)
		start = _text.left(pos+1);
	else
		start = _text.mid(i+1, pos-i);
	
	return start;
}

/* event function reimplemented */
bool Autocomplete::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *key_event = (QKeyEvent*)event;
		if(key_event->key() == Qt::Key_Tab)
		{
			
			
			// Autocomplete
			if(!tab_flag)
			{
				QString start;
				QString _text = text();
				
				start=last_word(_text, cursorPosition()-1 );
				
				completion_list=commands_entered;
				completion_list.removeDuplicates();
				completion_model->setStringList(completion_list);
				
				QString command= "_ide_completion_matches(\""+start+"\");";
				
				octave_connection->command_enter(command,false);
			}

			tab_flag = true;
			// Event accepted
			key_event->accept();
			return TRUE;
		}
		else
		{
			tab_flag = false;
		}
	}

	return QWidget::event(event);
}


void Autocomplete::keyPressEvent ( QKeyEvent * event )
{
	if(!commands_entered.isEmpty() && Qt::Key_Up==event->key())
	{
		if(actual_command_entered>=0 && actual_command_entered<commands_entered.size())
			setText(commands_entered[actual_command_entered]);
		--actual_command_entered;
		if(actual_command_entered<=0)
		{
			actual_command_entered=0;
		}
	}
	else if(!commands_entered.isEmpty() && Qt::Key_Down==event->key())
	{
		actual_command_entered++;
		if(actual_command_entered>=commands_entered.size())
		{
			actual_command_entered=commands_entered.size()-1;
			setText("");
		}
		else setText(commands_entered[actual_command_entered]);
	}
	else if(Qt::Key_Enter==event->key()||Qt::Key_Return==event->key())
	{
		if( !commands_entered.isEmpty() && (commands_entered.last()==text()) ) QLineEdit::keyPressEvent ( event );
		else
		{
			commands_entered << text();
			while(commands_entered.size()>127)
			{
				commands_entered.removeFirst();
			}
			
			completion_list=commands_entered;
			completion_list.removeDuplicates();
			completion_model->setStringList(completion_list);
			
			emit new_command_entered(commands_entered);
			
			QLineEdit::keyPressEvent ( event );
		}
		actual_command_entered=commands_entered.size()-1;
	}
    else if( event->key() == Qt::Key_F9 )
    {
        /* if the user presses the F9 key, we should send any selected
           text in the command line editor to octave, as a command.
        */
        if (this->selectedText().length() > 0)
        {
            // send the text as a command to octave, replacing any paragraph
            // separator characters with newline characters
            callingTerminal->command_enter(this->selectedText());
        }
    }
	else
	{
		QLineEdit::keyPressEvent ( event );
	}
}

QStringList Autocomplete::commands()
{
	return commands_entered;
}

void Autocomplete::set_octave_connection(OctaveConnection *oc)
{
	octave_connection=oc;
	connect(octave_connection,SIGNAL(line_ready(QString )), this, SLOT(add_completion_match(QString)) );
}

void Autocomplete::add_completion_match(QString line)
{
	if(line.startsWith("~~Completion:"))
	{
		QString word=line.trimmed();
		word=word.mid(13);
		
		QString start=last_word(text(), cursorPosition()-1 );
		QString _text=text().replace(cursorPosition()-start.length(), start.length(), word );
		completion_list.append(_text);
		completion_list.removeDuplicates();
		completion_model->setStringList(completion_list);
		completer.setCompletionPrefix(text().left(cursorPosition()-1));
		completer.complete();
	}
}



