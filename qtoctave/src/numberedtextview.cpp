/* This file is part of the KDE libraries
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include <QTextDocument>
#include <QTextBlock>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QToolTip>
#include <QTextStream>
#include <QProcess>
#include <QRegExp>
#include <QMessageBox>
#include <QFileInfo>
#include <QString>

#include "numberedtextview.h"
#include "numberbar.h"
#include "config.h"


NumberedTextView::NumberedTextView( QWidget *parent, SimpleEditor *textEdit, OctaveConnection *oc ): QFrame( parent )
{
    // Initialize the NumberedTextView
    this->initialize(parent, textEdit);

    // Set the connection to octave
    this->setOctaveConnection(oc);
}

void NumberedTextView::initialize(QWidget *parent, SimpleEditor *textEdit)
{
    setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    setLineWidth( 2 );

    view = textEdit;
    view->installEventFilter( this );

    connect( view->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(textChanged(int,int,int)) );

    connect( view, SIGNAL(cursorPositionChanged()), this, SLOT(cursor_moved_cb()) );

    // Setup the line number pane
    numbers = new NumberBar( this );
    numbers->setTextEdit( view );
    //numbers=NULL;

    vbox = new QVBoxLayout(this);
    vbox->setSpacing( 0 );
    vbox->setMargin( 0 );

    hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->setSpacing( 0 );
    hbox->setMargin( 0 );
    hbox->addWidget( numbers );
    hbox->addWidget( view );

    textModifiedOk = false;

    QHBoxLayout *messages_layout = new QHBoxLayout;
    vbox->addLayout(messages_layout);
    messages_layout->setSpacing( 0 );
    messages_layout->setMargin( 0 );

    line_column_label = new QLabel("Line: 1 Col: 1", this);
    messages_layout->addWidget(line_column_label);
    line_column_label->show();

    // Set the octave connection to nothing
    this->setOctaveConnection(NULL);

}

void NumberedTextView::setOctaveConnection(OctaveConnection *oc)
{
    octave_connection = oc;

    // Set the octave connection for the NumberBar also
    numbers->setOctaveConnection(oc);
}

NumberedTextView::~NumberedTextView()
{
	hide();
	//printf("Borrado ntv\n");
	SimpleEditor *_textEdit=textEdit();
	_textEdit->setDocument(NULL);
}

void NumberedTextView::setCurrentLine( int lineno )
{
	currentLine = lineno;

    if(numbers != NULL)
        numbers->setCurrentLine( lineno );
	
	//Move cursor to lineno
	if(lineno>-1)
	{
		QTextCursor cursor=textEdit()->textCursor();
		
		cursor.movePosition(QTextCursor::Start);
		
        for(int i=1; i<lineno; i++)
			cursor.movePosition(QTextCursor::NextBlock);
		
		textEdit()->setTextCursor(cursor);
	}
	
	textChanged( 0, 0, 1 );
}

void NumberedTextView::toggleBreakpoint( int lineno )
{
    if( numbers != NULL )
        numbers->toggleBreakpoint( lineno );
}

void NumberedTextView::setBugLine( int lineno )
{
    if( numbers != NULL )
        numbers->setBugLine( lineno );
}

void NumberedTextView::textChanged( int /*pos*/, int removed, int added )
{
    //Q_UNUSED( pos );

    if ( removed == 0 && added == 0 )
        return;

    //QTextBlock block = highlight.block();
    //QTextBlock block = view->document()->begin();
    //QTextBlockFormat fmt = block.blockFormat();
    //QColor bg = view->palette().base().color();
    //fmt.setBackground( bg );
    //highlight.setBlockFormat( fmt );
    /*
    QTextBlockFormat fmt;

    int lineCount = 1;
    for ( QTextBlock block = view->document()->begin();
	  block.isValid() && block!=view->document()->end(); block = block.next(), ++lineCount ) {

	if ( lineCount == currentLine )
	{
	    fmt = block.blockFormat();
	    QColor bg = view->palette().highlight().color();
	    fmt.setBackground( bg );

	    highlight = QTextCursor( block );
	    highlight.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
	    highlight.setBlockFormat( fmt );

	    break;
	}
    }
    */
    
    if( !textModifiedOk && view->document()->isModified() )
    {
    	textModifiedOk=true;
    	emit textModified();
    }
}

