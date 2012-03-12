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

#ifndef __AUTOCOMPLETE_H__
#define __AUTOCOMPLETE_H__
#include <QStringList>
#include <QString>
#include <QListWidget>
#include <QLineEdit>
#include <QStringList>
#include <QCompleter>
#include <QStringListModel>
#include "octave_connection.h"
#include "projects/projects.h"

class Terminal;

/** This class is a one line text editor with built in autocompletion
 *  of strings
 */
class Autocomplete:public QLineEdit
{
    Q_OBJECT

    // This will be used to access the functions of the
    // the parent terminal widget
    Terminal *callingTerminal;

    private:

        QStringList word_list;
        QStringList::const_iterator first_match, current_match;
        QString search_string;
        bool tab_flag;

        QStringList commands_entered;
        int actual_command_entered;
        QStringList completion_list;

        QCompleter completer;
        QStringListModel *completion_model;

        OctaveConnection *octave_connection;


    protected:

        bool event(QEvent *event);

        /** Searches for the first match and returns it.
         *  @param start The search pattern.
         *  @return The first match, or start if there's no matches.
         */
        QString search(QString start);

        /** Get the next match. After the last match comes the first one.
         *  @return The next match or the search pattern if there's no matches.
         */
        QString get_next();

        void keyPressEvent ( QKeyEvent * event );


    public:

        Autocomplete(QWidget *parent);

        /** Load from a text file the word list. It doesn't clear the previous loaded list.
         *  @param file A plain text file that contains the word list.
         */
        void load_from_file(const char *file);

        /** Add a word to the list.
         *  @param word The word that is going to be added.
         */
        void add(QString word);

        /** Remove a word from the list.
         *  @param word The word that is going to be removed.
         */
        void remove(QString word);

        /**Clears the list
         */
        void clear();

        /**List of commands.
         */
        QStringList commands();

        void set_octave_connection(OctaveConnection *oc);

        void setProject(QString project);


    public slots:

        void add_completion_match(QString line);


    signals:

        void new_command_entered(QStringList list);
};

#endif
