#ifndef __EXTRACTDIALOG_H__
#define __EXTRACTDIALOG_H__

#include <QDialog>

#include "ui_extract_dialog.h"


using namespace Ui;

class ExtractDialog:public QDialog
{
	Q_OBJECT

	UiExtractDialog ui;
	QString version;
	
	
	public:
	ExtractDialog(QWidget *parent = 0);
	
	void setModel(QAbstractListModel *model);
	
	public slots:
	
	void select_callback();
	void activated_version( const QModelIndex & index );
	inline QString getVersion() {return version;}
	inline QString getPath() {return ui.lineEdit->text();}
};

#endif
