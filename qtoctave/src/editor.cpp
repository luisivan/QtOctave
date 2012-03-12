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

#include <QBoxLayout>
#include <QToolBar>
#include <QIcon>
#include <QMdiArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCursor>
#include <QMenuBar>
#include <QPrinter>
#include <QPrintDialog>
#include <QClipboard>
#include "editor.h"
#include "config.h"
#include "search_dialog.h"
#include "numberedtextview.h"
#include "navigator.h"


// Constructor
Editor::Editor(QWidget *parent): BaseWidget(parent)
{
    widget_type = EDITOR;

    currentNtv = NULL;
    search_dialog = NULL;

	// Title
	setWindowTitle(tr("Editor"));
    setWindowIcon(QIcon(QString(ICON_PATH) + QDir::separator() + "xedit.png"));

	// Layout
	QVBoxLayout *vLayout = new QVBoxLayout;
	centralWidget()->setLayout(vLayout);
	vLayout->setSpacing (0);

    // Font Size
    minFontSize = 6;
    maxFontSize = 36;

    QString font_size_str = get_config("textEditFontSize");

    if(font_size_str.isEmpty())
    {
        font_size = 12;
    }
    else
    {
        font_size = font_size_str.toInt();
    }

    // Set the size of the editor widget
    resize(1000, 800);

	// Toolbar
	toolBar = addToolBar(tr("Options"));

	toolBar->setObjectName(tr("Editor Options"));

    // Make the toolbar icons smaller
    toolBar->setIconSize(QSize(16,16));

	//toolBar->setOrientation(Qt::Horizontal);
	//toolBar->setMinimumHeight(40);
	//toolBar->setMaximumHeight(40);
	connect(toolBar, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(toolbar_action(QAction*)));
	//vLayout->addWidget(toolBar);
	//toolBar->show();

	// Toolbar buttons

    // New file button
	actionNew = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "filenew.png"), tr("New"));
	actionNew->setShortcut(tr("Ctrl+N"));
	actionNew->setShortcutContext(Qt::WindowShortcut);

    // Open file button
	actionOpen = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "fileopen"), tr("Open"));
	actionOpen->setShortcut(tr("Ctrl+O"));
	actionOpen->setShortcutContext(Qt::WindowShortcut);

    // Save current file button
	actionSave = toolBar->addAction(QIcon(QString(ICON_PATH)  +QDir::separator()+ "filesave"), tr("Save"));
	actionSave->setShortcut(tr("Ctrl+S"));
	actionSave->setShortcutContext(Qt::WindowShortcut);

    // Save As button
	actionSaveAs = toolBar->addAction(QIcon(QString(ICON_PATH)  +QDir::separator()+ "filesaveas.png"), tr("Save as"));

    // Close file button
	actionClose = toolBar->addAction(QIcon(QString(ICON_PATH)  +QDir::separator()+ "fileclose.png"), tr("Close tab"));

	toolBar->addSeparator();

    // Run file button
	actionRun = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "run.png"), tr("Run"));
	actionRun->setShortcut(tr("F5"));
	actionRun->setShortcutContext(Qt::WindowShortcut);

    // Start debugging button
	actionDebug = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "find.png"), tr("Debug"));
	actionDebug->setToolTip(
            "<html>"
            "<b>Debug:</b>"
            "<p>Octave includes a built-in debugger to aid in the development of scripts. This can be used to interrupt the execution of an Octave script at a certain point.</p>"
            "<p>Click over this button to start debugging or continue to next breakpoint.</p>"
            "</html>"
            );
	actionDebug->setShortcut(tr("F6"));

    // Debug step button, icon created by Yusuke Kamiyamane and supplied with Creative
    // Commons Licence 3.0 http://creativecommons.org/licenses/by/3.0/
    //actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "step.png"), tr("Detailed debugging"));
    actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "arrow-step-over-32.png"), tr("Step"));
    actionStep->setShortcut(tr("F10"));
	actionStep->setEnabled(false);

    // Debug step in button, icon created by Yusuke Kamiyamane and supplied with Creative
    // Commons Licence 3.0 http://creativecommons.org/licenses/by/3.0/
    //actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "step.png"), tr("Detailed debugging"));
    actionStepIn = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "arrow-step-in-32.png"), tr("Step In"));
    actionStepIn->setShortcut(tr("F11"));
    actionStepIn->setEnabled(false);

    // Debug step out button, icon created by Yusuke Kamiyamane and supplied with Creative
    // Commons Licence 3.0 http://creativecommons.org/licenses/by/3.0/
    //actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "step.png"), tr("Detailed debugging"));
    actionStepOut = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "arrow-step-out-32.png"), tr("Step Out"));
    actionStepOut->setShortcut(tr("Shift+F11"));
    actionStepOut->setEnabled(false);

    // Debug continue button, icon created by Yusuke Kamiyamane and supplied with Creative
    // Commons Licence 3.0 http://creativecommons.org/licenses/by/3.0/
    //actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "step.png"), tr("Detailed debugging"));
    actionContinue = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "debug-continue-32.png"), tr("Continue"));
    actionContinue->setShortcut(tr("Shift+F5"));
    actionContinue->setEnabled(false);

    // Debug quit button, icon created by Yusuke Kamiyamane and supplied with Creative
    // Commons Licence 3.0 http://creativecommons.org/licenses/by/3.0/
    //actionStep = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "step.png"), tr("Detailed debugging"));
    actionDebugQuit = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator()+ "debug-stop-square-32.png"), tr("Quit Debugging"));
    actionDebugQuit->setShortcut(tr("Shift+F6"));
    actionDebugQuit->setEnabled(false);

    // Send selected text to octave button
    actionSendToOctave = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator() + "konsole.png"), tr("Send to Octave") );
	actionSendToOctave->setShortcut(tr("F9"));
	actionSendToOctave->setShortcutContext(Qt::WindowShortcut);
	actionSendToOctave->setToolTip(
            "<html>"
            "<b>Send to Octave:</b>"
            "<p>Sends selected text to Octave. If there is not selected text, the full text will be sent.</p>"
            "</html>"
            );

	toolBar->addSeparator();

	actionUndo = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "undo.png"), tr("Undo"));
	//actionUndo->setShortcut(tr("Ctrl+Z"));
	//actionUndo->setShortcutContext(Qt::WindowShortcut);

	actionRedo = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "redo.png"), tr("Redo"));
	//actionRedo->setShortcut(tr("Ctrl+Shift+Z"));
	//actionRedo->setShortcutContext(Qt::WindowShortcut);

	actionCut = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "editcut"), tr("Cut"));
	//actionCut->setShortcut(tr("Ctrl+X"));
	//actionCut->setShortcutContext(Qt::WindowShortcut);

	actionCopy = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "editcopy"), tr("Copy"));
	//actionCopy->setShortcut(tr("Ctrl+C"));
	//actionCopy->setShortcutContext(Qt::WindowShortcut);

	actionPaste = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "editpaste"),  tr("Paste"));
	//actionPaste->setShortcut(tr("Ctrl+V"));
	//actionPaste->setShortcutContext(Qt::WindowShortcut);

	toolBar->addSeparator();

    actionSearch = toolBar->addAction(QIcon(QString(ICON_PATH) + QDir::separator() + "search.png"),  tr("Search and replace"));
	actionSearch->setShortcut(tr("Ctrl+F"));
	actionSearch->setShortcutContext(Qt::WindowShortcut);

    toolBar->addSeparator();

    // Increase editor text size
    actionZoomIn = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "edit-size-up.png"),  tr("Increase Text Size"));
    actionZoomIn->setShortcut(tr("Ctrl+="));

    // Reduce Editor text size
    actionZoomOut = toolBar->addAction(QIcon(QString(ICON_PATH) +QDir::separator()+ "edit-size-down.png"),  tr("Reduce Text Size"));
    actionZoomOut->setShortcut(tr("Ctrl+-"));

	//Create menus

    menuFile = menuBar()->addMenu(tr("File"));
	menuFile->addAction(actionNew);
	menuFile->addAction(actionOpen);
	menuFile->addAction(actionSave);
	menuFile->addAction(actionSaveAs);
    actionClone = menuFile->addAction(tr("Clone View"));
	connect(actionClone, SIGNAL(triggered()),this, SLOT(clone_callback()));

	menuFile->addSeparator();
    actionPrint = menuFile->addAction(tr("Print"));
	connect(actionPrint, SIGNAL(triggered()),this, SLOT(print_callback()));
	menuFile->addSeparator();
	menuFile->addAction(actionClose);
	menuFile->addSeparator();
	QAction *actionCloseAll=menuFile->addAction(tr("Close"));
	connect(actionCloseAll, SIGNAL(triggered()),this, SLOT(close_editor()));
	//connect(menuFile, SIGNAL(triggered(QAction*)),this, SLOT(toolbar_action(QAction*)));

    menuEdit = menuBar()->addMenu(tr("Edit"));
	/*
	menuEdit->addAction(actionUndo);
	menuEdit->addAction(actionRedo);
	menuEdit->addAction(actionCut);
	menuEdit->addAction(actionCopy);
	menuEdit->addAction(actionPaste);
	menuEdit->addAction(actionSearch);
	*/
	connect(menuEdit, SIGNAL(aboutToShow()), this, SLOT(show_edit_menu()));
	
	
	//connect(menuEdit, SIGNAL(triggered(QAction*)),this, SLOT(toolbar_action(QAction*)));

    menuRun = menuBar()->addMenu(tr("Run"));
	menuRun->addAction(actionRun);
	menuRun->addAction(actionSendToOctave);
	menuRun->addAction(actionDebug);
	menuRun->addAction(actionStep);
    menuRun->addAction(actionStepIn);
    menuRun->addAction(actionStepOut);
    menuRun->addAction(actionContinue);
    menuRun->addAction(actionDebugQuit);
	//connect(menuRun, SIGNAL(triggered(QAction*)),this, SLOT(toolbar_action(QAction*)));
	menuRun->addSeparator();

    // Break point toggle
    actionToggleBreakPoint = menuRun->addAction(tr("Toggle breakpoint"));
	actionToggleBreakPoint->setShortcut(tr("F7"));
	connect(actionToggleBreakPoint, SIGNAL(triggered()),this, SLOT(toggleBreakPoint_callback()));

    menuTools = menuBar()->addMenu(tr("Tools"));

    actionIndent = menuTools->addAction(tr("Indent"));
	connect(actionIndent, SIGNAL(triggered()),this, SLOT(indent_callback()));

    actionUnindent = menuTools->addAction(tr("Unindent"));
	connect(actionUnindent, SIGNAL(triggered()),this, SLOT(unindent_callback()));
	menuTools->addSeparator();

    actionComment = menuTools->addAction(tr("Comment"));
	connect(actionComment, SIGNAL(triggered()),this, SLOT(comment_callback()));
	actionComment->setShortcut(tr("Ctrl+r"));

    actionUncomment = menuTools->addAction(tr("Uncomment"));
	connect(actionUncomment, SIGNAL(triggered()),this, SLOT(uncomment_callback()));
	actionUncomment->setShortcut(tr("Ctrl+Shift+r"));

    if(get_config("simple_rcs") == "true")
	{
		menuTools->addSeparator();
        actionSimpleRCS = menuTools->addAction("Revision control system (SimpleRCS)");
		connect(actionSimpleRCS, SIGNAL(triggered()),this, SLOT(simple_rcs_callback()));
	}
	
    menuConfig = menuBar()->addMenu(tr("Config"));


	// TabWidget
	tabWidget = new QTabWidget(this);
	tabWidget->show();

	connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));

	vLayout->addWidget(tabWidget);

	//List of files
    QDockWidget *list_files_dock = new QDockWidget(tr("File list"), this);
	list_files_dock->setObjectName(list_files_dock->windowTitle());
	addDockWidget(Qt::LeftDockWidgetArea, list_files_dock);
	//list_files_dock->show();

    list_files = new QListView(list_files_dock);
	list_files_dock->setWidget(list_files);
	list_files_dock->show();
	list_files->setAcceptDrops(true);
	list_files->setDropIndicatorShown(true);
	list_files->setDragEnabled(true);
	list_files->viewport()->setAcceptDrops(true);
	list_files->setModel(new ListModel(0, this));
	connect(list_files,SIGNAL(clicked (const QModelIndex &)), this, SLOT(file_selected(const QModelIndex &)) );

	//Clipboard list
    QDockWidget *list_clipboard_dock = new QDockWidget(tr("Small Clipboard"), this);
	list_clipboard_dock->setObjectName(list_clipboard_dock->windowTitle());
	addDockWidget(Qt::LeftDockWidgetArea, list_clipboard_dock);

    list_clipboard = new ClipboardListView(list_clipboard_dock);
	list_clipboard_dock->setWidget(list_clipboard);
	list_clipboard_dock->show();

	connect(list_clipboard,SIGNAL(doubleClicked (const QModelIndex &)), this, SLOT(clipboard_double_clicked(const QModelIndex &)) );

	//This menu is builded here because it show objects if they hav been created
    menuDocks = createPopupMenu();
	menuDocks->setTitle(tr("Show/Hide Objects"));
	menuConfig->addMenu(menuDocks);

	//setAcceptDrops(true);

    project_name = tr("Empty");

	toolbar_action(actionNew);
}

