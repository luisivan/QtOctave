#ifndef NUMBERBAR_H
#define NUMBERBAR_H

#include <QProcess>
//#include <QFrame>
#include <QPixmap>
//#include <QTextCursor>
//#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "octave_connection.h"
#include "simpleeditor.h"
#include "numberedtextview.h"

/**
 * @internal Used to display the numbers.
 */
class NumberBar : public QWidget
{
    Q_OBJECT

    NumberedTextView* NTV;

public:

    NumberBar( QWidget *parent );
    NumberBar( QWidget *parent, OctaveConnection *oc );
    ~NumberBar();

    void setCurrentLine( int lineno );

    void setBugLine( int lineno );

    void toggleBreakpoint( int lineno );

    QList<int> *getBreakpoints();

    void setTextEdit( SimpleEditor *edit );

    void paintEvent( QPaintEvent *ev );

    // sets the connection to octave
    void setOctaveConnection( OctaveConnection *oc );


protected:

    /** Handles the mouse press event on the NumberBar, useful for
      */
    void mousePressEvent(QMouseEvent *event);

    bool event( QEvent *ev );


private:

    SimpleEditor *edit;

    QPixmap stopMarker;

    QPixmap currentMarker;

    QPixmap bugMarker;

    QList<int> breakpoints;

    int currentLine;

    int bugLine;

    QRect stopRect;

    QRect currentRect;

    QRect bugRect;

    // Declare a connection to octave
    OctaveConnection *octave_connection;

    // initializes the NumberBar object
    void initialize(QWidget *parent);

    // Finds the nearest uncommented and non white space line
    // to the line number passed in
    int findValidCodeLine(int lineno);


signals:

    void numberBarLeftClick(int linenumber);
};

#endif // NUMBERBAR_H
