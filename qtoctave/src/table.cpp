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

#include "table.h"
#include <QTextStream>
#include <QRegExp>
#include <QMenu>
#include <QClipboard>
#include <QMessageBox>
#include <QFile>
#include <QMenuBar>

Table::Table(QWidget * parent):BaseWidget(parent)
{
	widget_type=TABLE;
	
	init_regular_expresions();

	table_form=new TableForm();
	table_form->setupUi(centralWidget());
	setWindowIcon( QIcon( QString( ICON_PATH )+"/table.png" ) );
	table_form->reloadButton->setIcon( QIcon( QString( ICON_PATH )+"/reload.png" ) );
	table_form->reloadButton->setToolTip(tr("<b>Reload matrix.</b><br> Some operations change matrix, use reload to view changes."));
	table_form->spinbox_widget->setLayout(new QHBoxLayout);
	table_form->spinbox_widget->layout()->addWidget(new QLabel("Indexes showed:"));
	table_form->spinbox_widget->layout()->setContentsMargins(0,0,0,0);
	
	model=new ComplexNumberTableModel;
	table_form->table_widget->setModel(model);
	table_form->table_widget->setItemDelegate( new LineEditDelegate(this) );
	
	connect(model,SIGNAL(cellChanged( int, int, QString)),this,SLOT(cellChanged( int, int, QString)));
	connect(table_form->rows_spinbox,SIGNAL( editingFinished() ),this,SLOT(rows_changed()));
	
	connect(table_form->cols_spinbox,SIGNAL( editingFinished ()),this,SLOT(cols_changed()));
	
	connect(table_form->changeOrderButton,SIGNAL( clicked() ),this,SLOT( rows_changed() ));
	connect(table_form->changeOrderButton,SIGNAL( clicked() ),this,SLOT( cols_changed() ));
	connect(table_form->changeOrderButton,SIGNAL( clicked() ),this,SLOT( windowActivated() ));
	
	connect(table_form->reloadButton,SIGNAL( clicked() ),this,SLOT( windowActivated() ));
	
	build_menu();
	setContextMenuPolicy ( Qt::DefaultContextMenu );
	menuBar()->addMenu(menu);
}

void Table::init_regular_expresions()
{
	mre.setPattern("~~matrix:([A-Za-z0-9_]+) \"([^\"]+)\" ([\\d ]+)"); //Used in line_ready
}

void Table::setOctaveConnection(OctaveConnection *octave_connection)
{
	this->octave_connection=octave_connection;
	connect(octave_connection,SIGNAL(line_ready(QString)),this,SLOT(line_ready(QString)));
}

void Table::setMatrix(QString matrix)
{
	QRegExp re("([A-Za-z0-9_]+)\\(((:|\\d+),)+(:|\\d+)\\)");
	matrix.remove(" ");
	matrix.remove("\t");
	matrix.remove("\n");
	setWindowTitle("Table: "+matrix);
	if(re.exactMatch(matrix))
	{
		this->matrix=re.cap(1);
		matrix.remove(this->matrix);
		QStringList dims;
		QRegExp re_dim("(:|\\d+)");
		int pos=0;
		while ((pos = re_dim.indexIn(matrix, pos)) != -1)
		{
			dims << re_dim.cap(0);
			pos += re_dim.matchedLength();
		}
		dimensions=dims;
		
		int count_doubledot=0;
		for(int i=0;i<dimensions.size();i++)
		{
			if(dimensions.at(i)==":")
			{
				count_doubledot++;
			}
		}
		if(count_doubledot!=2)
		{
			for(int i=0;i<dimensions.size();i++)
			{
				if(i<2)
					dimensions[i]=":";
				else
					dimensions[i]="1";
			}
		}
	}
	else
	{
		this->matrix=matrix;
		dimensions.clear();
	}
}

void Table::windowActivated()
{
	windowActivated(this);
}