Editor::~Editor()
{
	saveProject();

    if(search_dialog != NULL)
        delete search_dialog;
}

void Editor::show_edit_menu()
{
	menuEdit->clear();

    if(currentNtv != NULL)
		menuEdit->addMenu(currentNtv->textEdit()->createStandardContextMenu());

	menuEdit->addAction(actionSearch);

    menuEdit->addAction(actionZoomIn);
    menuEdit->addAction(actionZoomOut);
}

void Editor::saveProject()
{
	QStringList files;

    for(int i=0; i<tabWidget->count(); i++)
	{
        QString path = ((NumberedTextView*)tabWidget->widget(i) )->path();

        if(!path.isEmpty())
            files.append( path );
	}

	files.removeDuplicates();

    if( project_name.isEmpty() )
    {
        Projects::saveListFiles(tr("Empty"), files);
    }
    else
    {
        Projects::saveListFiles(project_name, files);
    }

}


// void Editor::dragEnterEvent(QDragEnterEvent *event)
// {
// 	if (event->mimeData()->hasFormat("text/plain"))
// 		event->acceptProposedAction();
// }
//
//
// void Editor::dropEvent ( QDropEvent * event )
// {
// 	QString path=event->mimeData()->text();
//
// 	openFile(path);
//
// 	event->acceptProposedAction();
// }


