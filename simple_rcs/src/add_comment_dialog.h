#ifndef __ADDCOMMENTDIALOG_H__
#define __ADDCOMMENTDIALOG_H__

#include <QDialog>

#include "ui_add_comment.h"


using namespace Ui;

class AddCommentDialog:public QDialog
{
	Q_OBJECT

	UiAddCommentDialog ui;
	QString version;

	public:
	AddCommentDialog(QWidget *parent = 0);
	
	void setModel(QAbstractListModel *model);
	
	public slots:
	
	void activated_version( const QModelIndex & index );
	inline QString getVersion() {return version;}
	inline QString getComment() {return ui.lineEdit->text();}
};

#endif