void Table::windowActivated(QWidget *w)
{
	if(w!=this) return;
	
	QString command;
	
	command+=matrix+"(";
	for(int i=0;i<spinbox_dimensions.size();i++)
	{
		int value=spinbox_dimensions.at(i)->value();
		
		if( value==0 )
			command+=":";
		else
			command+=QString::number(value);
		
		if(i<(spinbox_dimensions.size()-1))
		{
			command+=",";
		}
	}
	command+=")";
	
	if(spinbox_dimensions.size()>0) setMatrix(command);
	
	command="";
	//Next line: If matrix doesn't exit, it will build it
	command+="eval(\""+matrix+";\",\""+matrix+"=[0]\");";
	command+="_ide_reload_matrix("+matrix+","+QString::number((long)this)+",";
	if(dimensions.size()==0)
	{
			command+="resize("+matrix+"(:,:),size("+matrix+")(1),size("+matrix+")(2) ) ";
	}
	else
	{
		int x[2]; x[0]=x[1]=-1;
		QString sub_matrix=matrix+"(";
		for(int i=0;i<dimensions.size();i++)
		{
			if(dimensions[i]==":")
			{
				if(x[0]==-1)
					x[0]=i+1;
				else
					x[1]=i+1;
			}
			if(i<(dimensions.size()-1))
				sub_matrix+=dimensions[i]+",";
			else
				sub_matrix+=dimensions[i]+")";
		}
			
			
		command+="reshape("+sub_matrix+",size("+matrix+")("+QString::number(x[0])+"),size("+matrix+")("+QString::number(x[1])+") ) ";
			
	}
	
	command+=", \""+matrix+"\" );";

	//printf("%s",command.toLocal8Bit().data() );
	
	octave_connection->command_enter(command,false);
}


void Table::reloadCell(int row, int col)
{
	QString command;
		
	command+="fprintf(stderr,\"~~matrixCell:"+matrix+" %d %d "+QString::number((long)this)+" %g %g\\n\", "
	+QString::number(row+1)+", "+QString::number(col+1)+", ";
	
	if(dimensions.size()==0)
	{
		command+= "real("+matrix+"("+QString::number(row+1)+","+QString::number(col+1)+")), "
		"imag("+matrix+"("+QString::number(row+1)+","+QString::number(col+1)+")));";
	}
	else
	{
		QString aux=matrix_row_col(QString::number(row+1), QString::number(col+1));
		
		command+= "real("+aux+"), imag("+aux+"));";
	}
	
	octave_connection->command_enter(command,false);
}


#include <QTime>

