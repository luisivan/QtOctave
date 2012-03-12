/* Copyright (C) 2010 P.L. Lucas
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

#include "config.h"
#include "simpleeditor.h"
#include <QFile>
#include <QTextStream>
#include <QTextBlock>
#include <QFileInfo>
#include <QDir>

SimpleEditor::SimpleEditor(QWidget *parent):QPlainTextEdit(parent)
{
    // Initialize the syntax highlighter to null, this will
    // be set to the appropriate value later dependent on
    // the file type
    syntax = NULL;

    firstTimeUsedOk = true;

    thisFileType = SimpleEditor::OTHERFILE;

    // don't wrap long lines in the editor, force the use of
    // a horizontal acroll bar
    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    {
        // Create a new QTextDocument to hold the actual text of the file
        // being edited
        QTextDocument *_document = new QTextDocument();

        _document->setProperty("DocumentReferences", QVariant(0));

        QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(_document);

        _document->setDocumentLayout(layout);

        // Ensure we do not wrap lines in the document by setting the
        // text width to be infinite, we may want to change this for
        // some file types, e.g. .txt
        _document->setTextWidth(-1);

        setDocument(_document);
    }

    // Set up some properties related to the auto completion
    completerModel = new QStringListModel ();

    completer = new QCompleter(completerModel, this);

    completer->setCompletionMode(QCompleter::PopupCompletion);

    completer->setWidget(this);

    connect(completer, SIGNAL(activated ( const QString &)), this, SLOT(activated ( const QString &)));

    if(get_config("bracketsMatch") != "false")
    {
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedCallBack()));
    }

    if((get_config("autoCompletion") != "false"))
    {
        connect(document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(autoComplete(int, int, int)));
    }

    // Set auto-indentation mode dependent on user preferences
    auto_indent = ("false" != get_config("autoindent"));

    automatic_indention_statement_ok = (get_config("autoindent_statements") == "true");

    minFontSize = 5;
    maxFontSize = 30;
    int font_size = minFontSize;

    // Set editor's font, loading the values for the font
    // and font size from the configuration files
    QFont text_edit_font;

    QString font_name = get_config("textEditFont");

    QString font_size_str = get_config("textEditFontSize");

    // Set the default font and size if not supplied in
    // the config files
    if(font_name.isEmpty())
    {
        font_name = text_edit_font.family();
    }

    if(font_size_str.isEmpty())
    {
        font_size_str = QString::number(text_edit_font.pointSize());
    }
    else
    {
        font_size = font_size_str.toInt();

        if (font_size < minFontSize)
            font_size = minFontSize;

        if (font_size > maxFontSize)
            font_size = maxFontSize;
    }

    // Now apply the font choices to the editor
    text_edit_font.setFamily(font_name);

    text_edit_font.setPointSize(font_size);

    setFont(text_edit_font);

    // set the string inserted when the tab key is pressed
    // to be four space characters
    setTabInsertStr("    ");

}

void SimpleEditor::loadSyntaxXMLDescription()
{
	printf("[SimpleEditor::loadSyntaxXMLDescription] Loading syntax\n");

	QString installPath= QString(CONFIG_PATH) +QDir::separator()+"syntax_files"+QDir::separator();

	QFileInfo file(fileName);

	QString suffix=file.suffix();

    // Set the editor filetype based  on the suffix
    if ( QString::compare(suffix, "m", Qt::CaseInsensitive) || QString::compare(suffix, "oct", Qt::CaseInsensitive))
    {
        thisFileType = SimpleEditor::MFILE;
    }
    else if ( QString::compare(suffix, "cpp", Qt::CaseInsensitive) || QString::compare(suffix, "c++", Qt::CaseInsensitive) || QString::compare(suffix, "c", Qt::CaseInsensitive) )
    {
        thisFileType = SimpleEditor::CPPFILE;
    }
    else if (QString::compare(suffix, "txt", Qt::CaseInsensitive))
    {
        thisFileType = SimpleEditor::TXTFILE;
    }
    else
    {
        thisFileType = SimpleEditor::OTHERFILE;
    }

	if(comands_completion_list.isEmpty())
	{
		QString home=QDir::home().path()+QDir::separator()+".qtoctave"+QDir::separator()+"commands.txt";

		QFile file(home);

		if (file.open(QFile::ReadOnly))
		{
			char buf[1024];

			while(file.readLine(buf, sizeof(buf))>=0)
			{
				comands_completion_list.append(QString(buf).trimmed());
			}

			file.close();
		}
	}
	
	if(get_config("syntaxHighlighting")!="true") return;
	
	QFileInfo xml(installPath+suffix+".xml");

	if(xml.exists())
	{
		printf("[SimpleEditor::loadSyntaxXMLDescription] Loading syntax\n");
		if(syntax==NULL)
		{
			syntax=new Syntax( document() );
			syntax->load(xml.absoluteFilePath());
			syntax->setDocument(document());
		}
		else
			syntax->load(xml.absoluteFilePath());
	}
}

bool SimpleEditor::load(QString file)
{
	if(file.isEmpty())
	{
		setPlainText("");
		fileName=file;
		return true;
	}
	
	FILE *input=fopen(file.toLocal8Bit().data(),"r");
	if(input==NULL) return false;
	fclose(input);
	QFile in(file);
	if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QByteArray data=in.readAll();
	
	setPlainText( QString::fromLocal8Bit(data) );
	fileName=file;
	
    firstTimeUsedOk=false;
	
	loadSyntaxXMLDescription();
	
	return true;
}

bool SimpleEditor::save()
{
	QFile::remove(fileName+"~");

	QFile::copy(fileName, fileName+"~");

    FILE *out = fopen(fileName.toLocal8Bit().data(),"w");

    if(out == NULL) return false;

	fprintf( out, "%s", toPlainText().toLocal8Bit().data() );

	fclose(out);

	document()->setModified(false);

	return true;
}

void SimpleEditor::keyPressEvent(QKeyEvent * e)
{
    //printf("%d %s\n",e->key(), e->text().toLocal8Bit().data());

    // In all cases the completer popup must be hidden.
    if( (e->key() != Qt::Key_Return) && (e->key() != Qt::Key_Enter) )
    {
        QAbstractItemView *view = completer->popup();

        if(view->isVisible()) view->hide();
        //completer->setWidget(NULL);
    }

    if( (e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter) )
    {
        QAbstractItemView *view = completer->popup();

        if( view->isVisible() )
        {
            QString word = view->currentIndex().data().toString();

            if( word.isEmpty() )
            {
                word = completer->currentCompletion();

                activated( word );

                //return;
            }
        }

        if(auto_indent)
        {
            // get the cursor
            QTextCursor cursor = textCursor();
            // get the line of text the cursor is on
            QString line = cursor.block().text();
            // create a string to store the indentation of the current
            // line and initialise to the contents of the first line
            QString lastlineindent = line;

            // examine each character on the line until we come to a character
            // which is not a space or a tab character
            for(int i=0; i < line.length(); i++)
            {
                if(line[i] != ' ' && line[i] != '\t')
                {
                    // extract just the tab and space characters from the start
                    // of the line by trimming off the end of the string
                    lastlineindent.resize(i);
                    break;
                }
            }

            if( automatic_indention_statement_ok )
            {
                printf("[SimpleEditor::keyPressEvent] automatic_indention_statement_ok=%s\n", line.toLocal8Bit().data() );

                if ( thisFileType == SimpleEditor::MFILE )
                {
                    // we will create a regular expression to search for opening code sequences
                    QRegExp openCodeBlockRegExp("^\\b(while(\\b|[\\(\\[])"
                                                    "|if(\\b|[\\(\\[])"
                                                    "|for(\\b|[\\(\\[])"
                                                    "|switch(\\b|[\\(\\[])"
                                                    "|do(\\b|[\\(\\[])"
                                                    "|try(\\b|[\\(\\[])"
                                                    "|function(\\b|\\[)"
                                                    "|else(\\b|[\\(\\[])"
                                                    "|elseif(\\b|[\\(\\[])"
                                                    "|catch(\\b|[\\(\\[])"
                                                    "|case(\\b|[\\(\\[])"
                                                    "|otherwise(\\b|[\\(\\[]))", Qt::CaseSensitive, QRegExp::RegExp2);

                    // and another for closing sequences
                    QRegExp closeCodeBlockRegExp("(\\b|[,;])(end(?!\\s*\\S)|end\\s*;)");

                    QString newstatementstr = line.trimmed();

                    // perform a test to see if any opening code block statements
                    // were matched on the line
                    bool closetest = (closeCodeBlockRegExp.indexIn(newstatementstr, 0) != -1);

                    // perform a test to see if any closing code block statements
                    // were matched on the line
                    bool opentest = (openCodeBlockRegExp.indexIn(newstatementstr, 0) != -1);

                    if (opentest & closetest)
                    {
                        /* the line may contain both an opening and a closing statement
                       e.g. if x, y=1, end; or even worse:

                        if x
                            y = 1;
                        end;while 1
                            y = 1;
                        end

                       we should try and take appropriate action in either case to
                       match the subsequent end statement to the opening 'while'
                     */
                        if (openCodeBlockRegExp.pos() > closeCodeBlockRegExp.pos())
                        {
                            // Insert whatever the tab spacing is in addition to
                            // the current indentaion as a new code block is being opened
                            cursor.insertText("\n" + lastlineindent + tabInsert);
                        }
                        else
                        {
                            // the closing code statement is on the same line e.g.
                            // if (1), disp('hello'), end; so don't add another tab
                            cursor.insertText("\n" + lastlineindent);
                        }
                    }
                    else if(opentest)
                    {
                        // Otherwise insert whatever the tab spacing is in addition to
                        // the current indentaion as a new code block is being opened
                        cursor.insertText("\n" + lastlineindent + tabInsert);
                    }
                    else if (closetest)
                    {
                        // search above for an open code block, if we find one, reduce
                        // the indent on the current line to the same spacing as on
                        // that line

                        // store the current cursor position
                        int cursorPos = cursor.position();

                        int linenumber = cursor.blockNumber() - 1;

                        int codeblockcount = 0;

                        // change the pattern so it does not look for secondary codewords
                        // and just the main opening statements to match the end
                        openCodeBlockRegExp.setPattern("^\\b(while(\\b|[\\(\\[])"
                                                           "|if(\\b|[\\(\\[])"
                                                           "|for(\\b|[\\(\\[])"
                                                           "|switch(\\b|[\\(\\[])"
                                                           "|do(\\b|[\\(\\[])"
                                                           "|try(\\b|[\\(\\[])"
                                                           "|function(\\b|\\[))");

                        while ( linenumber != -1 )
                        {
                            // go to the next line up
                            cursor.movePosition(QTextCursor::PreviousBlock);

                            // get the text from the current line
                            line = cursor.block().text();

                            closetest = (closeCodeBlockRegExp.indexIn(line.trimmed(), 0) != -1);

                            opentest = (openCodeBlockRegExp.indexIn(line.trimmed(), 0) != -1);

                            // look to see if the current line contains one of the opening code
                            // block statements, if it does, we will copy its indentation.
                            if (closetest & opentest)
                            {
                                // if the closing statement was before the opening statement
                                // on the line we must count this closing statement as the end
                                // of another nested code block, so increment the codeblock counter
                                if (openCodeBlockRegExp.pos() > closeCodeBlockRegExp.pos())
                                {
                                    codeblockcount++;
                                }
                            }
                            else if (closetest)
                            {
                                codeblockcount++;
                            }
                            else if (codeblockcount == 0 & opentest)
                            {
                                // examine each character on the line until we come to a character
                                // which is not a space or a tab character
                                for(int i=0; i < line.length(); i++)
                                {
                                    if(line[i] != ' ' && line[i] != '\t')
                                    {
                                        // extract any tab and space characters from the line
                                        // by trimming off the end of the string
                                        lastlineindent = line;
                                        lastlineindent.resize(i);
                                        break;
                                    }
                                }

                                // insert a new line character and the spacing of the same size as for
                                // the opening of the code block
                                cursor.setPosition(cursorPos);

                                cursor.select(QTextCursor::BlockUnderCursor);

                                // remove the existing line and replace with one
                                // with matching indentation
                                cursor.beginEditBlock();

                                cursor.insertText("\n" + lastlineindent + newstatementstr);

                                cursor.endEditBlock();

                                // then make a new line from the end our replaced text
                                // with the same indentation
                                cursor.movePosition(QTextCursor::NextBlock);

                                cursor.movePosition(QTextCursor::EndOfBlock);

                                cursor.insertText("\n" + lastlineindent);
                                break;
                            }
                            else if (codeblockcount > 0 & opentest)
                            {
                                // We have found the opening statement of the last
                                // nested codeblock, so decrement the codeblock counter
                                codeblockcount--;
                            }

                            // decrement the line number
                            linenumber--;

                        } // while ( linenumber != -1 )

                        if (linenumber == -1)
                        {
                            // a match to the end statement could not be found, so put the
                            // cursor back in the original position and just go to the next
                            // line with the current indentation
                            cursor.setPosition(cursorPos);
                            cursor.insertText("\n" + lastlineindent);
                        }

                    }
                    else
                    {
                        // insert a new line character and the spacing from the start of the
                        // previous line
                        cursor.insertText("\n" + lastlineindent);
                    }

                } // if MFILE
                else if (thisFileType == SimpleEditor::TXTFILE)
                {
                    cursor.insertText("\n");
                }
                else
                {
                    // insert a new line character and the spacing from the start of the
                    // previous line
                    cursor.insertText("\n" + lastlineindent);
                }

            } //if( automatic_indention_statement_ok )
            else
            {
                if (thisFileType == SimpleEditor::TXTFILE)
                {
                    // just insert a new line
                    cursor.insertText("\n");
                }
                else
                {
                    // insert a new line character and the spacing from the start of the
                    // previous line
                    cursor.insertText("\n" + lastlineindent);
                }
            }

            setTextCursor(cursor);

        }
        else
            QPlainTextEdit::keyPressEvent(e);
    }
    //else if( e->key()==(Qt::Key_B) && Qt::ControlModifier==e->modifiers() )
    //{
    //	autoComplete();
    //	return;
    //}
    else if( e->key() == Qt::Key_Tab )
    {
        // user pressed the TAB key

        QTextCursor cursor = textCursor();

        // check if the user has selected any text
        int start = cursor.selectionStart();
        int end = cursor.selectionEnd();

        if(start == end)
        {
            // if they have not selected any text, insert
            // whatever the current tab character insertion
            // string is and return
            cursor.insertText(tabInsert);
            //QPlainTextEdit::keyPressEvent(e);
            return;
        }

        // if the user has selected text, we will insert the tab
        // indent string before the code text on every selected line

        // signal the start of a block editing operation
        cursor.beginEditBlock();
        // move the cursor to the end of the selected text and then
        // get the block number of the last block in the selected
        // region
        cursor.setPosition(end);
        end = cursor.blockNumber();
        // move the cursor to the start of the selected text and then
        // move the cursor to the start of the first line/block in the
        // selected region
        cursor.setPosition(start);
        cursor.movePosition(QTextCursor::StartOfBlock);
        // now loop through the selected blocks adding the tab insertion
        // string to the start of every block until we reach the block
        // number stored previously in 'end'
        while(true)
        {
            cursor.insertText(tabInsert);

            if(cursor.blockNumber() >= end)
                break;

            cursor.movePosition(QTextCursor::NextBlock);
        }
        // end the editing operation
        cursor.endEditBlock();
    }
    else if(e->key() == Qt::Key_Backtab )
    {
        // User pressed SHIFT+TAB

        // Get the cursor and the start and end of the current selection
        QTextCursor cursor = textCursor();

        int start = cursor.selectionStart();

        int end = cursor.selectionEnd();

        if(start == end)
        {
            // If there is no selection, handle the key press using the
            // normal QPlainTextEdit::keyPressEvent method
            QPlainTextEdit::keyPressEvent(e);
        }
        else
        {
            // If there is a selection, we reduce the indentation on each line
            // which is part of the selected region
            cursor.beginEditBlock();

            cursor.setPosition(end);

            end = cursor.blockNumber();

            cursor.setPosition(start);

            cursor.movePosition(QTextCursor::StartOfBlock);

            while( true )
            {
                QString line = cursor.block().text();

                if( line.length() > 0 && (line[0] == ' ' || line[0] == '\t') )
                {
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

                    cursor.removeSelectedText();
                }

                if(cursor.blockNumber() >= end)
                    break;

                cursor.movePosition(QTextCursor::NextBlock);

                cursor.movePosition(QTextCursor::StartOfBlock);
            }

            cursor.endEditBlock();
        }
    }
