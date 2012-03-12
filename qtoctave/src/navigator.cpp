/* Copyright (C) 2006 P.L. Lucas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PQListView ARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */

//#include <QFileSystemModel>
#include <QDirModel>
#include <QListView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QHBoxLayout>
#include <QPushButton>
#include <QModelIndex>
#include <QDrag>
#include <QMimeData>
#include <QMessageBox>
#include <QCompleter>

#include "navigator.h"
#include "config.h"


Navigator::Navigator(QWidget *parent) : BaseWidget(parent) 
{
	widget_type=NAVIGATOR;
	
	octave_connection=NULL;
	copy_ok=false; cut_ok=false;
	
	setWindowTitle(tr("Navigator"));
	setWindowIcon( QIcon(QString( ICON_PATH )+"/new_folder.png"));
	createActions();
	//createMenuBar();
	createToolBar();
	createLayout();
	createConnections();
	createContextMenu();

	centralWidget()->setLayout(layout);
	//show();
}

void Navigator::createActions() 
{
	newFolder = new QAction(QIcon(QString(ICON_PATH) + "/new_folder"), tr("&New folder"), this);
	exit = new QAction(QIcon(QString(ICON_PATH) + "/exit.png"), tr("Exit"), this);

	cut = new QAction(QIcon(QString(ICON_PATH) + "/editcut"), tr("Cu&t"), this);
	copy = new QAction(QIcon(QString(ICON_PATH) + "/editcopy"), tr("&Copy"), this);
	paste = new QAction(QIcon(QString(ICON_PATH) + "/editpaste"), tr("&Paste"), this);

	back = new QAction(QIcon(QString(ICON_PATH) + "/back.png"), tr("&Back"), this);
	forward = new QAction(QIcon(QString(ICON_PATH) + "/forward.png"), tr("&Forward"), this);
	up = new QAction(QIcon(QString(ICON_PATH) + "/up.png"), tr("Up"), this);
	remove=new QAction(QIcon(QString(ICON_PATH) + "/button_cancel.png"), tr("Delete file"), this);
	rename=new QAction(tr("Rename"), this);
	/* */
}

/*void Navigator::createMenuBar() 
{
	menubar = new QMenuBar(this);

	QMenu *menu = menubar->addMenu("&File");
	menu->addAction(newFolder);
	menu->addAction(exit);

	menu = menubar->addMenu("&Edit");
	menu->addAction(cut);
	menu->addAction(copy);
	menu->addAction(paste);

	menu = menubar->addMenu("&Navigator");
	menu->addAction(back);
	menu->addAction(forward);
	menu->addAction(up);
}*/

void Navigator::setSession(Session *session)
{
	//Sets octave_connection to NULL to not change dir at creation
	//Uses last pwd in Octave.
	OctaveConnection *oc=octave_connection;
	octave_connection=NULL;
	
	BaseWidget::setSession(session);
	setProject(session->getProjectName());
	connect(session, SIGNAL(projectChanged(QString)), this, SLOT(setProject(QString)) );
	
	//Restore octave_connection
	octave_connection=oc;
}

void Navigator::createToolBar() 
{
	toolbar = addToolBar(tr("Navigation tools"));
	toolbar->setObjectName("Navigation tools");
	//toolbar->setMinimumHeight(40);
  	//toolbar->setMaximumHeight(40);

	toolbar->addAction(back);
	toolbar->addAction(forward);
	toolbar->addAction(up);
	toolbar->addSeparator();
	
	toolbar->addAction(cut);
	toolbar->addAction(copy);
	toolbar->addAction(paste);
	toolbar->addSeparator();
	
	toolbar->addAction(newFolder);

}