void Table::line_ready(QString line)
{
	if(line.startsWith("~~matrixCell:"+matrix))
	{
		line=line.right(line.length() - (13+matrix.length()) ).trimmed();
		//printf("line:>%s<\n",line.toLocal8Bit().data());
		
		double real, imag;
		int row, col;
		long pointer;
		sscanf(line.toLocal8Bit().data(), "%d %d %ld %lf %lf",&row, &col, &pointer, &real, &imag);
		
		if(((long)this)!=pointer) return;
		
		model->setRawData(row-1,col-1, real, imag);
		//model->update();
		model->update(row-1,col-1);
		
		return;
	}
	else if( ! line.startsWith("~~matrix:"+matrix) ) return;
	
	//Read line-event data:
	//~~matrix:matrix-name "file_name" rows columns
	
	QTime time;
	
	time.start();
	
	line=line.trimmed();
	
	int _rows=-1, _columns=-1;
	QString file_name;
	QList<int> sizes;
	
	//QRegExp mre("~~matrix:([A-Za-z0-9_]+) \"([^\"]+)\" ([\\d ]+)");
	
	mre.indexIn ( line ) ;
	{
		QStringList list = mre.capturedTexts();
		//for(int i=0;i<list.size();i++)
		//	printf("Texto(%d): %s\n", i, list.at(i).toLocal8Bit().data());
		QStringList numbers=list.at(3).trimmed().split(" ", QString::SkipEmptyParts);
		
		long thisValue=numbers.at(0).toLong();
		if( ((long)this)!=thisValue )
                {
		   qDebug("PROBLEM: value mismatch; %ld != %ld\n", (long)this, thisValue);
		   return;
                }
                //printf("[Table::line_ready] this id=%ld table id=%ld\n", (long)this, thisValue);

                numbers.removeFirst();
		
		for(int i=0;i<numbers.size();i++)
		{
			sizes.append(numbers.at(i).toInt());
			printf("[Table::line_ready] sizes=%d\n", numbers.at(i).toInt());
		}
		//if( ((long)this)!=sizes.at(0) ) return;
		
		file_name=list.at(2);

		//Already removed in numbers.removeFirst()
		//sizes.removeFirst();
		
		int x[2]; x[0]=x[1]=-1;
		for(int i=0;i<dimensions.size();i++)
		{
			if(dimensions[i]==":")
			{
				if(x[0]==-1)
					x[0]=i;
				else
					x[1]=i;
			}
		}
		
		if(x[0]==-1 || x[1]==-1)
		{
			x[0]=0; x[1]=1;
		}
		_rows=sizes.at(x[0]);
		_columns=sizes.at(x[1]);
		model->resize(_rows,_columns);
		table_form->rows_spinbox->setValue(_rows);
		table_form->cols_spinbox->setValue(_columns);
		//printf("Table: %s %d %d\n", file_name.toLocal8Bit().data(), _rows, _columns);
	}
	
	{
		QFile file(file_name);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
		
		//disconnect(model,SIGNAL(cellChanged( int, int)),this,SLOT(cellChanged( int, int, QString)));
		
		int col=0, row=0;
		char ch;
		QString number, real, imag;
		number.reserve(64);
		real.reserve(32);
		imag.reserve(32);
		bool data_available_ok=false;
		
		while (!file.atEnd())
		{
			if(!file.getChar(&ch)) break;

			switch(ch)
			{
				case ' ':
				if( !number.isEmpty() )
				{
					real=number;
					data_available_ok=true;
				}
				number.clear();
				break;
				
				case ',':
				real=number;
				number.clear();
				break;
				
				case ')':
				imag=number;
				number.clear();
				data_available_ok=true;
				break;
				
				case '(':
				real.clear();
				imag.clear();
				number.clear();
				break;
				
				case '\n':
				if( !number.isEmpty() )
				{
					real=number;
					data_available_ok=true;
				}
				else {row++;col=0;}
				//real.clear();
				//imag.clear();
				number.clear();
				break;
				
				case '#':
					if(!file.getChar(&ch)) break;
					while(ch!='\n') if(!file.getChar(&ch)) break;
				continue;
				break;
				
				default:
				number.append(ch);
			};
			
			if( data_available_ok )
			{
				//printf("(%s,%s)\n", real.toLocal8Bit().data(), imag.toLocal8Bit().data());
				if( col<model->columnCount() && row<model->rowCount() )
				{
					if( imag.isEmpty() ) model->setRawData(row,col, real.toDouble() );
					else model->setRawData(row,col, real.toDouble(), imag.toDouble() );
				}
				col++;
				if(ch=='\n') {row++;col=0;}
				data_available_ok=false;
				real.clear();
				imag.clear();
			}
		}
		
		file.close();
		
		file.remove();
		
		//connect(model,SIGNAL(cellChanged( int, int, QString)),this,SLOT(cellChanged( int, int, QString)));
		
		printf("[Table::line_ready] Data loaded in %dms\n", time.elapsed() );
		fflush(stdout);
		
		model->update();
	}
	
	//Builds SpinBoxes to show
	{
		while(sizes.size() < spinbox_dimensions.size())
		{
			spinbox_dimensions.last()->close();
			spinbox_dimensions.removeLast();
		}
		
		while(sizes.size() > spinbox_dimensions.size())
		{
			QSpinBox *spinbox=new QSpinBox(table_form->spinbox_widget);
			spinbox->setAttribute(Qt::WA_DeleteOnClose);
			spinbox->setMinimum(0);
			spinbox->setSpecialValueText(":");
			spinbox_dimensions.append(spinbox);
			table_form->spinbox_widget->layout()->addWidget(spinbox);
		}
		
		for(int i=0;i<sizes.size();i++)
		{
			QSpinBox *spinbox=spinbox_dimensions.at(i);
			spinbox->setMaximum(sizes.at(i));
			
			if( (dimensions.size()==0 && i<2) || (dimensions.size()>i && dimensions.at(i)==":") )
				spinbox->setValue(0);
			else if(dimensions.size()>i)
				spinbox->setValue(dimensions.at(i).toInt());
			else
				spinbox->setValue(1);
		}
	}
}

void Table::cellChanged ( int row, int col, QString value )
{
	QString command(matrix_row_col(QString::number(row+1),QString::number(col+1)));
	
	command+="="+value+";";
	
	octave_connection->command_enter(command);
	
	reloadCell(row, col);
}

