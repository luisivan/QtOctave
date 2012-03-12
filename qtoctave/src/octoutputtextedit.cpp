#include "terminal.h"
#include <QKeyEvent>
#include "octoutputtextedit.h"

/** This class will be used to display the output from octave
 *  it inheirits QTextEdit class and will be used to capture key
 *  press events from the parent
 */
OctOutputTextEdit::OctOutputTextEdit(QWidget *parent):QTextEdit(parent)
{
    // get the terminal Widget which created the OctOutputTextEdit
    callingTerminal = (Terminal*)parent;
}

void OctOutputTextEdit::keyPressEvent(QKeyEvent * e)
{
    //printf("%d %s\n",e->key(), e->text().toLocal8Bit().data());
    octave_connection = callingTerminal->getOctaveConnection();

    if( e->key() == Qt::Key_F9 )
    {
        /* if the user presses the F9 key, we should send any selected
           text in the command window to octave, as a command.
        */

        // Get the cursor
        QTextCursor cursor = textCursor();

        if (cursor.selectedText().length() > 0)
        {
            // send the text as a command to octave, replacing any paragraph
            // separator characters with newline characters
            callingTerminal->command_enter(cursor.selectedText().replace(QChar(QChar::ParagraphSeparator), "\n"));
        }

    }
    else
    {
        // ignore the key press, it will be handled by the parent
        // Terminal widget, (which will pass it to the commandLineEdit
        // and switch focus to it)
        e->ignore();
    }

}

OctOutputTextEdit::~OctOutputTextEdit()
{
    printf("[OctOutputTextEdit::~OctOutputTextEdit] Entered\n");
}

