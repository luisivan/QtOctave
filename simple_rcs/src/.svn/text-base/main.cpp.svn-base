#include "main.h"
#include <QFileInfo>
#include "extract_dialog.h"
#include "ui_help_dialog.h"
#include "add_comment_dialog.h"

#include <QHeaderView>

Main::Main(QWidget *parent): QMainWindow(parent)
{
	ui.setupUi(this);
	versions_list_model = new ListModel();
	ui.versions1->setModel(versions_list_model);
	ui.versions2->setModel(versions_list_model);
	
	ui.versions1->header()->setResizeMode(QHeaderView::ResizeToContents);
	ui.versions2->header()->setResizeMode(QHeaderView::ResizeToContents);
	
	connect(ui.versions1 , SIGNAL(clicked( const QModelIndex & )), this, SLOT(activated_version1 ( const QModelIndex &) ) );
	connect(ui.versions2, SIGNAL(clicked( const QModelIndex & )), this, SLOT(activated_version2 ( const QModelIndex &) ) );
	
	connect(ui.actionExtract, SIGNAL(triggered ()), this, SLOT(extract_callback()) );
	connect(ui.actionHelp, SIGNAL(triggered ()), this, SLOT(help_callback()) );
	connect(ui.actionExit, SIGNAL(triggered ()), this, SLOT(close()) );
	connect(ui.actionAdd_coment, SIGNAL(triggered ()), this, SLOT(add_comment_callback()) );
	
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(close()));
	
	ui.close_pushButton->setFocus(Qt::ActiveWindowFocusReason);
	
	rcsinterface=new RcsInterfaceTarDiff;
}


void Main::setRepository(QString repository)
{
	this->repository=repository;
	rcsinterface->setRepository(repository);
	
	QFileInfo repository_file(repository);
	
	setWindowTitle("SimpleRcs ["+repository_file.fileName()+"]");
}

void Main::setFile(QString file)
{
	if(!file.isEmpty())
	{
		this->file=file;
		rcsinterface->add(file);
	}
}

void Main::loadVersions()
{
	QStringList list=rcsinterface->versions();
	
	
	for(int i=0;i<(list.size()/2);i++)
	{
		QString aux=list[i];
		list[i]=list[list.size()-i-1];
		list[list.size()-i-1]=aux;
	}
	
	QHash<QString, QString> comments=rcsinterface->getComment();
	
	versions_list_model->setVersions(list);
	versions_list_model->setComments(comments);
	
	int i=0;
	
	if(i<list.size())
	{
		version1=list[i];
		i=1;
		if(i<list.size())
		{
			version2=list[i];
			QString result=rcsinterface->diff(version1, version2);
	
			ui.textBrowser->setHtml(result);
		}
	}
	
	
}

void Main::activated_version1 ( const QModelIndex & index )
{
	printf("Activado\n");
	
	version1=index.model()->index(index.row(),0).data().toString();
	if(version2.isEmpty()) return;
	
	QString result=rcsinterface->diff(version1, version2);
	
	ui.textBrowser->setHtml(result);
}

void Main::activated_version2 ( const QModelIndex & index )
{
	version2=index.model()->index(index.row(),0).data().toString();
	if(version1.isEmpty()) return;
	
	QString result=rcsinterface->diff(version1, version2);
	
	ui.textBrowser->setHtml(result);
}

void Main::extract_callback()
{
	ExtractDialog dialog(this);
	
	dialog.setModel(versions_list_model);
	
	if(QDialog::Accepted!=dialog.exec()) return;
	
	QString version=dialog.getVersion();
	QString path=dialog.getPath();
	
	if( version.isEmpty() || path.isEmpty() ) return;
	
	rcsinterface->extract(path, version);
}


void  Main::help_callback()
{
	QDialog dialog(this);
	
	HelpDialog ui;
	
	ui.setupUi(&dialog);
	
	dialog.exec();
}

void Main::add_comment_callback()
{
	AddCommentDialog dialog(this);
	
	dialog.setModel(versions_list_model);
	
	if(QDialog::Accepted!=dialog.exec()) return;
	
	QString version=dialog.getVersion();
	QString comment=dialog.getComment();
	
	if( version.isEmpty() ) return;
	
	rcsinterface->setComment(version, comment);
	
	loadVersions();
}


ListModel::ListModel(QObject *parent):QAbstractListModel(parent)
{
	
}

int ListModel::rowCount(const QModelIndex &/*parent*/) const
{
	 return versions.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	
	if (index.row() >= versions.size())
		return QVariant();
	
	if (role == Qt::DisplayRole)
	{
		if(index.column()==0) return versions.at(index.row());
		else return comments[versions.at(index.row())];
	}
	else
		return QVariant();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	
	if (orientation == Qt::Horizontal)
	{
		if(section==0) return QString("Version");
		else return QString("Comment");
	}
	
	return QString("%1").arg(section);
}

int ListModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
	return 2;
}

void ListModel::setVersions(QStringList versions)
{
	this->versions=versions;
}

void ListModel::setComments(QHash<QString,QString> comments)
{
	this->comments=comments;
}


int main(int argn, char* argv[])
{
	QString repository;
	QString file;
	
	if(argn>1) repository=argv[1];
	if(argn>2) file=argv[2];
	
	
	QApplication application(argn, argv);
	
	Main app;
	
	app.setRepository(repository);
	app.setFile(file);
	
	if(file.isEmpty())
	{
		app.loadVersions();
		
		app.show();

		return application.exec();

	}
	
	return 0;
}