void Table::change_rows()
{
	int rows=table_form->rows_spinbox->value();
	int old_rows=model->rowCount();
	int old_cols=model->columnCount();
	
	if(rows<old_rows)
	{
		QString command( matrix + "=" );
		if(dimensions.size()==0)
		{
			command+=matrix_row_col("1:"+QString::number(rows), "1:"+QString::number(old_cols))+";";
		}
		else
		{
			bool row_ok=true;
			command+=matrix+"(";
			for(int i=0;i<dimensions.size();i++)
			{
				if(dimensions[i]==":")
				{
					if(row_ok)
					{
						row_ok=false;
						command+="1:"+QString::number(rows);
					}
					else
					{
						command+="1:"+QString::number(old_cols);
					}
				}
				else
				{
					command+="1:size("+matrix+")("+QString::number(i+1)+")";
				}
				
				if(i<(dimensions.size()-1)) command+=",";
				else command+=");";
			}
		}
		octave_connection->command_enter(command);
		return;
	}
	else if(rows>old_rows)
	{
		QString command;
		command+=matrix_row_col(QString::number(rows),QString::number(old_cols))+"=0;";
		octave_connection->command_enter(command);
	}
}

void Table::rows_changed()
{
	change_rows();
	windowActivated(this);
}

void Table::cols_changed()
{
	change_cols();
	windowActivated(this);
}

void Table::change_cols()
{
	int cols=table_form->cols_spinbox->value();
	int old_rows=model->rowCount();
	int old_cols=model->columnCount();
	
	if(cols<old_cols)
	{
		QString command( matrix + "=" );
		if(dimensions.size()==0)
		{
			command+=matrix_row_col("1:"+QString::number(old_rows), "1:"+QString::number(cols))+";";
		}
		else
		{
			bool row_ok=true;
			command+=matrix+"(";
			for(int i=0;i<dimensions.size();i++)
			{
				if(dimensions[i]==":")
				{
					if(row_ok)
					{
						row_ok=false;
						command+="1:"+QString::number(old_rows);
					}
					else
					{
						command+="1:"+QString::number(cols);
					}
				}
				else
				{
					command+="1:size("+matrix+")("+QString::number(i+1)+")";
				}
				
				if(i<(dimensions.size()-1)) command+=",";
				else command+=");";
			}
		}
		octave_connection->command_enter(command);
		return;
	}
	else if(cols>old_cols)
	{
		QString command;
		command+=matrix_row_col(QString::number(old_rows),QString::number(cols))+"=0;";
		octave_connection->command_enter(command);
	}
}

void Table::order_changed()
{
	change_rows();
	change_cols();
	windowActivated(this);
}

QString Table::getMatrix()
{
	return matrix;
}

void Table::build_menu()
{
	menu=new QMenu(tr("Table"), this);
	QAction *copyAction=new QAction(tr("Copy"), menu);
	menu->addAction(copyAction);
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy_cb()) );
	QAction *copyMatrixAction=new QAction(tr("Copy as Octave matrix"), menu);
	menu->addAction(copyMatrixAction);
	connect(copyMatrixAction, SIGNAL(triggered()), this, SLOT(copy_matrix_cb()) );
	QAction *pasteAction=new QAction(tr("Paste"), menu);
	menu->addAction(pasteAction);
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste_cb()) );
	QAction *deleteRowsAction=new QAction(tr("Delete rows"), menu);
	menu->addAction(deleteRowsAction);
	connect(deleteRowsAction, SIGNAL(triggered()), this, SLOT(delete_rows_cb()) );
	QAction *deleteColumnsAction=new QAction(tr("Delete columns"), menu);
	menu->addAction(deleteColumnsAction);
	connect(deleteColumnsAction, SIGNAL(triggered()), this, SLOT(delete_columns_cb()) );
	QAction *insertColumnRightAction=new QAction("Insert column (right)", menu);
	menu->addAction(insertColumnRightAction);
	connect(insertColumnRightAction, SIGNAL(triggered()), this, SLOT(insert_column_right_cb()) );
	QAction *insertColumnLeftAction=new QAction("Insert column (left)", menu);
	menu->addAction(insertColumnLeftAction);
	connect(insertColumnLeftAction, SIGNAL(triggered()), this, SLOT(insert_column_left_cb()) );
	QAction *insertRowUpAction=new QAction("Insert row (up)", menu);
	menu->addAction(insertRowUpAction);
	connect(insertRowUpAction, SIGNAL(triggered()), this, SLOT(insert_row_up_cb()) );
	QAction *insertRowDownAction=new QAction("Insert row (down)", menu);
	menu->addAction(insertRowDownAction);
	connect(insertRowDownAction, SIGNAL(triggered()), this, SLOT(insert_row_down_cb()) );

	/* Gráficas */
	menu->addSeparator();

	QMenu *plotMenu = new QMenu(tr("Plot"), menu);
	menu->addMenu(plotMenu);
	QMenu *plot2dMenu = new QMenu(tr("2D"), plotMenu);
	plotMenu->addMenu(plot2dMenu);

	QAction *plotAction = new QAction(tr("Plot"), plot2dMenu);
	plot2dMenu->addAction(plotAction);
	connect(plotAction, SIGNAL(triggered()), this, SLOT(plotPlot()));

	QAction *polarAction = new QAction(tr("Polar"), plot2dMenu);
	plot2dMenu->addAction(polarAction);
	connect(polarAction, SIGNAL(triggered()), this, SLOT(plotPolar()));

	QAction *logxyAction = new QAction(tr("Log scale for the x and y axis"), plot2dMenu);
	plot2dMenu->addAction(logxyAction);
	connect(logxyAction, SIGNAL(triggered()), this, SLOT(plotLogXandY()));

	QAction *logyAction = new QAction(tr("Log scale for the y axis"), plot2dMenu);
	plot2dMenu->addAction(logyAction);
	connect(logyAction, SIGNAL(triggered()), this, SLOT(plotLogY()));

	QAction *logxAction = new QAction(tr("Log scale for the x axis"), plot2dMenu);
	plot2dMenu->addAction(logxAction);
	connect(logxAction, SIGNAL(triggered()), this, SLOT(plotLogX()));

	QAction *barAction = new QAction(tr("Bar graph"), plot2dMenu);
	plot2dMenu->addAction(barAction);
	connect(barAction, SIGNAL(triggered()), this, SLOT(plotBar()));
}