void Editor::setOctaveConnection(OctaveConnection *oc)
{
    octave_connection = oc;

    for(int i=0; i < tabWidget->count(); i++)
    {
        // get the path of the file in the current tab
         NumberedTextView *ntv = ((NumberedTextView*)tabWidget->widget(i));

         ntv->setOctaveConnection(oc);
    }

}


void Editor::toolbar_action(QAction *action)
{
    QStringList filters;

    filters << "Octave (*.m; *.M)"
            << "Plain text (*.txt)"
            << "All files (*)";


    if(action == actionNew)
    {
        /** New File **/

        this->newEditorTab();

//        SimpleEditor *codeEdit = new SimpleEditor(NULL);

//        connect(codeEdit, SIGNAL(dynamic_help_required(const QString &)), this, SLOT(emit_dynamic_help_required(const QString &)));

//        NumberedTextView *ntv = new NumberedTextView(0, codeEdit, octave_connection);

//        connect(ntv->textEdit(), SIGNAL(toggleBreakpoint(int)), this, SLOT(toggleBreakpoint(int)));

//        //connect(ntv, SIGNAL(textModified()), this, SLOT(textModified()));
//        connect(codeEdit->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));

//        currentNtv = ntv;

//        tabWidget->setCurrentIndex( tabWidget->addTab(ntv, tr("New")) );

    }
    else if( action == actionOpen )
    {
        /** Open **/
        openFile();
    }
    else if( action == actionSave && !currentNtv->path().isEmpty() )
    {
        /** Save **/
        if( currentNtv->save() )
            setTabText(tabWidget->currentIndex(), currentNtv->path().split("/").last());
        else
            QMessageBox::critical(NULL, tr("Error"), tr("Can not be saved"));

    }
    else if( action == actionSaveAs || (action == actionSave && currentNtv->path().isEmpty()) )
    {
        /** Save as **/
        QString path;

        QFileDialog saveDialog(this, Qt::Dialog);

        saveDialog.setAcceptMode(QFileDialog::AcceptSave);

        saveDialog.setDefaultSuffix("m");

        saveDialog.setFilters(filters);

        //Use Navigator path if current path is empty
        if( currentNtv->path().isEmpty() )
        {
            QObject *obj = session->getFirstTool(NAVIGATOR);

            if(obj != NULL)
            {
                Navigator *nav=(Navigator*)obj;

                saveDialog.setDirectory(nav->getNavigatorCurrentPath());
            }
        }
        else
        {
            QFileInfo current_file(currentNtv->path());

            saveDialog.setDirectory(current_file.absolutePath());

            saveDialog.selectFile(current_file.baseName());
        }

        if(saveDialog.exec() == QDialog::Accepted)
        {
            path = saveDialog.selectedFiles().first();

            if(currentNtv->save(path))
            {
                setTabText(tabWidget->currentIndex(), currentNtv->path().split("/").last());
            }
            else
            {
                QMessageBox::critical(NULL, tr("Error"), path + tr("can not be saved"));
            }
        }
    }
    else if(action == actionRun)
    {
        /** Run file without debugging or saving */

        //if(currentNtv->path().isEmpty())
        //{
        //	QMessageBox::critical(NULL, tr("Error"), tr("You must save the file first"));
        //	return;
        //}

        if( currentNtv->modified() )
            toolbar_action(actionSave);

        QFileInfo finfo(currentNtv->path());

        octave_connection->command_enter(QString("cd '") + finfo.path() + "'");

        octave_connection->command_enter(QString("source (\"") +  finfo.fileName() + "\")");

    }
    else if( action == actionDebug )
    {
        /** Run with debugging */

        if( currentNtv->path().isEmpty() )
        {
            QMessageBox::critical(NULL, tr("Error"), tr("You must save the file first"));
            return;
        }

        // Debug?
        if( actionStep->isEnabled() )
        {
            //octave_connection->command_enter(QString("dbcont"));
            octave_connection->debugContinue();
        }
        else
        {
            QFileInfo finfo(currentNtv->path());

            QList<int> *breakpoints = currentNtv->getBreakpoints();

            if( breakpoints != NULL )
            {
                // Source
                //octave_connection->command_enter(QString("source('") + finfo.absoluteFilePath() + "')");

                // Clear breakpoints
                //octave_connection->command_enter(QString("dbclear('") + finfo.baseName() + "',dbstatus('"+finfo.baseName()+"') )");

                //Change to dir
                octave_connection->command_enter(QString("cd '") + finfo.path() + "'", false);

                // clear all current break points
                octave_connection->command_enter(QString(
                        "while (  length (dbstatus('" + finfo.baseName() + "')) >0  )"
                        "dbclear('" + finfo.baseName() + "', dbstatus('" + finfo.baseName() + "')(1).line);"
                        "endwhile"
                        ), false);

                // Insert breakpoints
                for(QList<int>::const_iterator i = breakpoints->constBegin(); i != breakpoints->constEnd(); i++)
                {
                    octave_connection->command_enter(QString("dbstop('")
                                                     + finfo.baseName()
                                                     + "','" + QString::number(*i) + "')", false);
                }

                // Connect debug and endDebug signals
                connect(octave_connection, SIGNAL(debug(int, int, QString)), this, SLOT(debug(int, int, QString)));

                connect(octave_connection, SIGNAL(endDebug()), this, SLOT(endDebug()));

                // Run the file
                octave_connection->command_enter(finfo.baseName());
            }
        } // End debug?
    }
    else if(action == actionUndo)
    {
        // Undo
        ((SimpleEditor*)currentNtv->textEdit())->undo();
    }
    else if(action == actionRedo)
    {
        // Redo
        currentNtv->textEdit()->document()->redo();
    }
    else if(action == actionCut)
    {
        // Cut
        currentNtv->textEdit()->cut();
    }
    else if(action == actionCopy)
    {
        // Copy
        currentNtv->textEdit()->copy();
    }
    else if(action == actionPaste){
        // Paste
        currentNtv->textEdit()->paste();
    }
    else if(action == actionSearch)
    {
        // Search
        if( search_dialog == NULL )
        {
            search_dialog = new SearchDialog(this);

            connect(search_dialog, SIGNAL(search_signal()), this, SLOT(search()));

            connect(search_dialog, SIGNAL(replace_signal()), this, SLOT(replace()));
        }

        search_dialog->show();
    }
    else if( action == actionSendToOctave )
    {
        QTextCursor cursor = currentNtv->textEdit()->textCursor();

        if( cursor.hasSelection() )
        {
            // Send the command to octave, replacing the paragraph separator characters
            // with newline characters
            octave_connection->command_enter(cursor.selectedText().replace(QChar(QChar::ParagraphSeparator), "\n"));
        }
        else
        {
            octave_connection->command_enter( currentNtv->textEdit()->document()->toPlainText() );
        }
    }
    else if(action == actionStep)
    {
        octave_connection->debugStep();
    }
    else if(action == actionStepIn)
    {
        octave_connection->debugStepIn();
    }
    else if(action == actionStepOut)
    {
        octave_connection->debugStepOut();
    }
    else if(action == actionContinue)
    {
        octave_connection->debugContinue();
    }
    else if(action == actionDebugQuit)
    {
        octave_connection->debugQuit();
    }
    else if(action == actionClose)
    {
        closeTabs(false);
    }
    else if(action == actionZoomIn)
    {
        font_size = font_size + 2;

        this->setFontSize(font_size);
    }
    else if(action == actionZoomOut)
    {
        font_size = font_size - 2;

        this->setFontSize(font_size);
    }
    else
    {
        printf("Unhandled action\n");
    }
}