void Navigator::createLayout() 
{
	layout = new QVBoxLayout(centralWidget());
	layout->setSpacing(0);
	layout->setMargin(0);
	//layout->setMenuBar(menubar);
	//layout->setStretchFactor(toolbar, 0);
	//layout->addWidget(toolbar);
	
	
	listmodel = new QFileSystemNewModel();
	listmodel->setResolveSymlinks(true);
	listmodel->setFilter(QDir::AllDirs|QDir::AllEntries|QDir::NoDotAndDotDot);
	listmodel->setNameFilters(QStringList() << "*.m");
	listmodel->sort(QFileSystemModel::FileNameRole);

	url = new QComboBox(this);
	url->setEditable(true);
	url->setDuplicatesEnabled(false);
	url->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	QCompleter *completer = new QCompleter(this);
	completer->setModel(new QDirModel(completer));
	url->setCompleter(completer);
	
	go = new QPushButton(tr("Go"), this);
	go->setFixedSize(url->height(), url->height());

	QHBoxLayout *urlBox = new QHBoxLayout();
	urlBox->addWidget(url);
	urlBox->addSpacing(10);
	urlBox->addWidget(go);
	urlBox->addSpacing(10);

	layout->addLayout(urlBox);
	
	filter_comboBox=new QComboBox(this);
	filter_comboBox->setEditable(true);
	filter_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	filter_comboBox->addItem("*.m");
	filter_comboBox->addItem("*");
	filter_comboBox->addItem("*.txt");
	filter_comboBox->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	
	QHBoxLayout *filterBox = new QHBoxLayout();
	QLabel *filters_label=new QLabel(tr("Filters: "));
	filterBox->addWidget(filters_label);
	filterBox->addWidget(filter_comboBox);
	layout->addLayout(filterBox);
	

	QString paths=get_config("navigator_start_path"), path = QDir::currentPath();
	if(!paths.isEmpty())
	{
		QStringList values=paths.split("\n");
		url->addItems(values);
	}
	QModelIndex root = listmodel->index(path);
	
	if(!root.isValid())
	{
		path = QDir::currentPath();
		root = listmodel->index(path);
	}
	
	listmodel->setRootPath(QDir::rootPath());
	
	printf("[Navigator::createLayout] Building listview\n");

	listview = new DragDropQTreeView(this);
	listview->setSelectionMode(QAbstractItemView::ExtendedSelection);
	listview->setContextMenuPolicy(Qt::CustomContextMenu);
	listview->setMouseTracking(true);
	listview->setSortingEnabled(true);
	listview->setItemsExpandable(false);
	listview->setRootIsDecorated(false);
	listview->setModel(listmodel);
	listview->setRootIndex(root);
	//listview->setCursor(Qt::OpenHandCursor);
	
	//printf("[Navigator::createLayout] Listview builded\n");


	url->lineEdit()->setText(path);

	back->setEnabled(false);
	forward->setEnabled(false);

	if (!(listmodel->parent(root)).isValid())
		up->setEnabled(false);

	layout->addWidget(listview,1);
}

QString Navigator::getNavigatorCurrentPath()
{
	QModelIndex root = listview->rootIndex();
	QString rootPath = listmodel->filePath(root);
	return rootPath;
}

Navigator::~Navigator()
{
	QMap<QString,QString> configuration;
	QString values=getNavigatorCurrentPath();
	QStringList repetidos; repetidos << values;
	for(int i=0;i<url->count() && i<10;i++)
	{
		QDir dir(url->itemText(i));
		if(!repetidos.contains(url->itemText(i)) && dir.exists())
		{
			values+="\n"+url->itemText(i);
			repetidos << url->itemText(i);
		}
	}
	configuration["navigator_start_path"]=values;
	set_config(configuration);
	
	if(project_name.isEmpty()) Projects::saveNavigatorPath("Empty", getNavigatorCurrentPath());
	else Projects::saveNavigatorPath(project_name, getNavigatorCurrentPath());
}

void Navigator::setProject(QString name)
{
	project_name=name;
	
	setNavigatorCurrentPath(Projects::navigatorPath(name));
}