void Table::insert_row_up_cb()
{
	QString command;
	
	int row=table_form->table_widget->currentIndex().row()+1;
	
	command=matrix+"="+resize_matrix("1","0", true)+";\n";
	
	command+=matrix_row_col(QString::number(row+1)+":size("+matrix+")(%)","1:size("+matrix+")(%)",true)
		+"="+matrix_row_col(QString::number(row)+":(size("+matrix+")(%)-1)","1:size("+matrix+")(%)",true)+"\n";
	command+=matrix_row_col(QString::number(row),"1:size("+matrix+")(%)",true)
		+"=0\n";
	
	octave_connection->command_enter(command);
	
	windowActivated();
}

void Table::insert_row_down_cb()
{
	QString command;
	
	int row=table_form->table_widget->currentIndex().row()+1;
	
	command=matrix+"="+resize_matrix("1","0", true)+";\n";
	
	command+=matrix_row_col(QString::number(row+2)+":size("+matrix+")(%)","1:size("+matrix+")(%)",true)
		+"="+matrix_row_col(QString::number(row+1)+":(size("+matrix+")(%)-1)","1:size("+matrix+")(%)",true)+"\n";
	command+=matrix_row_col(QString::number(row+1),"1:size("+matrix+")(%)",true)
		+"=0\n";
	
	octave_connection->command_enter(command);
	
	windowActivated();
}

void Table::insert_column_left_cb()
{
	QString command;

	int col=table_form->table_widget->currentIndex().column()+1;
	
	command=matrix+"="+resize_matrix("0","1", true)+";\n";
	
	command+=matrix_row_col("1:size("+matrix+")(%)",QString::number(col+1)+":size("+matrix+")(%)",true)
		+"="+matrix_row_col("1:size("+matrix+")(%)",QString::number(col)+":(size("+matrix+")(%)-1)",true)+"\n";
	command+=matrix_row_col("1:size("+matrix+")(%)",QString::number(col),true)
		+"=0\n";
	
	octave_connection->command_enter(command);
	
	windowActivated();
}

void Table::insert_column_right_cb()
{
	QString command;
	
	int col=table_form->table_widget->currentIndex().column()+1;
	
	command=matrix+"="+resize_matrix("0","1", true)+";\n";
	
	command+=matrix_row_col("1:size("+matrix+")(%)",QString::number(col+2)+":size("+matrix+")(%)",true)
		+"="+matrix_row_col("1:size("+matrix+")(%)",QString::number(col+1)+":(size("+matrix+")(%)-1)",true)+"\n";
	command+=matrix_row_col("1:size("+matrix+")(%)",QString::number(col+1),true)
		+"=0\n";
	
	octave_connection->command_enter(command);
	
	windowActivated();
}