void Editor::openFile(QString file)
{
	/** Open **/
	QString path;

	if(file.isEmpty())
	{
		QFileDialog openDialog(this, tr("Open") /*Qt::Dialog*/);

		QStringList filters;

		filters << "Octave (*.m; *.M)"
                << "Plain text (*.txt)"
                << "All files (*)";

		openDialog.setAcceptMode(QFileDialog::AcceptOpen);

		openDialog.setDefaultSuffix("m");

		openDialog.setFilters(filters);

		//openDialog.setViewMode(QFileDialog::Detail);
		QFileInfo current_file(currentNtv->path());

		openDialog.setDirectory(current_file.absolutePath());

		openDialog.selectFile(current_file.baseName());

		if(openDialog.exec() == QDialog::Accepted)
			path = openDialog.selectedFiles().first();
        else
            return;
	}
	else
        path = file;

	loadFiles(QStringList() << path);
}

void Editor::setProject(QString name)
{
	project_name=name;
	closeTabs(true);
	QStringList files=Projects::listFiles(project_name);
	loadFiles(files);
}

QString Editor::getProject()
{
	return project_name;
}

void Editor::setSession(Session *session)
{
	BaseWidget::setSession(session);
	setProject(session->getProjectName());
	connect(session, SIGNAL(projectChanged(QString)), this, SLOT(setProject(QString)) );
}