void Navigator::createConnections()
{
	connect(listview, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
	connect(listview, SIGNAL(entered(const QModelIndex &)), this, SLOT(change_cursor_cb(const QModelIndex &)));
	connect(back, SIGNAL(triggered(bool)), this, SLOT(goBack(bool)));
	connect(forward, SIGNAL(triggered(bool)), this, SLOT(goForward(bool)));
	connect(up, SIGNAL(triggered(bool)), this, SLOT(goUp(bool)));
	//connect(url->lineEdit(), SIGNAL(returnPressed()), this, SLOT(urlButtonPressed()));
	connect(url, SIGNAL(activated(const QString & )), this, SLOT(urlButtonPressed(const QString &)));
	connect(go, SIGNAL(clicked(bool)), this, SLOT(urlButtonPressed(bool)));
	connect(filter_comboBox, SIGNAL(activated(const QString & )), this, SLOT(filter_cb(const QString &)));
	connect(newFolder, SIGNAL( triggered() ), this, SLOT( mkdir() ) );
	connect(listview, SIGNAL( customContextMenuRequested ( const QPoint & )  ), this, SLOT( contextMenu_cb( const QPoint & ) ) );
	connect(copy, SIGNAL( triggered() ), this, SLOT( copy_cb() ) );
	connect(cut, SIGNAL( triggered() ), this, SLOT( cut_cb() ) );
	connect(remove, SIGNAL( triggered() ), this, SLOT( remove_cb() ) );
	connect(paste, SIGNAL( triggered() ), this, SLOT( paste_cb() ) );
	connect(rename, SIGNAL( triggered() ), this, SLOT( rename_cb() ) );
}


void Navigator::setOctaveConnection(OctaveConnection *oc)
{
	octave_connection=oc;
}


void Navigator::setUrl(const QModelIndex &index)
{
	QString path = listmodel->filePath(index);
	url->lineEdit()->setText(path);
}

void Navigator::setUpActionState(const QModelIndex &index)
{
	// Disables up Action if index has no parent. If has Enables it
	if (!(listmodel->parent(index)).isValid())
		up->setEnabled(false);
	else 
		up->setEnabled(true);
}

void Navigator::expanded(const QModelIndex &index)
{
	setUpActionState(index);

	if (listmodel->isDir(index)) {
		QModelIndex root = listview->rootIndex();
		QString rootPath = listmodel->filePath(root);
		
		// Add current dir to back Action record
		backHistory.append(rootPath);
		
		// Enables back Action
		back->setEnabled(true);
		
		// Clean forward Action record
		int size = forwardHistory.size();
		
		if ( size > 0) {
			forwardHistory.remove(0, size);
		}

		// Disables forward Action
		forward->setEnabled(false);
		
		// Change current URL to index`s URL
		setUrl(index);
		
		// Change listview current dir
		listview->setRootIndex(index);
		//listmodel->refresh(index);
		
		if(octave_connection!=NULL)
		{
			QFileInfo dirinfo = listmodel->fileInfo(index);

			octave_connection->command_enter("cd \""+dirinfo.absoluteFilePath()+"\"", true);
		}

	} else {
		//It´s a file... Do something!!
		//drag_start(index);
	}
}

/*
void Navigator::drag_start( const QModelIndex & index )
{
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setText(listmodel->filePath(index));
	
	//printf("%s\n",listmodel->filePath(index).toLocal8Bit().data());
	
	drag->setMimeData(mimeData);
	drag->setPixmap( (QString(ICON_PATH) + "/filenew.png") );
	//drag->setDragCursor(QCursor(Qt::ClosedHandCursor).pixmap(), Qt::CopyAction);

	//Qt::DropAction dropAction =
	drag->start();
}
*/

void Navigator::goBack(bool /*checked*/)
{
	int size = backHistory.size();

	if (size > 0) {
		QString lastPath = backHistory.last();
		QModelIndex last = listmodel->index(lastPath);
		QModelIndex root = listview->rootIndex();
		QString rootPath = listmodel->filePath(root);
		
		// Disables back Action if there is no more dirs saved
		if (--size == 0)
			back->setEnabled(false);
		
		// Add current dir to forward Action record
		forwardHistory.append(rootPath);

		// Enables forward Action
		forward->setEnabled(true);
	
		// Change current URL to last`s URL
		setUrl(last);

		// Change listview current dir
		listview->setRootIndex(last);
		//listmodel->refresh(last);
		
		listview->scrollTo(listmodel->index(rootPath));
		
		setUpActionState(last);

		// Remove last dir from back Action record
		backHistory.remove(size);
	}
}

void Navigator::goForward(bool /*checked*/)
{
	int size = forwardHistory.size();

	if (size > 0) {
		QString lastPath = forwardHistory.last();
		QModelIndex last = listmodel->index(lastPath);
		QModelIndex root = listview->rootIndex();
		QString rootPath = listmodel->filePath(root);		

		// Disables forward Action if there is no more dirs saved
		if (--size == 0)
			forward->setEnabled(false);
		
		// Add current dir to back Action record
		backHistory.append(rootPath);

		// Enables back Action
		back->setEnabled(true);
	
		// Change current URL to last`s URL
		setUrl(last);

		// Change listview current dir
		listview->setRootIndex(last);
		//listmodel->refresh(last);

		setUpActionState(last);

		// Remove last dir from forward Action record
		forwardHistory.remove(size);
	}
}

void Navigator::goUp(bool /*checked*/)
{
	QModelIndex root = listview->rootIndex();
	QModelIndex parent = listmodel->parent(root);

	emit expanded(parent);
}

void Navigator::urlButtonPressed(const QString & )
{
	go->click();
}

void Navigator::urlButtonPressed(bool /*checked*/)
{
	setNavigatorCurrentPath( url->lineEdit()->text() );
}

void Navigator::setNavigatorCurrentPath(QString path)
{
	if( path.isEmpty() ) return;
	
	QModelIndex next = listmodel->index(path);
	
	if (next.isValid()) {
		emit expanded(next);
	}
}

#include <QInputDialog>

void Navigator::mkdir()
{
	QString dir_name=QInputDialog::getText(this, tr("New Folder"), tr("Name:") );
	
	if( !dir_name.isEmpty() )
	{
		QModelIndex index = listview->rootIndex();
		QDir dir(listmodel->filePath(index));
		//printf("%s/%s\n",listmodel->filePath(index).toLocal8Bit().data(), dir_name.toLocal8Bit().data() );
		//listmodel->mkdir(index, dir_name);
		dir.mkdir(dir_name);
		//listmodel->refresh(index);
	}
}

void Navigator::contextMenu_cb( const QPoint & /*pos*/)
{
	context_menu->popup(QCursor::pos () );
}

void Navigator::createContextMenu()
{
	context_menu=new QMenu(this);
	
	QList<QAction *> actions;
	actions.append(newFolder);
	actions.append(copy);
	actions.append(cut);
	actions.append(paste);
	actions.append(remove);
	actions.append(rename);
	
	context_menu->addActions(actions);
}

/**Returns selected files.
*/
static QList<QFileInfo> selected_files(QTreeView *listview, QFileSystemNewModel *listmodel)
{
	QList<QFileInfo> list;
	QModelIndexList indexes=listview->selectionModel()->selectedRows();
	for(int i=0; i<indexes.size(); i++)
	{
		QModelIndex index=indexes.at(i);
		list.append(listmodel->fileInfo(index));
	}
	return list;
}

void Navigator::copy_cb()
{
	file_list=selected_files(listview, listmodel);
	file_list_parent_path=listmodel->fileInfo(listview->rootIndex());
	copy_ok=true;
	cut_ok=false;
}

void Navigator::cut_cb()
{
	file_list=selected_files(listview, listmodel);
	file_list_parent_path=listmodel->fileInfo(listview->rootIndex());
	copy_ok=false;
	cut_ok=true;
}

void Navigator::paste_cb()
{
	if(file_list.isEmpty()) return;
	
	FileActions *f=new FileActions(this);
	connect(f, SIGNAL(process_end(QThread*)), this, SLOT(process_end(QThread*)) );
	
	if(copy_ok)
	{
		QModelIndex index = listview->rootIndex();

		f->copy(file_list_parent_path, file_list, listmodel->filePath(index));
		
		//copy_ok=false;
		
		//file_list.clear();
	}
	else if(cut_ok)
	{
		QModelIndex index = listview->rootIndex();

		f->cut(file_list_parent_path, file_list, listmodel->filePath(index));
		
		cut_ok=false;
		
		file_list.clear();
	}
}

void Navigator::remove_cb()
{
	int ok=QMessageBox::question(NULL, tr("Remove file"), tr("Remove selected files?"), QMessageBox::Yes, QMessageBox::No);
	
	if(ok!=QMessageBox::Yes) return;
	
	file_list=selected_files(listview, listmodel);
	copy_ok=false;
	cut_ok=false;
	
	if(file_list.isEmpty()) return;
	
	FileActions *f=new FileActions(this);
	connect(f, SIGNAL(process_end(QThread*)), this, SLOT(process_end(QThread*)) );
	f->remove(file_list);
	
	file_list.clear();
}

void Navigator::rename_cb()
{
	QList<QFileInfo> file_list=selected_files(listview, listmodel);
	
	if( file_list.isEmpty() ) return;
	
	QString name=QInputDialog::getText(this, tr("Rename"), tr("Name:") );
	
	if( !name.isEmpty() )
	{
		QFileInfo fileinfo=file_list.at(0);
		
		QFile::rename(fileinfo.absoluteFilePath(), fileinfo.absolutePath()+"/"+name);
		
		//listmodel->refresh();
	}
}

void Navigator::process_end(QThread *object)
{
	object->wait();
	//listmodel->refresh();
	delete object;
}

void Navigator::filter_cb(const QString &s)
{
	listmodel->setNameFilters(QStringList() << s);
}

void Navigator::change_cursor_cb(const QModelIndex &index)
{
	if(listmodel->isDir(index))
		listview->setCursor(Qt::ArrowCursor);
	else
		listview->setCursor(Qt::OpenHandCursor);
}

FileActions::FileActions(QObject *parent):QThread(parent)
{
	window=new QWidget;
	window->setWindowTitle(tr("Processing files..."));
	
	QVBoxLayout *layout=new QVBoxLayout;
	
	progressBar=new QProgressBar(window);
	layout->addWidget(progressBar);
	progressBar->setMinimum(0);
	progressBar->setMaximum(100);
	progressBar->setValue(0);
	progressBar->show();
	
	textEdit=new QTextEdit(window);
	layout->addWidget(textEdit);
	textEdit->setReadOnly(true);
	textEdit->show();
	
	QHBoxLayout *button_layout=new QHBoxLayout;
	layout->addLayout(button_layout);
	
	ok_button=new QPushButton(window);
	ok_button->setText(tr("Ok"));
	button_layout->addWidget(ok_button);
	ok_button->show();
	
	cancel_button=new QPushButton(window);
	cancel_button->setText(tr("Cancel"));
	button_layout->addWidget(cancel_button);
	cancel_button->show();
	
	window->setLayout(layout);
	
	connect(cancel_button, SIGNAL(clicked()), this, SLOT(stop()) );
	connect(ok_button, SIGNAL(clicked()), this, SLOT(ok_button_callback()) );
	connect(this, SIGNAL(process_value(int)), progressBar, SLOT(setValue(int)) );
	
	copy_ok=move_ok=remove_ok=stop_ok=false;
	
	window->show();
}

FileActions::~FileActions()
{
	delete window;
}

void FileActions::stop()
{
	if(!isRunning()) emit process_end(this);
	stop_ok=true;
}

void FileActions::ok_button_callback()
{
	start();
}

void FileActions::copy(QFileInfo parent, QList<QFileInfo> list, QString path)
{
	this->list=list;
	this->path=path;
	this->parent=parent;
	
	copy_ok=true;
	
	window->show();
	
	set_label_text("<p><b>"+tr("These files will be copyed:")+"</b></p>");
}

void FileActions::cut(QFileInfo parent, QList<QFileInfo> list, QString path)
{
	this->list=list;
	this->path=path;
	this->parent=parent;
	
	move_ok=true;
	
	window->show();
	
	set_label_text("<p><b>"+tr("These files will be cut:")+"</b></p>");
}

void FileActions::remove(QList<QFileInfo> list)
{
	this->list=list;
	remove_ok=true;
	
	window->show();
	
	set_label_text("<p><b>"+tr("These files will be removed:")+"</b></p>");
}

QList<QFileInfo> FileActions::find(QList<QFileInfo> files)
{
	QList<QFileInfo> list_files;
	
	for(int i=0;i<files.size();i++)
	{
		list_files.append(find(files[i]));
	}
	
	return list_files;
}

QList<QFileInfo> FileActions::find(QFileInfo file)
{
	QList<QFileInfo> list_files;
	if(file.isDir())
	{
		QDir dir(file.absoluteFilePath());
		list_files=dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files);
		for(int i=0;i<list_files.size();i++)
		{
			if(list_files[i].isDir())
			{
				QDir dir_aux(list_files[i].absoluteFilePath());
				list_files.append(dir_aux.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files));
			}
		}
	}
	list_files.append(file);
	for(int i=0;i<list_files.size();i++)
		printf("%s\n", list_files[i].absoluteFilePath().toLocal8Bit().data());
	return list_files;
}

