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

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <QTextEdit>
#include <QPushButton>
#include <QMdiArea>
#include <QCloseEvent>
#include <QFile>
#include "basewidget.h"
#include "octave_connection.h"
#include "dynamic_help/dynamic_help.h"
#include "autocomplete.h"
#include "command_list.h"
#include "octoutputtextedit.h"


/** Builds windows with OctOutputTextEdit (Inheriting from QTextEdit) and Autocomplete
 *  (Inheiriting from QLineEdit). QTextEdit shows OctaveConnection output. OctOutputTextEdit
 *  is used to diplay the output from octave and Autocomplete is used to write commands.
 */
class Terminal:public BaseWidget
{
	Q_OBJECT

    public:

        Terminal( QWidget * parent = 0 );
        ~Terminal();

        // OctOutputTextEdit is a text display (Inheriting from QTextEdit)
        // used to display the output from octave
        OctOutputTextEdit *getTextEdit();

        void setOctaveConnection(OctaveConnection *octave_connection);

        OctaveConnection *getOctaveConnection();

        void command_enter(const QString &text);

        void setSession(Session *session);

        BaseWidget *copyBaseWidget( QWidget * parent = 0 );

        /** Use 'save' octave command to save variables. */
        void saveVariables();

        Autocomplete *getAutocomplete();

        QMdiArea *work_space;
        //Session *session;


    private:

        // Declare an OctOutputTextEdit to display the output from
        // octave. This class inheirits from QTextEdit
        OctOutputTextEdit *text;

        // The Autocomplete class inheirits QLineEdit, a one line text editor.
        // This is used to get input for octave from the user
        Autocomplete *commandLineEdit;

        QPushButton *show_commands_button;

        OctaveConnection *octave_connection;

        //Formats of entries
        QTextCharFormat normal_format, error_format,
        command_format, ide_command_format;

        bool diary_ok; //Controls diary output
        QFile diary_file; //File to save diary

        bool show_ide_commands_ok;

        int lines_in_terminal;

        int cols_in_terminal;

        void remove_lines(QTextCursor &cursor);

        void closeEvent(QCloseEvent*);

        /** Regular expresions:*/
        QRegExp regexp_octave_prompt; //Used in remove_lines
        QRegExp re; //Used in line_ready

        /** Init most used regular expresions*/
        void init_regular_expresions();


    protected:

        //Drag and drop
        void dragEnterEvent(QDragEnterEvent *event);

        void dropEvent ( QDropEvent * event );

        virtual void keyPressEvent(QKeyEvent * event);

    public slots:

        void return_pressed();

        void write_output(QString output);

        void write_error(QString error);

        void write_command(QString command);

        void write_ide_command(QString command);

        void completion_matches_callback();

        void stop_process_callback();

        void clear_callback();

        void clear_command_line_first_time();

        CommandList *show_commands_callback();

        void textChanged(const QString &text);

        void setProject(QString project=QString());

        void line_ready(QString line);
};

#endif