//    else if( (e->key() == Qt::Key_Equal) && (Qt::ControlModifier == e->modifiers()) )
//    {
//        int font_size = this->font().pointSize();

//        this->setFontSize(font_size + 2);
//    }
    else if( (e->key() == Qt::Key_Minus) && (Qt::ControlModifier == e->modifiers()) )
    {
        int font_size = this->font().pointSize();

        this->setFontSize(font_size - 2);
    }
    else
    {
        if( (e->key() == Qt::Key_B) && (Qt::ControlModifier == e->modifiers()) )
        {
            autoComplete(0);
            return;
        }

        QPlainTextEdit::keyPressEvent(e);

    }
}

void SimpleEditor::setCharFormat(QTextCharFormat charFormat)
{
	this->charFormat=charFormat;
	QTextCursor cursor=textCursor();
	cursor.movePosition(QTextCursor::Start);
	cursor.setCharFormat(charFormat);
	cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
	setFont(charFormat.font());
	
	QFontMetrics fm(charFormat.font());
	int textWidthInPixels = fm.width("        ");
	setTabStopWidth(textWidthInPixels);
}

void SimpleEditor::activated( const QString & text )
{
    QAbstractItemView *view = completer->popup();

    QTextCursor cursor = textCursor();

    // Select the word currently being typed
    cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);

    // Insert the word from the auto-completion popup
	cursor.insertText(text);

    // Close the popup
	view->hide();
}