void FileActions::run()
{
	list=find(list);
	for(int i=0;i<list.size() && !stop_ok;i++)
	{
		QFileInfo file=list.at(i);
		
		if(copy_ok) copy(file,path);
		else if(move_ok) cut(file,path);
		else if(remove_ok) remove(file);
		
		int value=(((double)i)/(double)list.size())*100;
		
		emit process_value(value);
	}
	
	emit process_end(this);
	copy_ok=move_ok=remove_ok=stop_ok=false;
}

void FileActions::set_label_text(QString text)
{
	text="<html>"+text;
	
	for(int i=0;i<list.size();i++)
	{
		text+=list.at(i).absoluteFilePath()+"<br>";
	}
	textEdit->setHtml(text+"<html>");
}

static QString destinationPathToCopy(QFileInfo parent, QFileInfo file, QString path)
{
	QDir parent_dir(parent.absoluteFilePath());
	return path+QDir::separator()+parent_dir.relativeFilePath(file.absoluteFilePath());
}

void FileActions::copy(QFileInfo file, QString path)
{
	QString file_path=file.absoluteFilePath();
	QString destination_path=destinationPathToCopy(parent, file, path);
	
	printf("[FileActions::copy] Destination %s\n", destination_path.toLocal8Bit().data());

	QFileInfo destination_info(destination_path);
	if(destination_info.exists()) return;
	if(file.isDir())
	{
		QDir pathDir(path);
		pathDir.mkpath(destination_path);
	}
	else
	{
		if(! QFileInfo(destination_info.dir().absolutePath()).exists() )
		{
			QDir pathDir(path);
			pathDir.mkpath(destination_info.dir().absolutePath());
		}
		QFile::copy(file_path, destination_path);
	}
	
/*
	#if defined Q_OS_WIN32
		system(("copy \""+file_path+"\" \""+path+"\"").toLocal8Bit().data() );
	#else
		system(("cp -fp \""+file_path+"\" \""+path+"\"").toLocal8Bit().data() );
	#endif
*/
}