void Editor::search()
{
    QString search, replace;
    QTextCursor cursor;
    QPlainTextEdit *textEdit = currentNtv->textEdit();

    // Strings
    search = search_dialog->searchString();
    replace = search_dialog->replaceString();

    // Flags
    QTextDocument::FindFlags flags;
    if(search_dialog->caseSensitive())
        flags |= QTextDocument::FindCaseSensitively;
    if(search_dialog->wholeWords())
        flags |= QTextDocument::FindWholeWords;

    // Search
    cursor = textEdit->textCursor();
    if(search_dialog->searchStringIsRegExp())
    {
        // Search string is a regular expression
        QRegExp searchReg(search);

        cursor = textEdit->document()->find(searchReg, cursor, flags);
        //cursor = textEdit->document()->find(search, cursor, flags);
    }else{
        // Search string is not a regular expression
        cursor = textEdit->document()->find(search, cursor, flags);
    }

    textEdit->setTextCursor(cursor);
}

void Editor::replace()
{
	QTextCursor cursor = currentNtv->textEdit()->textCursor();

	if(!cursor.selectedText().isEmpty())
	{
		int pos=cursor.position();
		cursor.insertText(search_dialog->replaceString());
		cursor.setPosition(pos);
		currentNtv->textEdit()->setTextCursor(cursor);
	}

	//Next line is comented because editor loose cursor
	//search();
}

void Editor::toggleBreakpoint(int lineno)
{
    currentNtv->toggleBreakpoint(lineno);
}

void Editor::debug(int lineno, int colno, QString filestr)
{
    bool isfileopen = false;

    // clear the line indicator in case we change files
    // through stepping in for instance
    currentNtv->setCurrentLine(-1);

    if (!filestr.isEmpty())
    {
        QFileInfo filestrFileInfo(filestr);

        for(int i=0; i < tabWidget->count(); i++)
        {
            // get the path of the file in the current tab
            QString tabPath = ((NumberedTextView*)tabWidget->widget(i))->path();

            // get the file informaion from this path
            QFileInfo tabFileInfo(tabPath);

            // If the file path is the same as the file being debugged
            // switch to this tab
            if ( !QString::compare(tabFileInfo.absoluteFilePath(), filestrFileInfo.absoluteFilePath()))
            {
                // Open the correct tab
                tabWidget->setCurrentIndex(i);

                // Set the current numbered text view to the one containing the
                // correct file
                currentNtv = (NumberedTextView*)tabWidget->widget(i);

                // not that we have located the current file
                isfileopen = true;
            }
        }

        // If we couldn't find the current file in the existing tabs
        // open it in a new tab and proceed with debugging in this
        // file
        if (!isfileopen)
        {
            QStringList fileList;

            fileList += filestr;

            loadFiles(fileList);
        }
    }

    // Set the current debug line and enable debugging actions
    currentNtv->setCurrentLine(lineno);
    actionStep->setEnabled(true);
    actionStepIn->setEnabled(true);
    actionStepOut->setEnabled(true);
    actionContinue->setEnabled(true);
    actionDebugQuit->setEnabled(true);
}

void Editor::endDebug()
{
    currentNtv->setCurrentLine(-1);
    actionStep->setEnabled(false);
    actionStepIn->setEnabled(false);
    actionStepOut->setEnabled(false);
    actionContinue->setEnabled(false);
    actionDebugQuit->setEnabled(false);

    //Clean break points
    QFileInfo finfo(currentNtv->path());

    octave_connection->command_enter(QString(
            "while (  length (dbstatus('" + finfo.baseName() + "')) >0  )"
            "dbclear('" + finfo.baseName() + "', dbstatus('" + finfo.baseName() + "')(1).line);"
            "endwhile"
            ), false );
}

void Editor::tabChanged(int index)
{
	//printf("Activado %d\n", index);
	//if(currentNtv!=NULL)
	//	disconnect(currentNtv->textEdit(), SIGNAL(toggleBreakpoint(int)), this, SLOT(toggleBreakpoint(int)));
	currentNtv = (NumberedTextView*)tabWidget->widget(index);
	//setWindowTitle(tabWidget->tabText(index));
	//connect(currentNtv->textEdit(), SIGNAL(toggleBreakpoint(int)), this, SLOT(toggleBreakpoint(int)));
	ListModel *list=(ListModel*)list_files->model();
	list_files->setCurrentIndex(list->position_index(index));
}

void Editor::textModified(bool ok)
{
	for(int i=0;i<tabWidget->count(); i++)
	{
		NumberedTextView *ntv=(NumberedTextView *)tabWidget->widget(i);
		if( ntv==NULL ) continue;
		QPlainTextEdit *text=ntv->textEdit();
		
		if(ntv->path().isEmpty())
		{
			if(!text->document()->isModified()) 
				setTabText(i, tr("New"));
			else
				setTabText(i, tr("New")+"*");
		}
		else
		{
			if(!text->document()->isModified()) 
				setTabText(i, ntv->path().split("/").last());
			else
				setTabText(i, ntv->path().split("/").last()+"*");
		}
	}
}

/////////////////////////////////////////////////////////////