void SimpleEditor::autoComplete(int position, int charsRemoved, int charsAdded)
{
	//printf("[SimpleEditor::autoComplete] charsAdded=%d\n", charsAdded);
    if(charsAdded == 1)
		autoComplete();
}

void SimpleEditor::autoComplete(int size)
{
	QTextCursor cursor=textCursor();
	
	cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
	
	//printf("[SimpleEditor::autoComplete] >%s<\n", cursor.selectedText().toLocal8Bit().data());
	
	if( cursor.selectedText().endsWith(" ") || cursor.selectedText().trimmed().length()<size ) return;
	
	QStringList list=toPlainText().split(QRegExp("\\W+"));
	
	list.removeDuplicates();
	list.removeOne(cursor.selectedText());
	list.sort();
	
	list.append(comands_completion_list);
	
	completerModel->setStringList( list );
	
	completer->setCompletionPrefix(cursor.selectedText());
	
	//printf("[SimpleEditor::autoComplete] >%d<\n", completer->completionCount());
	
	if (completer->completionCount()>0 )
	{
		//completer->setWidget(this);
		QRect r=cursorRect(cursor);
		r.setWidth(200);
		completer->complete(r);
	}
}


QString SimpleEditor::getFileName()
{
	return fileName;
}


void SimpleEditor::setFile(QString file)
{
	fileName=file;
	loadSyntaxXMLDescription();
}

