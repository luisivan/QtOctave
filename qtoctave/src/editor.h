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

#ifndef __EDITOR_H__
#define __EDITOR_H__
#include <QAction>
#include <QToolBar>
#include <QTabWidget>
#include <QDropEvent>
#include <QDockWidget>
#include <QAbstractListModel>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "basewidget.h"
#include "syntax.h"
#include "octave_connection.h"
#include "search_dialog.h"
#include "simpleeditor.h"
#include "numberedtextview.h"
#include "projects/projects.h"

class ClipboardListView;

/** Editor with multiple file tabs, line numbering, breakpoint
  * insertion and syntax highlighting features. Supports several
  * editors and autocompletion code.*/
class Editor: public BaseWidget
{
    Q_OBJECT


public:

    /**New editor.*/
    Editor(QWidget *parent);
    ~Editor();

    /**Set conection with Octave.*/
    void setOctaveConnection(OctaveConnection *octave_connection);

    /**Gets active project name.*/
    QString getProject();

    /**Load a list of files.*/
    void loadFiles(const QStringList &files);

    /**Close one tab. If close_all_tabs== true close all tabs. It's usefull for load projects.*/
    void closeTabs(bool close_all_tabs);

    void setSession(Session *session);

    BaseWidget *copyBaseWidget( QWidget * parent = 0 );

    /** Sets the font size in the editor and all editor windows
      */
    void setFontSize(int newfontsize);

private:

    //Project opened in this editor.
    QString project_name;

    QTabWidget *tabWidget;
    QToolBar *toolBar;
    QDockWidget *list_files_dock, list_clipboard_dock;
    QListView *list_files;

    // The font size to be used in the editor and number bar
    int font_size;

    // The max and min font sizes allowed in the editor
    int minFontSize, maxFontSize;

    ClipboardListView *list_clipboard;

    // Declare a connection to octave
    OctaveConnection *octave_connection;

    // The search dialog will be used to find text in files
    SearchDialog *search_dialog;

    // Declare a NumberedTextView to hold the currently selected
    // text view the user is working on
    NumberedTextView *currentNtv;

    // Actions

    // Actions relating to the manipulation of files
    QAction *actionNew, *actionOpen, *actionSave, *actionSaveAs;
    QAction *actionClose, *actionClone, *actionPrint;

    // Actions relating to the editing of text in the editor
    QAction *actionUndo, *actionRedo,*actionCut, *actionCopy, *actionPaste, *actionSearch;
    QAction *actionIndent, *actionUnindent, *actionComment, *actionUncomment, *actionSimpleRCS;
    QAction *actionZoomIn, *actionZoomOut;

    // Actions related to the running and debugging of octave code files
    QAction *actionToggleBreakPoint, *actionRun, *actionDebug, *actionSendToOctave;
    QAction *actionStep, *actionStepIn, *actionStepOut, *actionContinue, *actionDebugQuit;

    // The editor menu items
    QMenu *menuFile, *menuEdit, *menuRun, *menuConfig, *menuDocks, *menuTools;

    void saveProject();

    /**Defines a new label for the page at position index's tab and updates list_files*/
    void setTabText(int index, const QString & label);

    /**Updates list_files*/
    void updateFileList();


protected:
    //   void dropEvent ( QDropEvent * event );
    //   void dragEnterEvent(QDragEnterEvent *event);
    void closeEvent ( QCloseEvent * event );


public slots:

    /**Execute actions as copy, paste, save, run, ...*/
    void toolbar_action(QAction*);

    /**Search word inserted in dialog in code.*/
    void search();

    /**Replace word inserted in dialog in code.*/
    void replace();

    /**Put/Quit break point to debug Octave code.*/
    void toggleBreakpoint(int lineno);

    /**Another document is selected to edit.*/
    void tabChanged(int index);

    /**Sets actual document modify flag true or false.*/
    void textModified(bool ok=true);

    /**Starts debug of Octave code.*/
    void debug(int lineno, int colno, QString filestr);

    /**Ends of debug of Octave code.*/
    void endDebug();

    /**File selected in list file*/
    void file_selected(const QModelIndex & index);

    /**Open file.*/
    void openFile(QString file=QString());

    /**Emits dynamic_help_required*/
    void emit_dynamic_help_required(const QString &text);

    /**Sets active project name.*/
    void setProject(QString name);

    /**Indent text.*/
    void indent_callback();

    /**Indent text.*/
    void unindent_callback();

    /**Indent text.*/
    void comment_callback();

    /**Indent text.*/
    void uncomment_callback();

    /**Shows simple_rcs tool for actual file*/
    void simple_rcs_callback();

    /**Sets/unsets breakpoint for debug*/
    void toggleBreakPoint_callback();

    /**Close window*/
    void close_editor();

    /**Print file*/
    void print_callback();

    /**Shows edit menu*/
    void show_edit_menu();

    /**Clone editor tab. Clone actual editor tab.*/
    void clone_callback();

    /**Text in clipboard has been selected double clicked, insert in text.*/
    void clipboard_double_clicked(const QModelIndex &);


private:

    /**Add new empty tab to editor*/
    void newEditorTab();

};


class ListModel: public QAbstractListModel
{
    Q_OBJECT

    Editor *editor;

public:

    ListModel(QObject *parent = 0, Editor *editor = 0 );

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    /**Clear data*/
    void clear();

    void append(QString name, int position);

    void update();

    int position(const QModelIndex &index);

    QModelIndex position_index(int position);

    Qt::DropActions supportedDropActions() const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList mimeTypes() const;


private:

    class ListItem
    {
    public:
        QString name;
        int position;
    };

    QList <ListItem> list;
};



class ClipboardListView:public QListView
{
    Q_OBJECT

    QMenu *popup;

    QStringListModel *_stringModel;

    QAction *stopAction;


public:

    ClipboardListView(QWidget *parent = 0);
    ~ClipboardListView();

    QStringListModel *stringModel();

    /**Saves state of ClipboardListView in xml.
 * @param partOk if true, xml is part of other xml file*/
    void saveStateXML(QXmlStreamWriter &out, bool partOk = false);

    void loadStateXML(QXmlStreamReader &in, bool partOk = false);


protected:

    void contextMenuEvent ( QContextMenuEvent * event );


public slots:

    void edit_callback();

    void remove_callback();

    void remove_all_callback();

    void sort_ascending_callback();

    void sort_descending_callback();

    void up_callback();

    void down_callback();

    void top_callback();

    void bottom_callback();

    /**Text in clipboard has been selected.*/
    void clipboard_selected(const QModelIndex &);

    /**New clipboard data*/
    void clipboard_new_data_callback();
};

#endif