void FileActions::cut(QFileInfo file, QString path)
{
	copy(file, path);
	remove(file);

/*
	#if defined Q_OS_WIN32
		copy(file, path);
		remove(file);
	#else
		QString file_path=file.absoluteFilePath();
		QString destination_path=destinationPathToCopy(parent, file, path);
		
		printf("[FileActions::cut] Destination %s\n", destination_path.toLocal8Bit().data());
	
		QFileInfo destination_info(destination_path);
		if( destination_info.exists() || !file.exists() ) return;
		if(file.isDir())
		{
			QDir pathDir(path);
			pathDir.mkpath(destination_path);
		}
		else
		{
			if(! QFileInfo(destination_info.dir().absolutePath()).exists() )
			{
				QDir pathDir(path);
				pathDir.mkpath(destination_info.dir().absolutePath());
			}
			QFile::rename(file_path, destination_path);
		}
	#endif
*/
}

void FileActions::remove(QFileInfo file)
{
	QString file_path=file.absoluteFilePath();
	QDir pathDir=file.dir();
	
	if(file.isDir())
	{
		pathDir.rmpath(file_path);
	}
	else
	{
		pathDir.remove(file_path);
	}
	
/*
	#if defined Q_OS_WIN32
		system(("del \""+file_path+"\"").toLocal8Bit().data() );
	#else
		system(("rm -f \""+file_path+"\"").toLocal8Bit().data() );
	#endif
*/
}