void Editor::closeEvent ( QCloseEvent * event )
{
	bool modified=false;
	for(int i=0;i<tabWidget->count();i++)
	{
		modified|=( (NumberedTextView*)tabWidget->widget(i) )->modified();
	}
	int ok;
	if(modified)
		ok=QMessageBox::warning (this,tr("Close this window?"), tr("You are going to close Editor. Are you sure?"), QMessageBox::Ok, QMessageBox::Cancel);
	else
		ok=QMessageBox::Ok;

	if (ok==QMessageBox::Ok)
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

/////////////////////////////////////////////////////////////

void Editor::closeTabs(bool close_all_tabs)
{
	while(tabWidget->count()>0)
	{
		if(currentNtv==NULL)
		{
			printf("[Editor::closeTabs] currentNtv==NULL\n");
			break;
		}
		if(
                (
                        currentNtv->modified() &&
                        !currentNtv->textEdit()->toPlainText().isEmpty() &&
                        currentNtv->path().isEmpty()
                        )
                ||
                (
                        currentNtv->modified() && !currentNtv->path().isEmpty()
                        )
                )
		{
			QMessageBox msg(tr("Close"), tr("The file has been modified. Save changes?"),
                            QMessageBox::Question,
                            QMessageBox::Yes, QMessageBox::No,
                            QMessageBox::Cancel | QMessageBox::Default,
                            this);

			switch(msg.exec())
			{
            case QMessageBox::Yes:
                toolbar_action(actionSave);
                break;
            case QMessageBox::No:
                // No hacer nada
                break;
            default:
                return;
			}
		}

		// Borrar
		//tabWidget->removeTab(tabWidget->currentIndex());
		disconnect(currentNtv->textEdit()->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));
		NumberedTextView *ntv=currentNtv;
		currentNtv=NULL;
		//TODO: Check if another view (clone view) is using the same document object
		printf("Padre %d ; textEdit %d\n", ntv->textEdit()->document()->parent(), ntv->textEdit() );
		//if(ntv->textEdit()->document()->parent()==ntv->textEdit())
		/*
		{
			for(int i=0;i<tabWidget->count();i++)
			{
				NumberedTextView *w=(NumberedTextView *)tabWidget->widget(i);
				if(
					w!=ntv 
					&& 
					ntv->textEdit()->document()==w->textEdit()->document()
				)
				{
					printf("Padre %d ; textEdit %d\n", w->textEdit()->document()->parent(), w->textEdit() );
					ntv->textEdit()->document()->setParent(w->textEdit());
					w->textEdit()->setDocument(ntv->textEdit()->document());
					break;
				}
			}
		}
		*/
		delete ntv;

		if(!close_all_tabs) break;
	}

	// Crear uno si no queda ninguno
	if(tabWidget->count() == 0)
	{
		SimpleEditor *codeEdit = new SimpleEditor(NULL);

		connect(codeEdit, SIGNAL(dynamic_help_required(const QString &)), this, SLOT(emit_dynamic_help_required(const QString &)));

        NumberedTextView *ntv = new NumberedTextView(NULL, codeEdit, octave_connection);

		connect(ntv->textEdit(), SIGNAL(toggleBreakpoint(int)), this, SLOT(toggleBreakpoint(int)));

		connect(codeEdit->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));
		//connect(ntv->textEdit()->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));

		tabWidget->addTab(ntv, tr("New"));

		currentNtv = ntv;
	}
	else
	{
		tabChanged(tabWidget->currentIndex());
	}

	updateFileList();
}

/////////////////////////////////////////////////////////////

void Editor::newEditorTab()
{
	SimpleEditor *codeEdit = new SimpleEditor(NULL);

	connect(codeEdit, SIGNAL(dynamic_help_required(const QString &)), this, SLOT(emit_dynamic_help_required(const QString &)));

    NumberedTextView *ntv = new NumberedTextView(NULL, codeEdit, octave_connection);

    ntv->setFontSize(font_size);

	connect(ntv->textEdit(), SIGNAL(toggleBreakpoint(int)), this, SLOT(toggleBreakpoint(int)));
	
	connect(codeEdit->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));
	
	tabWidget->setCurrentIndex(tabWidget->addTab(ntv, tr("New")));
	
    currentNtv = ntv;
	
	updateFileList();

    // give the editor focus
    codeEdit->setFocus(Qt::OtherFocusReason);
}

/////////////////////////////////////////////////////////////

void Editor::loadFiles(const QStringList &files)
{
	QString path;

	foreach(path, files)
	{
		if( path.isEmpty() ) continue;
		
		try
		{
			// Si la pestaña activa no contiene ningún texto ni es un archivo,
			// abrir en ella
			
			if(!currentNtv->path().isEmpty() || !currentNtv->textEdit()->document()->isEmpty())
			{
				newEditorTab();
			}
			
			SimpleEditor *codeEdit=(SimpleEditor*)(currentNtv->textEdit());
			disconnect(codeEdit->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));
			//disconnect(currentNtv->textEdit()->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));

            // Ensure the path uses Qt's separator format
            path = QDir::fromNativeSeparators(path);

            // Open the file in this tab
			currentNtv->open(path);

			//connect(currentNtv->textEdit()->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));
			connect(codeEdit->document(), SIGNAL(modificationChanged (bool)), this, SLOT(textModified(bool)));

            // Extract only the file name for the tab text
            setTabText(tabWidget->currentIndex(), path.split("/").last());
		}
		catch(...)
		{
			QMessageBox::critical(NULL, tr("Error"), path + " can not be opened");
		}
	}
}

/////////////////////////////////////////////////////////////

void Editor::close_editor()
{
	if(parent()!=NULL) ((QWidget*)parent())->close();
	else close();
}

void Editor::emit_dynamic_help_required(const QString &text)
{
	//printf("%s\n", text.toLocal8Bit().data());
	emit dynamic_help_required(text);
}

BaseWidget *Editor::copyBaseWidget(QWidget * parent )
{
	saveProject();

	Editor *bw=new Editor(parent);
	bw->setSession(session);
	bw->octave_connection=octave_connection;

	for(int i=bw->tabWidget->count();i>0;i--)
	{
		bw->toolbar_action(bw->actionClose);
	}

	for(int i=0;i<tabWidget->count();i++)
	{
		NumberedTextView *code=((NumberedTextView*)tabWidget->widget(i) );
		if(i!=0) bw->toolbar_action(bw->actionNew);
		bw->currentNtv->textEdit()->setPlainText(code->textEdit()->toPlainText());
		bw->currentNtv->setPath(code->path());
		bw->currentNtv->setModified(code->modified());
		code->setModified(false);
		if(!code->path().isEmpty())
		{
			bw->setTabText(bw->tabWidget->currentIndex(), code->path().split("/").last());
		}
	}

	return bw;
}