void Table::delete_columns_cb()
{
	int result=QMessageBox::question (this, tr("Warning"), tr("Comlumns will be deleted. Continue?"), QMessageBox::Ok, QMessageBox::Cancel);
	
	if(result!=QMessageBox::Ok) return;
	
	int col=table_form->table_widget->currentIndex().column()+1;
	
	QString command;
	
	command+=matrix_row_col("1:size("+matrix+")(%)",QString::number(col)+":(size("+matrix+")(%)-1)",true)
		+"="+matrix_row_col("1:size("+matrix+")(%)",QString::number(col+1)+":(size("+matrix+")(%))",true)+";\n";
	
	command+=matrix+"="+resize_matrix("0","(-1)", true)+";\n";
	
	
	octave_connection->command_enter(command);
	
	windowActivated();
}


void Table::delete_rows_cb()
{
	int result=QMessageBox::question (this, tr("Warning"), tr("Rows will be deleted. Continue?"), QMessageBox::Ok, QMessageBox::Cancel);
	
	if(result!=QMessageBox::Ok) return;
	
	int row=table_form->table_widget->currentIndex().row()+1;
	
	QString command;
	
	command+=matrix_row_col(QString::number(row)+":size("+matrix+")(%)-1","1:size("+matrix+")(%)",true)
		+"="+matrix_row_col(QString::number(row+1)+":(size("+matrix+")(%))","1:size("+matrix+")(%)",true)+";\n";
		
	command+=matrix+"="+resize_matrix("(-1)","0", true)+";\n";
	
	octave_connection->command_enter(command);
	
	windowActivated();
}

void Table::paste_cb()
{
	QClipboard *clipboard = QApplication::clipboard();
	QString text=clipboard->text();
	
	QString _command;
	QTextStream command(&_command);
	
	int row=table_form->table_widget->currentIndex().row()+1, col=table_form->table_widget->currentIndex().column()+1;
	
	QRegExp rx("([0-9ieEj\\.\\-\\+]+|\\n)");
	QString value;
	int pos = 0;
	while ((pos = rx.indexIn(text, pos)) != -1) {
		value=rx.cap(0);
		pos += rx.matchedLength();
		if(value=="\n") {row++;col=table_form->table_widget->currentIndex().column()+1;}
		else
		{
			command << matrix_row_col(QString::number(row),QString::number(col)) << "=" << value << ";\n";
			col++;
		}
	}
	octave_connection->command_enter(_command);
	
	windowActivated();
}

void Table::copy_matrix_cb()
{
	QString str("[");
	QModelIndexList ranges=table_form->table_widget->selectionModel ()->selectedIndexes ();
	
	QMap<int, QMap<int,QString> > values;
	
	for(int i=0;i<ranges.size();i++)
	{
		values[ranges.at(i).row()][ranges.at(i).column()] = model->data(ranges.at(i)).toString();
	}
	
	QList<int> rows=values.keys();
	for(int i=0;i<rows.size();i++)
	{
		QList<int> cols=values[rows.at(i)].keys();
		str+=values[rows.at(i)][cols.at(0)];
		for(int j=1;j<cols.size();j++)
		{
			str+=", "+values[rows.at(i)][cols.at(j)];
		}
		str+=";\n";
	}
	
	str+="]";
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(str);
}

void Table::copy_cb()
{
	QString str;
	QModelIndexList ranges=table_form->table_widget->selectionModel ()->selectedIndexes ();
	
	QMap<int, QMap<int,QString> > values;
	
	for(int i=0;i<ranges.size();i++)
	{
		values[ranges.at(i).row()][ranges.at(i).column()] = model->data(ranges.at(i)).toString();
	}
	
	QList<int> rows=values.keys();
	for(int i=0;i<rows.size();i++)
	{
		QList<int> cols=values[rows.at(i)].keys();
		str+=values[rows.at(i)][cols.at(0)];
		for(int j=1;j<cols.size();j++)
		{
			str+=" "+values[rows.at(i)][cols.at(j)];
		}
		str+="\n";
	}

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(str);
}

void Table::contextMenuEvent ( QContextMenuEvent * event )
{
	QPoint p(event->globalX(),event->globalY());
	menu->popup(p);
}

