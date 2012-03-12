#include <QTextDocument>
#include <QTextBlock>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QPainter>
#include <QToolTip>
#include <QProcess>
#include <QRegExp>
#include <QMessageBox>
#include <QFileInfo>
#include <QString>
#include <cmath>

#include "numberbar.h"
#include "numberedtextview.h"

NumberBar::NumberBar( QWidget *parent )
    : QWidget( parent ), edit(0), currentLine(-1), bugLine(-1)
{
    this->initialize(parent);

    NTV = ((NumberedTextView*) parent);
}

NumberBar::NumberBar( QWidget *parent, OctaveConnection *oc)
    : QWidget( parent ), edit(0), currentLine(-1), bugLine(-1)
{
    this->initialize(parent);

    this->setOctaveConnection(oc);
}

NumberBar::~NumberBar()
{
}

void NumberBar::initialize(QWidget *parent)
{
    // set the octave connection to NULL
    this->setOctaveConnection(NULL);

    // Make room for 4 digits and the breakpoint icon
    setFixedWidth( fontMetrics().width( QString("0000") + 10 + 32 ) );

    stopMarker = QPixmap( QString(ICON_PATH) + "/stop.png" );

    currentMarker = QPixmap( QString(ICON_PATH) + "/bookmark.png" );

    bugMarker = QPixmap( QString(ICON_PATH) + "/bug.png" );

    //edit = NTV->getSimpleEditor();
}

void NumberBar::setOctaveConnection(OctaveConnection *oc)
{
    this->octave_connection = oc;
}

void NumberBar::setCurrentLine( int lineno )
{
    currentLine = lineno;
    update();
}

void NumberBar::setBugLine( int lineno )
{
    bugLine = lineno;
}

void NumberBar::toggleBreakpoint( int lineno )
{

    if(lineno > 0)
    {
        // check to see if we can find this line number in the list
        // of current break points
        int i = breakpoints.indexOf(lineno);

        if(i > -1)
        {
            // If a break point exists on this line, remove it
            breakpoints.removeAt(i);

            // If we are debugging, remove the break point in the file
            // immediately
            if (octave_connection->isDebugging())
            {
                QString filename = NTV->path();

                QFileInfo finfo(filename);

                octave_connection->removeBreakPoint(finfo.baseName(), lineno);
            }
        }
        else
        {
            // first find the nearest valid line of code (with an
            // executeable statement on it)
            lineno = findValidCodeLine(lineno);

            if (lineno > 0)
            {
                // check again to see if we can find this new line number in
                // the list of current break points
                i = breakpoints.indexOf(lineno);

                if(i == -1)
                {
                    // then insert a break point
                    breakpoints.push_back(lineno);

                    // If we are debugging, insert the break point in the file
                    // immediately
                    if (octave_connection->isDebugging())
                    {
                        QString filename = NTV->path();

                        QFileInfo finfo(filename);

                        octave_connection->addBreakPoint(finfo.baseName(), lineno);
                    }
                }
            }
        }
    }

    update();

}

int NumberBar::findValidCodeLine(int lineno)
{
    QString currentLine;

    //edit->syntax->currentBlock()

    int startlineno = lineno;

    // find the next line not starting with a comment
    QRegExp commentRegExp("^[%#]");

    while (lineno <= edit->document()->blockCount())
    {
        // search downwards for the next valid line
        currentLine = edit->getTextLine(lineno).trimmed();

        if (currentLine.isEmpty() || commentRegExp.indexIn(currentLine, 0) != -1)
        {
            lineno++;
        }
        else
        {
            return lineno;
        }
    }

    if ( lineno > edit->document()->blockCount() && startlineno > 1)
    {
        lineno = startlineno - 1;

        while ( lineno >=1 )
        {
            // search downwards for the next valid line
            currentLine = edit->getTextLine(lineno).trimmed();

            if (currentLine.isEmpty() || commentRegExp.indexIn(currentLine, 0) != -1)
            {
                lineno--;
            }
            else
            {
                return lineno;
            }
        }
    }

    return -1;
}


void NumberBar::setTextEdit( SimpleEditor *edit )
{
    this->edit = edit;

    setFixedWidth( edit->fontMetrics().width( QString("0000") + 10 + 32 ) );

    connect( edit->verticalScrollBar(), SIGNAL(valueChanged(int) ),
             this, SLOT( update() ) );

    connect( edit->document()->documentLayout(), SIGNAL( update(const QRectF &) ),
             this, SLOT( update() ) );

}

