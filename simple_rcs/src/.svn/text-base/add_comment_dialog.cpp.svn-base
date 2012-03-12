#include "add_comment_dialog.h"
#include <QFileDialog>
#include <QHeaderView>

AddCommentDialog::AddCommentDialog(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	
	ui.list->header()->setResizeMode(QHeaderView::ResizeToContents);
	
	connect(ui.list, SIGNAL(clicked( const QModelIndex & )), this, SLOT(activated_version( const QModelIndex &) ) );
}

void AddCommentDialog::setModel(QAbstractListModel *model)
{
	ui.list->setModel(model);
}

void AddCommentDialog::activated_version( const QModelIndex & index )
{
	version=index.model()->index(index.row(),0).data().toString();
}