void Table::plot(TablePlot::Type type)
{
  TablePlot *dialog = new TablePlot(this, table_form->table_widget, model, type);
  
  if(dialog->exec() == QDialog::Accepted)
  {
    try
    {
      this->octave_connection->command_enter(dialog->command());
    }catch(const char *str)
    {
      //QMessageBox errorMsg(QMessageBox::Warning, "Error", str, QMessageBox::Ok, this);
      //errorMsg.exec();
      
      QMessageBox::warning (NULL, tr("Error"), str);
    }
  }

  delete dialog;
}

void Table::plotPlot()
{
  plot(TablePlot::PLOT);
}

void Table::plotPolar()
{
  plot(TablePlot::POLAR);
}

void Table::plotLogXandY()
{
  plot(TablePlot::LOGLOG);
}

void Table::plotLogY()
{
  plot(TablePlot::SEMILOGY);
}

void Table::plotLogX()
{
  plot(TablePlot::SEMILOGX);
}

void Table::plotBar()
{
  plot(TablePlot::BAR);
}


BaseWidget *Table::copyBaseWidget(QWidget * parent )
{
	Table *bw=new Table(parent);
	bw->setSession(session);
	bw->setOctaveConnection(octave_connection);
	bw->setMatrix( matrix_row_col(":",":") );
	
	return bw;
}

void Table::toXML(QXmlStreamWriter &xml)
{
	xml.writeStartElement("matrix");
	
	xml.writeAttribute("value", matrix_row_col(":",":") );
	
	xml.writeEndElement();
}

QString Table::matrix_row_col(QString row, QString col, bool ranges)
{
	
	bool row_ok=true;
	QString command(matrix+"(");
	
	if(dimensions.size()==0)
	{
		row.replace("%", QString::number(1));
		col.replace("%", QString::number(2));
		command+=row+","+col+")";
		return command;	
	}
	
	for(int i=0;i<dimensions.size();i++)
	{
		if(dimensions[i]==":")
		{
			if(row_ok)
			{
				row_ok=false;
				row.replace("%", QString::number(i+1));
				command+=row;
			}
			else
			{
				col.replace("%", QString::number(i+1));
				command+=col;
			}
		}
		else if(ranges)
		{
			command+="1:size("+matrix+")("+QString::number(i+1)+")";
		}
		else
		{
			command+=dimensions[i];
		}
		
		if(i<(dimensions.size()-1)) command+=",";
		else command+=")";
	}
	
	return command;
}

QString Table::resize_matrix(QString rows, QString cols, bool add)
{
	QString command("resize(");
	command+=matrix+",[";
	bool row_ok=true;
	
	for(int i=0;i<dimensions.size();i++)
	{
		if(dimensions[i]==":")
		{
			if(add)
			{
				command+="size("+matrix+")("+QString::number(i+1)+")+";
			}
			
			if(row_ok)
			{
				command+=rows;
				row_ok=false;
			}
			else
			{
				command+=cols;
			}
		}
		else
		{
			command+="size("+matrix+")("+QString::number(i+1)+")";
		}
		
		if(i<(dimensions.size()-1)) command+=",";
		else command+="])";
	}
	
	if(dimensions.size()==0)
	{
		if(add)
		{
			command+="size("+matrix+")(1)+"+rows+","+"size("+matrix+")(2)+"+cols+"])";
		}
		else
		{
			command+=rows+","+cols+"])";
		}
	}
	
	return command;
}


////////////////////////////////////////////////////////////////////////////////

ComplexNumberTableModel::ComplexNumberTableModel(QObject *parent):QAbstractTableModel(parent)
{
	rows=cols=0;
	real=imag=NULL;
}

////////////////////////////////////////////////////////////////////////////////

int ComplexNumberTableModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
	return rows;
}

////////////////////////////////////////////////////////////////////////////////

int ComplexNumberTableModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
	return cols;
}

////////////////////////////////////////////////////////////////////////////////

QVariant ComplexNumberTableModel::data ( const QModelIndex & index, int role ) const
{
	if (!index.isValid())
		return QVariant();
	
	if (index.row() > rows || index.column() > cols)
		return QVariant();
	
	if (role == Qt::DisplayRole)
	{
		int row=index.row(), col=index.column();
		
		return data(row,col);
	}
	else
		return QVariant();
}

////////////////////////////////////////////////////////////////////////////////