void NumberBar::paintEvent( QPaintEvent * )
{

    QVector<qreal> lines_list;

    // Get the current numberbar font, for writing the line numbers
    QFont numberBarFont = this->font();

    // Get the font size that is currently being used in the editor
    int font_size = edit->getFontSize();

    // Set the number bar font size to the same size
    numberBarFont.setPointSize(font_size);
    this->setFont(numberBarFont);

    // Get info about the font currently being used in the editor so that
    // we can compute the correct positions of the number bar elements,
    // i.e. the line number text and any markers
    const QFontMetrics fm = edit->fontMetrics();

    // Get the height of the text being used in the editor
    const int ascent = fontMetrics().ascent() + 1; // height = ascent + descent + 1

    // get the full height of the text line
    int linespacing = fm.lineSpacing();

    // Ensure there is enough space in the number bar
    //setFixedWidth( edit->fontMetrics().width( QString("0000") + 10 + 32 ) );
    this->setFixedWidth(this->fontMetrics().width( QString("0000") ) + linespacing);

    // We will put the number of the first line in the following variable and use it
    // start the count of line numbers for the text in the NumberBar
    int first_line_no;

    // publicBlockBoundingRectList gets a list of the vertical positions
    // of the top of each line of text currently shown in the editor and
    // the line number of the first line shown
    edit->publicBlockBoundingRectList(lines_list, first_line_no);

    // Declare a QPainter object to do the drawing of all the elements
    // of the number bar
    QPainter p(this);

    p.setPen(palette().windowText().color());

// The following statements are no longer required
//    bugRect = QRect();
//    stopRect = QRect();
//    currentRect = QRect();

    // This will store the position of the current line. The
    // vertical position is defined as being positive in the
    // downward direction from the top of the number bar
    // widget
    int position_y;

    int lineCount;

    const int lines_list_size = lines_list.size();

    for(int i=0; i < lines_list_size; i++)
    {
        // fing the top of the current line
        position_y = qRound( lines_list[i] );

        // increment the line count
        lineCount = first_line_no + i;

        // get the line number text
        const QString txt = QString::number( lineCount );

        // Bug marker
        if ( bugLine == lineCount )
        {
            p.drawPixmap( 1, position_y, bugMarker.scaled(linespacing, linespacing, Qt::KeepAspectRatioByExpanding) );
            bugRect = QRect( 19, position_y, bugMarker.width(), bugMarker.height() );
        }

        // Stop (break point) marker
        if ( breakpoints.contains(lineCount) )
        {
            QLinearGradient linearGrad = QLinearGradient(1, position_y, 1 + linespacing, position_y + linespacing);
            linearGrad.setColorAt(0, Qt::black);
            linearGrad.setColorAt(0.1, Qt::red);
            linearGrad.setColorAt(1, Qt::white);
            linearGrad.setSpread( QGradient::PadSpread );

            QBrush breakBrush = QBrush(linearGrad);

            p.setBrush(breakBrush);

            p.drawEllipse(1, position_y + 0.1*linespacing, 0.8*linespacing, 0.8*linespacing);

            p.setBrush(Qt::NoBrush);

            //p.drawPixmap( 1, position_y, stopMarker.scaled(linespacing, linespacing, Qt::KeepAspectRatioByExpanding) );
            //stopRect = QRect( 1, position_y, stopMarker.width(),  stopMarker.height() );
        }

        // Current line marker
        if ( currentLine == lineCount )
        {
            p.drawPixmap( 1, position_y, currentMarker );
            currentRect = QRect( 1, position_y, currentMarker.width(), currentMarker.height() );
        }

        // Finally add the line number text
        p.drawText( width() - fm.width(txt), position_y, fm.width(txt), linespacing + fm.descent(), Qt::AlignVCenter, txt );


    }

}

bool NumberBar::event( QEvent *event )
{

    if ( event->type() == QEvent::ToolTip )
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

        if ( stopRect.contains( helpEvent->pos() ) )
        {
            QToolTip::showText( helpEvent->globalPos(), tr("Stop Here"));
        }
        else if ( currentRect.contains( helpEvent->pos() ) )
        {
            QToolTip::showText( helpEvent->globalPos(), tr("Current Line"));
        }
        else if ( bugRect.contains( helpEvent->pos() ) )
        {
            QToolTip::showText( helpEvent->globalPos(), tr("Error Line" ));
        }
    }

    return QWidget::event(event);
}

/** Handles the mouse press event on the NumberBar, toggling the existance
  * of a break point on a given line in the editor.
  */
void NumberBar::mousePressEvent(QMouseEvent *event)
{
    // if the user left-clicks on the number bar, and the current
    // file is an m-file or oct-file we will toggle a break point on
    // the appropriate line
    if ( event->button() == Qt::LeftButton && edit->getFileType() == SimpleEditor::MFILE )
    {

        QVector<qreal> lines_list;

        int first_line_no;

        edit->publicBlockBoundingRectList(lines_list, first_line_no);

        int offset = qRound(lines_list[0]);

        // Get the position of the mouse click
        QPoint lastPoint = event->pos();

        const QFontMetrics fm = edit->fontMetrics();

        const int lines_list_size = edit->blockCount();

        // Get the line number that was clicked on based on the height of
        // one line and the location of the mouse click
        int clickedLineNo = floor(first_line_no + ((lastPoint.y()-offset) / fm.lineSpacing()));

        if (clickedLineNo > lines_list_size)
        {
            clickedLineNo = lines_list_size;
        }

        // Toggle the breakpoint on the selected line
        this->toggleBreakpoint(clickedLineNo);
    }

}

QList<int> *NumberBar::getBreakpoints()
{
    return &breakpoints;
}