bool NumberedTextView::eventFilter( QObject *obj, QEvent *event )
{
    if ( obj != view )
        return QFrame::eventFilter(obj, event);

    if ( event->type() == QEvent::ToolTip )
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

        QTextCursor cursor = view->cursorForPosition( helpEvent->pos() );
        cursor.movePosition( QTextCursor::StartOfWord, QTextCursor::MoveAnchor );
        cursor.movePosition( QTextCursor::EndOfWord, QTextCursor::KeepAnchor );

        QString word = cursor.selectedText();
        emit mouseHover( word );
        emit mouseHover( helpEvent->pos(), word );

        // QToolTip::showText( helpEvent->globalPos(), word ); // For testing
    }

    return false;
}


QList<int> *NumberedTextView::getBreakpoints()
{
    QList<int> *br = NULL;

    if(numbers!=NULL)
        br = numbers->getBreakpoints();

	return br;
}

void NumberedTextView::open(QString path)
{
    FILE *fl;

    fl = fopen(path.toLocal8Bit().constData(), "rt");
    if(fl)
    {
        fclose(fl);
        filePath = path;

        textEdit()->load(path);

        textModifiedOk=false;
        textEdit()->document()->setModified(false);
    }else{
        throw path;
    }
}

bool NumberedTextView::save(QString path)
{
    FILE *fl;

    if(path.isEmpty()) path = filePath;
    QRegExp re("[A-Za-z_][A-Za-z0-9_]*\\.m");

    if( ! re.exactMatch( QFileInfo(path).fileName() ) )
    {
        QMessageBox msgBox;
        msgBox.setText( tr("This file name is not valid.") );
        msgBox.setInformativeText(tr("Octave doesn't understand this file name:\n")+path+tr("\nPlease, change it.\n Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret)
        {
		case QMessageBox::Save:
		    // Save was clicked
		    break;
		case QMessageBox::Cancel:
		    // Cancel was clicked
            return false;
		    break;
		default:
		    // should never be reached
		    break;
        }
    }


    fl = fopen(path.toLocal8Bit().constData(), "wt");
    if(fl)
    {
        filePath = path;
        QTextStream *stream = new QTextStream(fl);
        (*stream) << textEdit()->document()->toPlainText();
        delete stream;
        fclose(fl);
        textModifiedOk=false;
        view->document()->setModified(false);
    }else{
        return false;
    }

    if(get_config("simple_rcs")=="true")
    {
        QString repository=path+"~~";
        QString command("simplercs \""+repository+"\" \""+path+"\"");
        QProcess::startDetached(command);
        //QProcess::execute(command);
        printf("[NumberedTextView::save] Comando: %s\n", command.toLocal8Bit().data() );
    }
    else
    {
        //printf("[NumberedTextView::save] No rcs\n");
    }

    //Set syntax
    textEdit()->setFile(filePath);

    return true;
}

QString NumberedTextView::path()
{
    return filePath;
}

void NumberedTextView::setPath(QString path)
{
	filePath=path;
	textEdit()->setFile(path);
}

void NumberedTextView::setModified(bool modify)
{
	textModifiedOk=modify;
}

bool NumberedTextView::modified()
{
	return textModifiedOk;
}

void NumberedTextView::cursor_moved_cb()
{
	QTextCursor cursor=view->textCursor();
	QTextBlock actual_block=cursor.block();
	int lineCount=1;
	QTextBlock block = view->document()->begin();
	
	for ( ;block.isValid() && actual_block!=block; block = block.next()) lineCount++ ;
	
	int col= cursor.position() - block.position() + 1;
	
	line_column_label->setText("Line: "+QString::number(lineCount)+" Col: "+QString::number(col) );
}

static QString startLineInsertText(QString str, QString textToInsert)
{
	str.replace(QChar(0x2029), "\n");
	//printf("str=%s\n", str.toLocal8Bit().data() );
	
	QStringList list = str.split("\n");
	
	for(int i=0;i<list.size();i++)
	{
		QString s=list[i];
		
		int x;
		
		for(x=0;x<s.size();x++)
		{
			if( s.at(x)!=' ' && s.at(x)!='\t' ) break;
		}
		
		QString s1=s.left(x);
		QString s2=s.right(s.size()-x);
		//printf("s1=%s s2=%s\n", s1.toLocal8Bit().data(), s2.toLocal8Bit().data() );
		list[i]=s1+textToInsert+s2;
	}
	
	return list.join("\n");
}

static QString startLineRemoveText(QString str, QStringList textToRemove)
{
	str.replace(QChar(0x2029), "\n");
	//printf("str=%s\n", str.toLocal8Bit().data() );
	
	QStringList list = str.split("\n");
	
    for(int i = 0; i < list.size(); i++)
	{
        QString s = list[i];
		
		int x;
		
        for(x = 0; x < s.size(); x++)
		{
            if( s.at(x) != ' ' && s.at(x) != '\t' ) break;
		}
		
        QString s1 = s.left(x);

        QString s2 = s.right(s.size()-x);
		
        for(int k = 0; k < textToRemove.size(); k++)
		{
			if(s1.endsWith(textToRemove[k]))
			{
                s1 = s1.left(s1.size()-textToRemove[k].size());
				break;
			}
			else if(s2.startsWith(textToRemove[k]))
			{
                s2 = s2.right(s2.size() - textToRemove[k].size());
				break;
			}
		}
		
		//printf("s1=%s s2=%s \n", s1.toLocal8Bit().data(), s2.toLocal8Bit().data());
        list[i] = s1 + s2;
	}
	
	return list.join("\n");
}

void NumberedTextView::indent()
{
	//QTextDocument *doc=textEdit()->document();
	
	QTextCursor cursor(textEdit()->textCursor());
	
	if( !cursor.hasSelection() ) return;
	
	QString str=cursor.selectedText();
	
    str = startLineInsertText(str, "\t");
	
	cursor.insertText(str);

	cursor.setPosition(cursor.position()-str.size(), QTextCursor::KeepAnchor);

	textEdit()->setTextCursor(cursor);
}

void NumberedTextView::unindent()
{
	//QTextDocument *doc=textEdit()->document();
	
	QTextCursor cursor(textEdit()->textCursor());
	
	if( !cursor.hasSelection() ) return;
	
	QString str=cursor.selectedText();
	
	QStringList textToRemove;

	textToRemove << "\t" << " ";

    str = startLineRemoveText(str, textToRemove);
	
	cursor.insertText(str);

	cursor.setPosition(cursor.position()-str.size(), QTextCursor::KeepAnchor);

	textEdit()->setTextCursor(cursor);
}

static void findStartOfText(QTextCursor &cursor)
{
	cursor.movePosition(QTextCursor::StartOfBlock);

	cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);

    QString text = cursor.selectedText();

	//printf("[findStartOfText] >%s< %d\n", text.toLocal8Bit().data(), text.trimmed().length());
	if(text.trimmed().length()!=0)
	{
		cursor.movePosition(QTextCursor::StartOfBlock);
		//printf("[findStartOfText] Texto lleno\n");
	}

	cursor.movePosition(QTextCursor::StartOfWord);
}

void NumberedTextView::comment()
{
	//QTextDocument *doc=textEdit()->document();
	
	QTextCursor cursor(textEdit()->textCursor());
	
	if( !cursor.hasSelection() ) 
	{
		QTextCursor cursor_aux(cursor);
		findStartOfText(cursor);
		cursor.insertText("%");
		textEdit()->setTextCursor(cursor_aux);
		return;
	}
	
    QString str = cursor.selectedText();
	
	str=startLineInsertText(str, "%");
	
	cursor.insertText(str);
	cursor.setPosition(cursor.position()-str.size(), QTextCursor::KeepAnchor);
	textEdit()->setTextCursor(cursor);
}

void NumberedTextView::uncomment()
{
	//QTextDocument *doc=textEdit()->document();
	
	QTextCursor cursor(textEdit()->textCursor());
	
	if( !cursor.hasSelection() ) 
	{
		findStartOfText(cursor);
		cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
	}
	
	if( !cursor.hasSelection() ) return;
	
    QString str = cursor.selectedText();
	
	QStringList textToRemove;
	textToRemove << "%" << "#";
    str = startLineRemoveText(str, textToRemove);
	
	cursor.insertText(str);
	cursor.setPosition(cursor.position()-str.size(), QTextCursor::KeepAnchor);
	textEdit()->setTextCursor(cursor);
}

void NumberedTextView::setFontSize(int newfontsize)
{
    // Set the font size for the editor
    view->setFontSize(newfontsize);
}

SimpleEditor *NumberedTextView::getSimpleEditor()
{
    return view;
}