QString ComplexNumberTableModel::data(int row, int col) const
{
	QString value = QString::number(real[row][col]);
	if(imag[row][col]!=0) 
	{
		if(imag[row][col]>0) value+="+"+QString::number(imag[row][col])+"i";
		else value+=QString::number(imag[row][col])+"i";
	}
	return value;
}

////////////////////////////////////////////////////////////////////////////////

void ComplexNumberTableModel::resize( int _rows, int _columns)
{
	if(_rows==rows && _columns==cols) 
	{
		update();
		return;
	}
	
	if(real!=NULL)
	{
		for(int i=0;i<rows;i++) delete [] real[i];
		delete [] real;
	}
	if(imag!=NULL)
	{
		for(int i=0;i<rows;i++) delete [] imag[i];
		delete [] imag;
	}
	
	if(rows>0)
	{
		beginRemoveRows(QModelIndex(), 0, rows-1);
		endRemoveRows();
	}
	if(cols>0)
	{
		beginRemoveColumns(QModelIndex(), 0, cols-1);
		endRemoveColumns();
	}
	
	rows=_rows; cols=_columns;
	
	real=new double*[rows];
	imag=new double*[rows];
	
	for(int i=0;i<rows;i++)
	{
		real[i]=new double[cols]; imag[i]=new double[cols];
	}
	
	if(rows>0)
	{
		beginInsertRows(QModelIndex(), 0, rows-1);
		endInsertRows();
	}
	if(cols>0)
	{
		beginInsertColumns(QModelIndex(), 0, cols-1);
		endInsertColumns();
	}
}

////////////////////////////////////////////////////////////////////////////////

ComplexNumberTableModel::~ComplexNumberTableModel()
{
	if(real!=NULL)
	{
		for(int i=0;i<rows;i++) delete [] real[i];
		delete [] real;
	}
	if(imag!=NULL)
	{
		for(int i=0;i<rows;i++) delete [] imag[i];
		delete [] imag;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool ComplexNumberTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	if (index.isValid() && role == Qt::EditRole)
	{
		emit dataChanged(index,index);
		emit cellChanged(index.row(), index.column(), value.toString() );
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags ComplexNumberTableModel::flags ( const QModelIndex & /*index*/ ) const
{
	return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

////////////////////////////////////////////////////////////////////////////////

void ComplexNumberTableModel::update()
{
	//beginInsertRows(QModelIndex(), 0, rows-1);
	//endInsertRows();
	//beginInsertColumns(QModelIndex(), 0, rows-1);
	//endInsertColumns();
	QModelIndex cell0=index(0, 0), cell1;
	int n_rows=rows-1, n_cols=cols-1;
	if(n_rows<0) n_rows=0;
	if(n_cols<0) n_cols=0;
	cell1=index(n_rows, n_cols);
	emit dataChanged(cell0,cell1);
}

////////////////////////////////////////////////////////////////////////////////

void ComplexNumberTableModel::update(int row, int col) 
{
	QModelIndex cell=index(row, col);
	emit dataChanged(cell,cell);
}

////////////////////////////////////////////////////////////////////////////////

LineEditDelegate::LineEditDelegate(QObject *parent ):QItemDelegate(parent)
{
	
}

////////////////////////////////////////////////////////////////////////////////

void LineEditDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();
	
	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
	lineedit->setText(value);
}

#ifdef __DELEGATE_WIDGET__

////////////////////////////////////////////////////////////////////////////////

QWidget *LineEditDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &/* option */,
        const QModelIndex & index) const
{
	QLineEdit *editor = new QLineEdit(parent);
	editor->installEventFilter(const_cast<LineEditDelegate*>(this));

	return editor;
}



////////////////////////////////////////////////////////////////////////////////

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const
{
	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
	
	QString value = lineedit->text();
	
	model->setData(index, value);
}

////////////////////////////////////////////////////////////////////////////////

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

////////////////////////////////////////////////////////////////////////////////

bool LineEditDelegate::eventFilter( QObject * object, QEvent * event )
{
	if(event->type()==QEvent::KeyPress)
	{
		QKeyEvent *ekey=(QKeyEvent*)event;
		QWidget *editor=static_cast<QWidget*>(object);
		switch(ekey->key())
		{
			case Qt::Key_Return:
			case Qt::Key_Enter:
				emit commitData(editor);
			case Qt::Key_Escape:
				emit  closeEditor(editor,NoHint);
				event->setAccepted(true);
				return true;
			break;
		}
	}
	
	return false;
}

#endif