DragDropQTreeView::DragDropQTreeView(QWidget *parent):QTreeView(parent)
{
}

/*
void DragDropQTreeView::mousePressEvent(QMouseEvent *event)
{
	QModelIndex index=indexAt( event->pos() );
	if (event->button() == Qt::LeftButton &&  index.isValid() )
	{
		QFileSystemNewModel *listmodel=(QFileSystemNewModel *)model();
		
		//if( index.column()==-1 )
		//{
		//	selectionModel()->select(index, QItemSelectionModel::Toggle);
		//	return;
		//}
		
		if( listmodel->isDir(index) )
		{
			QTreeView::mousePressEvent(event);
			return;
		}
		
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		
		mimeData->setText(listmodel->filePath(index));
		
		//printf("%s\n",listmodel->filePath(index).toLocal8Bit().data());
		
		drag->setMimeData(mimeData);
		drag->setPixmap( (QString(ICON_PATH) + "/filenew.png") );
		//drag->setDragCursor(QCursor(Qt::ClosedHandCursor).pixmap(), Qt::CopyAction);
	
		drag->start();
	}
}
*/

void DragDropQTreeView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragStartPosition = event->pos();
		//printf("[DragDropQTreeView::mousePressEvent]\n");
	}
	QTreeView::mousePressEvent(event);
}