void SimpleEditor::setTabInsertStr(QString tabstr)
{
    tabInsert = tabstr;
}

void SimpleEditor::setFontSize(int fontsize)
{
    // Check the requested font size is not larger or
    // smaller than allowed
    if (fontsize < minFontSize)
        fontsize = minFontSize;

    if (fontsize > maxFontSize)
        fontsize = maxFontSize;

    // Then change the font size
    QFont thefont = this->font();

    thefont.setPointSize(fontsize);

    this->setFont(thefont);
}

int SimpleEditor::getFontSize()
{
    return this->font().pointSize();
}

void SimpleEditor::cursorPositionChangedCallBack()
{
    // Hightlight brackets
    if(syntax != NULL)
		syntax->setFormatPairBrackets(this);
}

void SimpleEditor::publicBlockBoundingRectList(QVector<qreal> &list, int &first_line)
{
	qreal pageBottom = /*viewport()->*/height();

    // get the offset of the content from the top left
    // corner of the document
    QPointF offset = contentOffset();

    QTextBlock block = firstVisibleBlock();

    first_line = block.blockNumber() + 1;

    qreal first_position = blockBoundingGeometry(block).topLeft().y();
	
	for ( ; block.isValid(); block = block.next() )
	{
        QRectF position = blockBoundingGeometry(block);

        qreal y = position.topLeft().y() + offset.y() - first_position;
		
        if( y > pageBottom )
            break;
		
		list.append(y);
	}
}