void Editor::setTabText(int index, const QString & label)
{
	tabWidget->setTabText(index, label);

	updateFileList();
}

void Editor::updateFileList()
{
	ListModel *model=(ListModel *)list_files->model();

	model->clear();

	for(int i=0;i<tabWidget->count();i++)
	{
		model->append(tabWidget->tabText(i), i);
	}

	model->update();
}

void Editor::file_selected(const QModelIndex & index)
{
	ListModel *model=(ListModel *)list_files->model();
	tabWidget->setCurrentIndex(model->position(index));
}

void Editor::setFontSize(int newfontsize)
{
    font_size = newfontsize;

    // Check the requested font size is not larger or
    // smaller than allowed
    if (newfontsize < minFontSize)
        font_size = minFontSize;

    if (newfontsize > maxFontSize)
        font_size = maxFontSize;

    // Set the font size for all the open tabs
    for(int i=0;i<tabWidget->count();i++)
    {
        NumberedTextView *ntv = (NumberedTextView*)tabWidget->widget(i);

        ntv->setFontSize(font_size);
    }
}


void Editor::indent_callback()
{
	currentNtv->indent();
}

void Editor::unindent_callback()
{
	currentNtv->unindent();
}

void Editor::comment_callback()
{
	currentNtv->comment();
}

void Editor::uncomment_callback()
{
	currentNtv->uncomment();
}

void Editor::simple_rcs_callback()
{
	QString path=currentNtv->path();

	QString repository=path+"~~";
	QString command("simplercs \""+repository+"\" ");
	QProcess::startDetached(command);
	//QProcess::execute(command);
	printf("[NumberedTextView::save] Comando: %s\n", command.toLocal8Bit().data() );
}


void Editor::print_callback()
{
	QPrinter printer;

	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));

	if (currentNtv->textEdit()->textCursor().hasSelection())
		dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	if (dialog->exec() == QDialog::Accepted)
	{
		currentNtv->textEdit()->print(&printer);
	}
}


void Editor::clone_callback()
{
	QTextDocument *document=currentNtv->textEdit()->document();
	QString path=currentNtv->path();
	
	newEditorTab();
	
	currentNtv->textEdit()->setDocument(document);
	if(!path.isEmpty())
	{
		setTabText(tabWidget->currentIndex(), path.split("/").last());
		currentNtv->setPath(path);
	}
}


void Editor::toggleBreakPoint_callback()
{
    int lineno = currentNtv->textEdit()->textCursor().blockNumber()+1;

	currentNtv->toggleBreakpoint(lineno);
}

////////////////////////////////////////////////////////////////

void Editor::clipboard_double_clicked(const QModelIndex &index)
{
	QString text=index.data().toString();
	currentNtv->textEdit()->textCursor().insertText(text);
}
////////////////////////////////////////////////////////////////

ListModel::ListModel(QObject *parent, Editor *editor):QAbstractListModel(parent)
{
	this->editor=editor;
}

int ListModel::rowCount(const QModelIndex &parent) const
{
	return list.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() >= list.size())
		return QVariant();

	if (role == Qt::DisplayRole)
		return list.at(index.row()).name;
	else
		return QVariant();
}

void ListModel::clear()
{
	list.clear();
}

void ListModel::append(QString name, int position)
{
	ListItem item;
	item.name=name;
	item.position=position;
	list.append(item);
}

int ListModel::position(const QModelIndex &index)
{
	return list.at(index.row()).position;
}

void ListModel::update()
{
	//printf("[ListModel::update] %d Inicio\n",list.size());
	QModelIndex index0=index(0);
	beginInsertRows(index0, 0, list.size()-1);
	//printf("[ListModel::update] %d Proceso\n",list.size());
	endInsertRows();
	//printf("[ListModel::update] %d Fin\n",list.size());
}

QModelIndex ListModel::position_index(int position)
{
	for(int i=0;i<list.size();i++)
	{
		if(list[i].position==position)
		{
			return index(i,0);
		}
	}

	return QModelIndex();
}

Qt::DropActions ListModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

bool ListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (data->hasFormat("text/plain"))
	{
		QString path=data->text();
		editor->openFile(path);
		return true;
	}

	return false;
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

	return Qt::ItemIsDropEnabled | defaultFlags;
}


QStringList ListModel::mimeTypes() const
{
	QStringList types;
	types << "text/plain";
	return types;
}

////////////////////////////////////////////////////////////////////

ClipboardListView::ClipboardListView(QWidget *parent):QListView(parent)
{
	_stringModel = new QStringListModel();
	setModel(_stringModel);

	popup = new QMenu();
	QAction *edit=popup->addAction(tr("Edit"));
	popup->addSeparator();
	QAction *remove=popup->addAction(tr("Delete entry"));
	QAction *remove_all=popup->addAction(tr("Delete all"));
	popup->addSeparator();
	QAction *sort_ascending=popup->addAction(tr("Sort ascending"));
	QAction *sort_descending=popup->addAction(tr("Sort descending"));
	popup->addSeparator();
	QAction *up=popup->addAction(tr("Up"));
	QAction *down=popup->addAction(tr("Down"));
	QAction *top=popup->addAction(tr("Top"));
	QAction *bottom=popup->addAction(tr("Bottom"));
	popup->addSeparator();
	stopAction=popup->addAction(tr("Stop append"));
	stopAction->setCheckable(true);
	stopAction->setChecked(false);
	stopAction->setToolTip(tr("Stops append text from clipboard"));

	connect(edit, SIGNAL(triggered()),this, SLOT(edit_callback()));
	connect(remove, SIGNAL(triggered()),this, SLOT(remove_callback()));
	connect(remove_all, SIGNAL(triggered()),this, SLOT(remove_all_callback()));
	connect(sort_ascending, SIGNAL(triggered()),this, SLOT(sort_ascending_callback()));
	connect(sort_descending, SIGNAL(triggered()),this, SLOT(sort_descending_callback()));
	connect(up, SIGNAL(triggered()),this, SLOT(up_callback()));
	connect(down, SIGNAL(triggered()),this, SLOT(down_callback()));
	connect(top, SIGNAL(triggered()),this, SLOT(top_callback()));
	connect(bottom, SIGNAL(triggered()),this, SLOT(bottom_callback()));

	connect(this, SIGNAL(clicked (const QModelIndex &)), this, SLOT(clipboard_selected(const QModelIndex &)) );
	connect(QApplication::clipboard(),SIGNAL(dataChanged()), this, SLOT(clipboard_new_data_callback()) );

	//Load last state SmallClipboard
	QString home=QDir::home().path()+"/.qtoctave/SmallClipboard.xml";
	QFile inFile(home);
	if( inFile.exists() )
	{
		inFile.open(QIODevice::ReadOnly);
		QXmlStreamReader in(&inFile);
		loadStateXML(in);
		inFile.close();
	}

	setToolTip(
            "<b>Small Clipboard:</b>"
            "<p>Small Clipboard is a list of clipboard texts. When you copy some text using ctrl+C, this text is copied to system clipboard. If text is small, it will be listed in Small Clipboard. Whichever the text selected from Small Clipboard list, you can paste it pressing ctrl+V. You can paste text with double click, also.</p>"
            "Use right click to see popup menu."
            );
}

