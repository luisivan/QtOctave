/* Copyright (C) 2006 P.L. Lucas
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

#ifndef __TABLE_H__
#define __TABLE_H__

#include <QTableWidget>
#include <QContextMenuEvent>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include "octave_connection.h"
#include "ui_table_ui.h"
#include "basewidget.h"
#include "operations/table_plot.h"

using namespace Ui;

class ComplexNumberTableModel;

/** Shows QTable with spreadsheet. Spreadsheet's data are from given matrix.
When spreadsheet is modificated, matrix is modificated.
*/

class Table:public BaseWidget
{
	Q_OBJECT
	public:
	Table( QWidget * parent = 0 );
	void setOctaveConnection(OctaveConnection *octave_connection);
	void setMatrix(QString matrix);
	QString getMatrix();
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	void toXML(QXmlStreamWriter &xml);
	
	private:
	TableForm *table_form;
	OctaveConnection *octave_connection;
	QString matrix;
	QStringList dimensions;
	QList<QSpinBox*> spinbox_dimensions;
	void change_rows();
	void change_cols();
	void build_menu();
	QMenu *menu;
	ComplexNumberTableModel *model;
	
	void reloadCell(int row, int col);
	
	/**Returns matrix name and change : by row and col. Per example, if we are editing matrix a(1,2,:,2,:), matrix_row_col("2","2")==a(1,2,2,2,2). If ranges==true, then show 1:size(a)(#dimension) in other dimensions. For example,  matrix_row_col("2","2", true)==a(1:size(a)(1),1:size(a)(2),2,1:size(a)(4),2).
	% is changed for column number.
	*/
	QString matrix_row_col(QString row, QString col, bool ranges=false);
	/**Returns Octave command to resize matrix. If add==true, then rows or cols are added to dimensions. Negative rows or cols between brackets.*/
	QString resize_matrix(QString rows, QString cols, bool add=false);
	
	/**Regular expresions:*/
	
	QRegExp mre; //Used in line_ready
	
	/**Init most used regular expresions*/
	void init_regular_expresions();
	
	protected:
	void contextMenuEvent ( QContextMenuEvent * event );
	public slots:
	void windowActivated ( QWidget * w );
	void windowActivated();
	//Plot callbacks
	void plot(TablePlot::Type);
	void plotPlot();
	void plotPolar();
	void plotLogXandY();
	void plotLogY();
	void plotLogX();
	void plotBar();
	/** Callback to line_ready signal from OctaveConnection.
	 * @param line line from Octave.
	*/
	void line_ready(QString line);
	void cellChanged ( int row, int col, QString value );
	void rows_changed();
	void cols_changed();
	void order_changed();
	void copy_cb();
	void copy_matrix_cb();
	void paste_cb();
	void delete_rows_cb();
	void delete_columns_cb();
	void insert_column_right_cb();
	void insert_column_left_cb();
	void insert_row_down_cb();
	void insert_row_up_cb();
};

class ComplexNumberTableModel: public QAbstractTableModel
{
	Q_OBJECT
	private:
	double **real, **imag;
	int rows, cols;
	public:
	ComplexNumberTableModel(QObject *parent=0);
	~ComplexNumberTableModel();
	
	inline void setRawData(int row, int col, double real)
	{
		this->real[row][col]=real;
		this->imag[row][col]=0;
	}
	
	inline void setRawData(int row, int col, double real, double imag)
	{
		this->real[row][col]=real;
		this->imag[row][col]=imag;
	}
	
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
	Qt::ItemFlags flags ( const QModelIndex & index ) const;
	
	QString data(int row, int col) const;
	
	void resize( int _rows, int _columns);
	void update();
	void update(int row, int col) ;
	
	signals:
	void cellChanged( int, int, QString);
};

class LineEditDelegate : public QItemDelegate
{
	Q_OBJECT
	public:
	LineEditDelegate(QObject *parent = 0);
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
#ifdef __DELEGATE_WIDGET__
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
				const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
	protected:
	bool eventFilter ( QObject * object, QEvent * event );
#endif
};


#endif
