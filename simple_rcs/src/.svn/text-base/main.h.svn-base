#ifndef __MAIN_H__
#define __MAIN_H__

#include <QMainWindow>
#include <QStringListModel>

#include "ui_main_window.h"
#include "rcsinterface.h"
#include "rcsinterfacetardiff.h"
#include <QHash>

using namespace Ui;

class ListModel;

class Main:public QMainWindow
{
	Q_OBJECT

	MainWindow ui;
	//QStringListModel *versions_list_model;
	ListModel *versions_list_model;
	QString version1, version2;
	
	RcsInterface *rcsinterface;
	
	QString repository;
	QString file;
	
	public:
	Main(QWidget *parent = 0);
	
	void setRepository(QString repository);
	void setFile(QString file);
	
	void loadVersions();
	
	public slots:
	
	void activated_version1 ( const QModelIndex & index );
	void activated_version2 ( const QModelIndex & index );
	void extract_callback();
	void help_callback();
	void add_comment_callback();
};

class ListModel : public QAbstractListModel
 {
     Q_OBJECT

	public:
	ListModel(QObject *parent = 0);
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	
	void setVersions(QStringList versions);
	void setComments(QHash<QString,QString> comments);
	
	private:
	QStringList versions;
	QHash<QString,QString> comments;
 };

#endif