ClipboardListView::~ClipboardListView()
{
	QString home=QDir::home().path()+"/.qtoctave/SmallClipboard.xml";
	QFile outFile(home);
	outFile.open(QIODevice::WriteOnly);
	QXmlStreamWriter out(&outFile);
	saveStateXML(out);
	outFile.close();
}

void ClipboardListView::contextMenuEvent ( QContextMenuEvent * event )
{
	popup->popup(event->globalPos());
}

QStringListModel *ClipboardListView::stringModel()
{
	return _stringModel;
}

void ClipboardListView::edit_callback()
{
	QModelIndexList indexes=selectedIndexes();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		edit(index);
	}
}

void ClipboardListView::remove_callback()
{
	QModelIndexList indexes=selectedIndexes();
	QStringList list=_stringModel->stringList();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		QString text=index.data().toString();
		int k=list.indexOf(text);
		list.removeAt(k);
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::remove_all_callback()
{
	QStringList list;
	_stringModel->setStringList(list);
}

void ClipboardListView::sort_ascending_callback()
{
	QStringList list=_stringModel->stringList();
	list.sort();
	_stringModel->setStringList(list);
}

void ClipboardListView::sort_descending_callback()
{
	QStringList list=_stringModel->stringList();
	list.sort();
	for(int i=0; i<list.size()/2; i++)
	{
		list.swap(i, list.size()-1-i);
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::up_callback()
{
	QModelIndexList indexes=selectedIndexes();
	QStringList list=_stringModel->stringList();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		QString text=index.data().toString();
		int k=list.indexOf(text);
		if(k>0) list.swap(k, k-1);
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::down_callback()
{
	QModelIndexList indexes=selectedIndexes();
	QStringList list=_stringModel->stringList();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		QString text=index.data().toString();
		int k=list.indexOf(text);
		if(k<list.size()-1) list.swap(k, k+1);
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::top_callback()
{
	QModelIndexList indexes=selectedIndexes();
	QStringList list=_stringModel->stringList();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		QString text=index.data().toString();
		int k=list.indexOf(text);
		list.removeAt(k);
		list.prepend(text);
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::bottom_callback()
{
	QModelIndexList indexes=selectedIndexes();
	QStringList list=_stringModel->stringList();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes[i];
		QString text=index.data().toString();
		int k=list.indexOf(text);
		list.removeAt(k);
		list << text;
	}
	_stringModel->setStringList(list);
}

void ClipboardListView::clipboard_new_data_callback()
{
	if( stopAction->isChecked() ) return;

	QClipboard *clipboard = QApplication::clipboard();

	const QMimeData *mimedata = clipboard->mimeData();
	if(mimedata->hasText())
	{
		QStringList list=_stringModel->stringList();
		while(list.size()>100)
		{
			list.removeFirst();
		}

		QString text=clipboard->text();
		if( text.indexOf('\n')<0 && text.size()<256 && !list.contains(text) )
		{
			list << text;
			_stringModel->setStringList(list);
		}
	}
}

void ClipboardListView::clipboard_selected(const QModelIndex &index)
{
	QString text=index.data().toString();
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(text);
}

void ClipboardListView::saveStateXML(QXmlStreamWriter &out, bool partOk)
{
	if(!partOk) out.writeStartDocument();
	out.writeStartElement("ClipboardListView");
	out.writeStartElement("stopAction");
	if( stopAction->isChecked() ) out.writeAttribute("value", "true");
	else out.writeAttribute("value", "false");
	out.writeEndElement();
	QStringList list=_stringModel->stringList();
	for(int i=0;i<list.size();i++)
	{
		out.writeStartElement("entry");
		out.writeCharacters(list[i]);
		out.writeEndElement();
	}
	out.writeEndElement();
	if(!partOk) out.writeEndDocument();
}

void ClipboardListView::loadStateXML(QXmlStreamReader &in, bool partOk)
{
	bool insideOk=false;
	QStringList list;

	while( !in.atEnd() )
	{
		in.readNext();
		if( in.isStartElement() )
		{
			QStringRef name=in.qualifiedName();
			if(insideOk && name=="entry")
			{
				QString entry=in.readElementText();
				list << entry;
			}
			else if(insideOk && name=="stopAction")
			{
				if( "true"==in.attributes().value("value") )
					stopAction->setChecked(true);
				else
					stopAction->setChecked(false);
			}
			else if(name=="ClipboardListView")
				insideOk=true;
		}
		else if( in.isEndElement() )
		{
			QStringRef name=in.qualifiedName();
			if( name=="ClipboardListView" && insideOk )
				break;
		}
	}

	_stringModel->setStringList(list);
}