/**Checks if document has references. If not document is deleted.
 */
static void unreferenceDocument(QTextDocument * document)
{
	printf("[SimpleEditor::unreferenceDocument] Entered\n");
	QVariant n_refs = document->property("DocumentReferences");
	if(n_refs.isValid())
	{
		int n=n_refs.toInt()-1;
		printf("[unreferenceDocument] References: %d\n", n);
		if(n<=0)
			delete document;
		else
			document->setProperty("DocumentReferences", QVariant(n));
	}
}

void SimpleEditor::setDocument ( QTextDocument * document )
{
	printf("[SimpleEditor::setDocument] Entered\n");
	if(document!=NULL)
	{
		QVariant n_refs = document->property("DocumentReferences");
		if(n_refs.isValid())
		{
			document->setProperty("DocumentReferences", QVariant(n_refs.toInt()+1));
			int n=n_refs.toInt()+1;
			printf("[SimpleEditor::setDocument] References: %d\n", n);
		}
	}
	
	QTextDocument * oldDocument=this->document();
	QVariant n_refs = oldDocument->property("DocumentReferences");
		
	//if(document!=NULL)
		QPlainTextEdit::setDocument(document);
		
	if(n_refs.isValid())
		unreferenceDocument(oldDocument);
}

QString SimpleEditor::getTextLine( int lineno )
{
    QString textLine = "";

    if (lineno > this->document()->blockCount() || lineno < 1)
    {
        textLine = "";
    }
    else
    {
        QTextBlock tempTextBlock = this->document()->findBlockByNumber(lineno - 1);

        textLine = tempTextBlock.text();
    }

    return textLine;
}

SimpleEditor::FileType SimpleEditor::getFileType()
{
    return thisFileType;
}

SimpleEditor::~SimpleEditor()
{
	printf("[SimpleEditor::~SimpleEditor] Entered\n");
	QTextDocument * oldDocument=this->document();
	unreferenceDocument(oldDocument);
}