void DragDropQTreeView::mouseMoveEvent(QMouseEvent *event)
{
	QModelIndex index=indexAt( event->pos() );
	
	if ( (event->buttons() & Qt::LeftButton) &&  index.isValid() )
	{
		//if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
		//	return;
		QFileSystemNewModel *listmodel=(QFileSystemNewModel *)model();
		
		//if( index.column()==-1 )
		//{
		//	selectionModel()->select(index, QItemSelectionModel::Toggle);
		//	return;
		//}
		
		//printf("[DragDropQTreeView::mouseMoveEvent]\n");
		
		if( listmodel->isDir(index) )
		{
			//QTreeView::mousePressEvent(event);
			return;
		}
		
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		
		mimeData->setText(listmodel->filePath(index));
		
		//printf("%s\n",listmodel->filePath(index).toLocal8Bit().data());
		
		drag->setMimeData(mimeData);
		drag->setPixmap( (QString(ICON_PATH) + "/filenew.png") );
		//drag->setDragCursor(QCursor(Qt::ClosedHandCursor).pixmap(), Qt::CopyAction);
	
		drag->start();
	}
	
	QTreeView::mouseMoveEvent(event);
}

BaseWidget *Navigator::copyBaseWidget(QWidget * parent )
{
	Navigator *bw=new Navigator(parent);
	bw->setSession(session);
	bw->setOctaveConnection(octave_connection);
	
	return bw;
}


