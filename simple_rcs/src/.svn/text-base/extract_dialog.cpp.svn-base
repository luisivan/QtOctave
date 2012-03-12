#include "extract_dialog.h"
#include <QFileDialog>
#include <QHeaderView>

ExtractDialog::ExtractDialog(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	
	ui.list->header()->setResizeMode(QHeaderView::ResizeToContents);
	
	connect(ui.select_pushButton, SIGNAL(clicked()), this, SLOT(select_callback()) );
	connect(ui.list, SIGNAL(clicked( const QModelIndex & )), this, SLOT(activated_version( const QModelIndex &) ) );
}

void ExtractDialog::select_callback()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),ui.lineEdit->text());
	
	if(fileName.isEmpty()) return;
	
	ui.lineEdit->setText(fileName);
}

void ExtractDialog::setModel(QAbstractListModel *model)
{
	ui.list->setModel(model);
}

void ExtractDialog::activated_version( const QModelIndex & index )
{
	version=index.model()->index(index.row(),0).data().toString();
}

