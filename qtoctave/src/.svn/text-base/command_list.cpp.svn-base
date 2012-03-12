/* Copyright (C) 2007 P.L. Lucas
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

#include "command_list.h"
#include "projects/projects.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QApplication>
#include <QClipboard>
#include "terminal.h"

CommandList::CommandList( QWidget * parent ):BaseWidget(parent)
{
	widget_type=COMMANDLIST;
	
	setWindowTitle(tr("Commands' List"));
	setWindowIcon(QIcon(QString(ICON_PATH)+"/news_section.png"));
	
	QVBoxLayout *layout=new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	
	menu_bar=menuBar();
	
	list=new QListView(this);
	model=new StringListModel();
	list->setModel(model);
	list->setSelectionMode(QAbstractItemView::ExtendedSelection);
	list->show();
	list->setToolTip(tr("Right click to see menu"));
	
	QObject::connect(list, SIGNAL(activated(const QModelIndex & )), this, SLOT(itemClicked (const QModelIndex & )) );
	
	layout->addWidget(list);
	
	centralWidget()->setLayout(layout);
	
	list->setContextMenuPolicy(Qt::CustomContextMenu);
	
	//Builds context menu
	context_menu=new QMenu(tr("Command List"), this);
	
	QAction *copy_clipboard=new QAction(tr("Copy"), this);
	connect(copy_clipboard, SIGNAL(triggered()), this, SLOT(copy_clipboard_callback()) );
	context_menu->addAction(copy_clipboard);
	
	QAction *clean_history=new QAction(tr("Clean history"), this);
	clean_history->setIcon(QIcon(QString( ICON_PATH )+ "/eraser.png"));
	connect(clean_history, SIGNAL(triggered()), this, SLOT(clean_history_callback()) );
	context_menu->addAction(clean_history);
	
	connect(list, SIGNAL( customContextMenuRequested ( const QPoint & )  ), this, SLOT( contextMenu_cb( const QPoint & ) ) );
	
	menu_bar->addMenu(context_menu);
}

CommandList::~CommandList()
{
	QStringList history=model->stringList();
	Projects::saveListCommandHistory(session->getProjectName(), history);
}

void CommandList::setLineEdit(QLineEdit *lineEdit)
{
	this->lineEdit=lineEdit;
}

void CommandList::set_list(QStringList list)
{
	//printf("[CommandList::set_list] Setting list.\n");
	QStringList history;
	// history << project_history << list;
	project_history=list;
	history << list;
	int history_size=127;
	if(!get_config("lines_in_history").isEmpty())
	{
		history_size=get_config("lines_in_history").toInt();
	}
	while(history.size()>history_size)
	{
		history.removeFirst();
		//project_history.removeFirst();
	}
	model->setStringList(history);
	QModelIndex item=model->index(history.size()-1);
	this->list->scrollTo(item);
}

void CommandList::itemClicked (const QModelIndex & item )
{
	lineEdit->setText(item.data().toString());
}

void CommandList::setSession(Session *session)
{
	BaseWidget::setSession(session);
	setProject(session->getProjectName());
	connect(session, SIGNAL(projectChanged(QString)), this, SLOT(setProject(QString)));
}

void CommandList::setProject(QString project)
{
	QStringList history=Projects::listCommandHistory( project );
	project_history.clear();
	history.append(QDateTime::currentDateTime().toString("%% dddd MMMM d yyyy -- hh:mm:ss %%"));
	project_history << history;
	model->setStringList(history);
}

void CommandList::copy_clipboard_callback()
{
	QClipboard *clipboard=QApplication::clipboard();
	QString history;
	for(int i=0;i<model->rowCount();i++)
	{
		QModelIndex item=model->index(i);
		if( list->selectionModel()->isSelected(item) )
			history += item.data().toString()+"\n";
	}
	clipboard->setText(history);
}

void CommandList::contextMenu_cb( const QPoint & /*pos*/)
{
	context_menu->popup(QCursor::pos () );
}

void CommandList::clean_history_callback()
{
	QStringList list;
	//model->setStringList(list);
	project_history.clear();
	set_list(list);
}

QMenu *CommandList::get_menu()
{
	return context_menu;
}

BaseWidget *CommandList::copyBaseWidget(QWidget * parent )
{
	CommandList *bw=new CommandList(parent);
	Terminal *terminal=(Terminal*)session->getFirstTool(TERMINAL);
	connect(terminal->getAutocomplete(), SIGNAL(new_command_entered(QStringList)), bw, SLOT(set_list(QStringList)) );
	
	bw->setSession(session);
	bw->setLineEdit(terminal->getAutocomplete());
	bw->set_list(terminal->getAutocomplete()->commands());
	
	return bw;
}


StringListModel::StringListModel():QStringListModel()
{
}

Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index);
}
