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

#include "../operations.h"
#include "../editor.h"
#include "../projects/projects.h"
#include <QInputDialog>
#include <QTextStream>
#include <QMdiArea>
#include <QPushButton>
#include <QClipboard>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QPointer>
#include <QDockWidget>

Operations::Operations(QObject *parent, BaseWidget **active_widget, MainWindow *main_window):QObject(parent)
{
	this->active_widget=active_widget;
	this->main_window=main_window;

	//File actions
	connect(main_window->actions.value("exit"), SIGNAL(triggered()), this, SLOT(exit_callback()));
	connect(main_window->actions.value("cd"), SIGNAL(triggered()), this, SLOT(cd_callback()));
	
	//Matrix signals
	connect(main_window->actions.value("+"), SIGNAL(triggered()), this, SLOT(suma_callback()));
	connect(main_window->actions.value("*"), SIGNAL(triggered()), this, SLOT(producto_callback()));
	connect(main_window->actions.value("**"), SIGNAL(triggered()), this, SLOT(exponente_callback()));
	connect(main_window->actionInverse, SIGNAL(triggered()), this, SLOT(inverse_callback()));
	connect(main_window->actionDeterminant, SIGNAL(triggered()), this, SLOT(determinant_callback()));
	connect(main_window->actionEigenvalues, SIGNAL(triggered()), this, SLOT(eigenvalues_callback()));
	connect(main_window->actionTranspose, SIGNAL(triggered()), this, SLOT(transpose_callback()));
	connect(main_window->actionSubmatrix, SIGNAL(triggered()), this, SLOT(submatrix_callback()));
	
	//Statistics signals
	connect(main_window->actionMean, SIGNAL(triggered()), this, SLOT(mean_callback()));
	connect(main_window->actionMedian, SIGNAL(triggered()), this, SLOT(median_callback()));
	connect(main_window->actionStd, SIGNAL(triggered()), this, SLOT(std_callback()));
	connect(main_window->actionCov, SIGNAL(triggered()), this, SLOT(cov_callback()));
	connect(main_window->actionCorrcoef, SIGNAL(triggered()), this, SLOT(corrcoef_callback()));
	
	//Plot signals
	connect(main_window->actionPlot, SIGNAL(triggered()), this, SLOT(plot_callback()));
	connect(main_window->actionSemilogy, SIGNAL(triggered()), this, SLOT(semilogy_callback()));
	connect(main_window->actionSemilogx, SIGNAL(triggered()), this, SLOT(semilogx_callback()));
	connect(main_window->actionLogLog, SIGNAL(triggered()), this, SLOT(log_log_callback()));
	connect(main_window->actionPolar, SIGNAL(triggered()), this, SLOT(polar_callback()));
	connect(main_window->actions.value("errorbar"), SIGNAL(triggered()), this, SLOT(error_bars_callback()));
	connect(main_window->actions.value("hist"), SIGNAL(triggered()), this, SLOT(hist_callback()));
	connect(main_window->actions.value("bar"), SIGNAL(triggered()), this, SLOT(bar_callback()));

	connect(main_window->actions.value("contour"), SIGNAL(triggered()), this, SLOT(contour_callback()));
	connect(main_window->actions.value("mesh"), SIGNAL(triggered()), this, SLOT(mesh_callback()));
	
	connect(main_window->actionAxis, SIGNAL(triggered()), this, SLOT(axis_callback()));
	connect(main_window->actionTitleLabel, SIGNAL(triggered()), this, SLOT(title_label_callback()));
	//connect(main_window->actionToEPS, SIGNAL(triggered()), this, SLOT(to_eps_callback()));
	//connect(main_window->actionToPDF, SIGNAL(triggered()), this, SLOT(to_pdf_callback()));
	//connect(main_window->actionToPNG, SIGNAL(triggered()), this, SLOT(to_png_callback()));
	
	//General signals
	connect(main_window->actionProjects, SIGNAL(triggered()), this, SLOT(show_projects_dialog()));
	
	//Config signals
	connect(main_window->actionGeneralConfig, SIGNAL(triggered()), this, SLOT(general_config_callback()));
	connect(main_window->actions.value("qtoctave_pkg"), SIGNAL(triggered()), this, SLOT(qtoctave_pkg_callback()));

}

void Operations::setOctaveConnection(OctaveConnection *octave_connection)
{
	this->octave_connection=octave_connection;
}

void Operations::setMatrix(QString matrix)
{
	this->matrix=matrix;
}

void Operations::inverse_callback()
{
	/*
	BaseWidget *top_widget=*active_widget;
	QString window_name=top_widget->windowTitle();
	
	if(top_widget->widgetType()==TABLE)
	{
		QString aux;
		QTextStream(&window_name) >> aux >> matrix;
	}
	*/
	
	bool ok;
	
	ok=input_dialog("Inverse", "Matrix name:", matrix);
	
	if (ok && !matrix.isEmpty())
	{
		QString command;
		QTextStream(&command) << "inv(" << matrix << ")";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		//else if(window_name.startsWith("Octave Terminal") || window_name.startsWith("Help"))
		else
		{
			octave_connection->command_enter(command);
		}
		//else if(window_name.startsWith("Table:"))
		//{
		//	octave_connection->command_enter(matrix+"="+command);
		//	((Table*)top_widget)->windowActivated(top_widget);
		//}
	}
}

void Operations::determinant_callback()
{
	/*
	BaseWidget *top_widget=*active_widget;
	QString window_name=top_widget->windowTitle();
	
	if(top_widget->widgetType()==TABLE)
	{
		QString aux;
		QTextStream(&window_name) >> aux >> matrix;
	}
	*/
	
	bool ok;
	
	ok=input_dialog("Determinant", "Matrix name:", matrix);
	
	if (ok && !matrix.isEmpty())
	{
		QString command;
		QTextStream(&command) << "det(" << matrix << ")";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		else octave_connection->command_enter(command);
	}
}

void Operations::eigenvalues_callback()
{
	/*
	BaseWidget *top_widget=*active_widget;
	QString window_name=top_widget->windowTitle();
	
	if(top_widget->widgetType()==TABLE)
	{
		QString aux;
		QTextStream(&window_name) >> aux >> matrix;
	}
	*/
	
	copy_clipboard_ok=false;
	
	//Se muestra el cuadro de di�ogo.
	QDialog dialog(main_window);
	dialog.setWindowTitle("Eigenvalues and eigenvectors");
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QLabel matrix_label("Matrix name:",&dialog);
	layout.addWidget(&matrix_label);
	matrix_label.show();
	
	QLineEdit matrix_lineedit(&dialog);
	matrix_lineedit.setText(matrix);
	layout.addWidget(&matrix_lineedit);
	matrix_lineedit.show();
	
	QLabel eigenvalues_label("Eigenvalues matrix name:",&dialog);
	layout.addWidget(&eigenvalues_label);
	eigenvalues_label.show();
	
	QLineEdit eigenvalues_lineedit(&dialog);
	eigenvalues_lineedit.setText("eigenvalues");
	layout.addWidget(&eigenvalues_lineedit);
	eigenvalues_lineedit.show();
	
	QLabel eigenvectors_label("Eigenvectors matrix name:",&dialog);
	layout.addWidget(&eigenvectors_label);
	eigenvectors_label.show();
	
	QLineEdit eigenvectors_lineedit(&dialog);
	eigenvectors_lineedit.setText("eigenvectors");
	layout.addWidget(&eigenvectors_lineedit);
	eigenvectors_lineedit.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout.addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	matrix=matrix_lineedit.text();
	QString eigenvalues=eigenvalues_lineedit.text();
	QString eigenvectors=eigenvectors_lineedit.text();
	
	if (dialog.result()==QDialog::Accepted && !matrix.isEmpty() && !eigenvectors.isEmpty() && ! eigenvalues.isEmpty())
	{
		QString command;
		QTextStream(&command) << "[" << eigenvectors << "," << eigenvalues << "]=eig(" << matrix << ")";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		else
		{
			octave_connection->command_enter(command);
		}
	}
}

void Operations::transpose_callback()
{
	/*
	BaseWidget *top_widget=*active_widget;
	QString window_name=top_widget->windowTitle();
	
	if(top_widget->widgetType()==TABLE)
	{
		QString aux;
		QTextStream(&window_name) >> aux >> matrix;
	}
	*/
	
	bool ok;
	
	ok=input_dialog("Transpose", "Matrix name:", matrix);
	
	if (ok && !matrix.isEmpty())
	{
		QString command;
		QTextStream(&command) << matrix << ".'";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		else // if(window_name.startsWith("Octave Terminal") || window_name.startsWith("Help"))
		{
			octave_connection->command_enter(command);
		}
		//else if(window_name.startsWith("Table:"))
		//{
		//	octave_connection->command_enter(matrix+"="+command);
		//	((Table*)top_widget)->windowActivated(top_widget);
		//}
	}
}

void Operations::submatrix_callback()
{
	/*
	BaseWidget *top_widget=*active_widget;
	QString window_name=top_widget->windowTitle();
	
	if(top_widget->widgetType()==TABLE)
	{
		QString aux;
		QTextStream(&window_name) >> aux >> matrix;
	}
	*/
	
	copy_clipboard_ok=false;
	
	//Se muestra el cuadro de di�ogo.
	QDialog dialog(main_window);
	dialog.setWindowTitle("Submatrix");
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QLabel matrix_label("Matrix name:",&dialog);
	layout.addWidget(&matrix_label);
	matrix_label.show();
	
	QLineEdit matrix_lineedit(&dialog);
	matrix_lineedit.setText(matrix);
	layout.addWidget(&matrix_lineedit);
	matrix_lineedit.show();
	
	QHBoxLayout rows_layout;
	layout.addLayout(&rows_layout);
	
	QLabel from_row_label("From row:",&dialog);
	rows_layout.addWidget(&from_row_label);
	from_row_label.show();
	
	QLineEdit from_row_lineedit(&dialog);
	from_row_lineedit.setText("1");
	rows_layout.addWidget(&from_row_lineedit);
	from_row_lineedit.show();
	
	QLabel to_row_label("To row:",&dialog);
	rows_layout.addWidget(&to_row_label);
	to_row_label.show();
	
	QLineEdit to_row_lineedit(&dialog);
	to_row_lineedit.setText("1");
	rows_layout.addWidget(&to_row_lineedit);
	to_row_lineedit.show();
	
	QHBoxLayout cols_layout;
	layout.addLayout(&cols_layout);
	
	QLabel from_col_label("From column:",&dialog);
	cols_layout.addWidget(&from_col_label);
	from_col_label.show();
	
	QLineEdit from_col_lineedit(&dialog);
	from_col_lineedit.setText("1");
	cols_layout.addWidget(&from_col_lineedit);
	from_col_lineedit.show();
	
	QLabel to_col_label("To column:",&dialog);
	cols_layout.addWidget(&to_col_label);
	to_col_label.show();
	
	QLineEdit to_col_lineedit(&dialog);
	to_col_lineedit.setText("1");
	cols_layout.addWidget(&to_col_lineedit);
	to_col_lineedit.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout.addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	matrix=matrix_lineedit.text();
	QString from_row=from_row_lineedit.text();
	QString to_row=to_row_lineedit.text();
	QString from_col=from_col_lineedit.text();
	QString to_col=to_col_lineedit.text();
	
	if (dialog.result()==QDialog::Accepted && !matrix.isEmpty() && !from_row.isEmpty() && !to_row.isEmpty() && !from_col.isEmpty() && !to_col.isEmpty())
	{
		QString command;
		QTextStream(&command) << matrix << "(" << from_row << ":" << to_row << "," << from_col << ":" << to_col <<")";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		else
		{
			octave_connection->command_enter(command);
		}
	}
}


void Operations::copy_clipboard_callback()
{
	copy_clipboard_ok=true;
}

bool Operations::input_dialog(QString title, QString label, QString &input)
{
	copy_clipboard_ok=false;
	
	//Show the dialog window
	QDialog dialog(main_window);
	dialog.setWindowTitle(title);
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QLabel matrix_label(label,&dialog);
	layout.addWidget(&matrix_label);
	matrix_label.show();
	
	QLineEdit matrix_lineedit(&dialog);
	matrix_lineedit.setText(matrix);
	layout.addWidget(&matrix_lineedit);
	matrix_lineedit.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout.addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	input=matrix_lineedit.text();
	
	if (dialog.result()==QDialog::Accepted && !input.isEmpty()) return true;
	return false;
}

void Operations::simple_comand(QString title, QString label, QString _command)
{
	/*
	BaseWidget *top_widget=*active_widget;
	bool ok;
	
	if(top_widget->widgetType()==TABLE)
	{
		matrix=((Table*)top_widget)->getMatrix();
	}
	*/
	
	bool ok;
	
	ok=input_dialog(title, label, matrix);
	
	if (ok && !matrix.isEmpty())
	{
		QString command;
		QTextStream(&command) << _command << "(" << matrix << ")";
		
		if(copy_clipboard_ok)
		{
			 QClipboard *clipboard = QApplication::clipboard();
			 clipboard->setText(command);
		}
		else
		{
			octave_connection->command_enter(command);
		}
	}
}

Operations::MenuResult Operations::menu_window(QString title, QStringList parameters_labels, QStringList &parameters, QStringList output_labels, QStringList &output, QString help, bool accept_blank_parameters)
{
	copy_clipboard_ok=false;
	
	//Se muestra el cuadro de di�ogo.
	QDialog dialog(main_window);
	dialog.setWindowTitle(title);
	QVBoxLayout *layout=new QVBoxLayout;
	dialog.setLayout(layout);
	
	QLabel *parameters_label[parameters_labels.size()];
	QLineEdit *parameters_lineedit[parameters_labels.size()];

	if(parameters_labels.size()>0)
	{
		QGroupBox *input_box=new QGroupBox(&dialog);
		QVBoxLayout *input_layout=new QVBoxLayout;
		
		input_box->setTitle("Input parameters");
		layout->addWidget(input_box);
		input_box->setLayout(input_layout);
		input_box->show();
		
		for (int i = 0; i < parameters_labels.size(); ++i)
		{
			QHBoxLayout *parameters_layout=new QHBoxLayout;
			input_layout->addLayout(parameters_layout);
			
			parameters_label[i]=new QLabel(input_box);
			
			parameters_label[i]->setText(parameters_labels.at(i));
			parameters_layout->addWidget(parameters_label[i]);
			parameters_label[i]->show();
			
			parameters_lineedit[i]=new QLineEdit(input_box);
			parameters_lineedit[i]->setText(parameters.at(i));
			parameters_layout->addWidget(parameters_lineedit[i]);
			parameters_lineedit[i]->show();
		}
	}
	
	QLabel *output_label[parameters_labels.size()];
	QLineEdit *output_lineedit[parameters_labels.size()];

	if(output_labels.size()>0)
	{
		QGroupBox *output_box=new QGroupBox(&dialog);
		QVBoxLayout *output_box_layout=new QVBoxLayout;
		
		output_box->setTitle("Output parameters");
		layout->addWidget(output_box);
		output_box->setLayout(output_box_layout);
		output_box->show();
	
		for (int i = 0; i < output_labels.size(); ++i)
		{
			QHBoxLayout *output_layout=new QHBoxLayout;
			
			output_box_layout->addLayout(output_layout);
			
			output_label[i]= new QLabel(output_box);
			output_label[i]->setText(output_labels.at(i));
			output_layout->addWidget(output_label[i]);
			output_label[i]->show();
			
			output_lineedit[i]= new QLineEdit(output_box);
			output_lineedit[i]->setText(output.at(i));
			output_layout->addWidget(output_lineedit[i]);
			output_lineedit[i]->show();
		}
	}
	
	QTextBrowser *help_text=new QTextBrowser(&dialog);
	help_text->hide();
	
	if(!help.isEmpty())
	{
		help_text->setHtml(help);
		layout->addWidget(help_text);
		help_text->show();
	}
	
	QHBoxLayout *buttons_layout=new QHBoxLayout;
	layout->addLayout(buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout->addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout->addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout->addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	if (dialog.result()==QDialog::Accepted)
	{
		for (int i = 0; i < output_labels.size(); ++i)
		{
			if(output_lineedit[i]->text().isEmpty() && !accept_blank_parameters)
			{
				return CANCEL;
			}
		}
		
		for (int i = 0; i < parameters_labels.size(); ++i)
		{
			if(parameters_lineedit[i]->text().isEmpty() && !accept_blank_parameters)
			{
				return CANCEL;
			}
		}
		 
		if(output_labels.size()>0)
		{
			output.clear();
			for (int i = 0; i < output_labels.size(); ++i)
			{
				output << output_lineedit[i]->text();
			}
		}
		
		if(parameters_labels.size()>0)
		{
			parameters.clear();
			for (int i = 0; i < parameters_labels.size(); ++i)
			{
				parameters << parameters_lineedit[i]->text();
			}
		}
		
		if(copy_clipboard_ok)
		{
			return COPY_CLIPBOARD;
		}
		else
		{
			return ACCEPTED;
		}
	}
	
	return CANCEL;
}

void Operations::compex_comand(QString title, QString _command, QStringList parameters_labels, QStringList & parameters, QStringList output_labels, QStringList & output, QString help, bool is_table, bool accept_blank_parameters)
{
	MenuResult r=menu_window(title, parameters_labels, parameters, output_labels, output, help, accept_blank_parameters);
	
	QString command;
	
	if (r!=CANCEL)
	{
		QTextStream command_stream(&command);
		 
		if(output_labels.size()>0)
		{
			command_stream << "[";
			for (int i = 0; i < output_labels.size();)
			{
				command_stream << output.at(i);
				++i;
				if(i < output_labels.size()) command_stream << ",";
			}
			command_stream << "]=";
		}
		
		if(parameters_labels.size()>0)
		{
			command_stream << _command << "(";
			if( !parameters.at(0).isEmpty() ) command_stream << parameters.at(0);
			for (int i = 1; i < parameters_labels.size();i++)
			{
				if( !parameters.at(i).isEmpty() )
				{
					command_stream << "," << parameters.at(i);
					//++i;
					//if( i < parameters_labels.size() ) command_stream << ",";
				}
				//else i++;
			}
			command_stream << ")";
		}
	}
	else return;
	
	
	if(r==COPY_CLIPBOARD)
	{
			QClipboard *clipboard = QApplication::clipboard();
			clipboard->setText(command);
	}
	//r==ACCEPTED
	else if(!is_table)
	{
		octave_connection->command_enter(command);
	}
	else
	{
		//BaseWidget *top_widget=*active_widget;
		matrix=parameters.at(0);
		/*
		if(top_widget->widgetType()==TABLE)
		{
			if( matrix==((Table*)top_widget)->getMatrix() )
			{
				command=matrix+"="+command;
			}
		}
		*/
		octave_connection->command_enter(command);
	}

}

void Operations::mean_callback()
{
	simple_comand("Mean", "Vector name:", "mean");
}

void Operations::median_callback()
{
	simple_comand("Median", "Vector name:", "median");
}

void Operations::std_callback()
{
	simple_comand("Standard deviation", "Vector name:", "std");
}

void Operations::cov_callback()
{
	QStringList parameters_labels;
	parameters_labels << "x" << "y";
	QStringList parameters;
	parameters << "" << "";
	QStringList output_labels;
	QStringList output;
	QString help("Calculates covariance: cov(x,y)<br>To calculate variance of x use cov(x,x)");
	
	compex_comand("Covariance", "cov", parameters_labels, parameters, output_labels, output, help);
}

void Operations::corrcoef_callback()
{
	QStringList parameters_labels;
	parameters_labels << "x" << "y";
	QStringList parameters;
	parameters << "" << "";
	QStringList output_labels;
	QStringList output;
	
	compex_comand("Correlation Coefficient", "corrcoef", parameters_labels, parameters, output_labels, output);
}

void Operations::plot_callback()
{
	Plot *plot = new Plot(main_window);
	int ok=plot->exec();
	if(ok==QDialog::Accepted)
	{
		QString command=plot->command();
		octave_connection->command_enter(command);
	}
	delete plot;
}

void Operations::semilogy_callback()
{
	Plot *plot = new Plot(main_window, Plot::SEMILOGY);
	int ok=plot->exec();
	if(ok==QDialog::Accepted)
	{
		QString command=plot->command();
		octave_connection->command_enter(command);
	}
	delete plot;
}

void Operations::semilogx_callback()
{
	Plot *plot = new Plot(main_window, Plot::SEMILOGX);
	int ok=plot->exec();
	if(ok==QDialog::Accepted)
	{
		QString command=plot->command();
		octave_connection->command_enter(command);
	}
	delete plot;
}

void Operations::log_log_callback()
{
	Plot *plot = new Plot(main_window, Plot::LOGLOG);
	int ok=plot->exec();
	if(ok==QDialog::Accepted)
	{
		QString command=plot->command();
		octave_connection->command_enter(command);
	}
	delete plot;
}

void Operations::polar_callback()
{
	Plot *plot = new Plot(main_window, Plot::POLAR);
	int ok=plot->exec();
	if(ok==QDialog::Accepted)
	{
		QString command=plot->command();
		octave_connection->command_enter(command);
	}
	delete plot;
}

void Operations::error_bars_callback()
{
	QDialog dialog(main_window);
	dialog.setWindowTitle("Error Bars");
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QTextBrowser text(&dialog);
	layout.addWidget(&text);
	text.setHtml(
	"Please use <b>Plot</b> menu.<br>You can use <b>errorbar</b> command to plot error bars. Also you can use <b>loglogerr, semilogxerr</b> and  <b>semilogyerr</b> for log a semilog errorbars graph.<br>Another way to plot error bars is use plot command and the next function to generate error bars:<br><pre>"
	"%Error bar matrix for plots\n"
	"function [xx, yy]=error_bar(x,y,lex,rex)\n"
	" k=1;\n"
	" for i=1:rows(x)\n"
	"  for j=1:columns(x)\n"
	"   yy(1,k)=y(i,j);\n"
	"   yy(2,k)=y(i,j);\n"
	"   xx(1,k)=x(i,j)-lex(i,j);\n"
	"   xx(2,k)=x(i,j)+rex(i,j);\n"
	"   k=k+1;\n"
	"  end\n"
	" end\n"
	"end\n\n</pre>"
	"Per example:\n<br>"
	"If you have data in x and y. Errors are in ex and ey. You can plot error bars using:<br><pre>"
	"%Create horizontal error bars\n"
	"[xxX, yyX]=error_bar(x,y,ex,ex);\n"
	"%Create vertical error bars. IMPORTANT: x and y position are changed.\n"
	"[yyY, xxY]=error_bar(y,x,ey,ey);\n"
	"%Plot points with error bars\n"
	"plot(x,y,\"+\",xxX,yyX,\"-b\",xxY,yyY,\"-b\");\n"
	"</pre>"
	);
	text.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	if (dialog.result()==QDialog::Accepted)
	{
		
		if(copy_clipboard_ok)
		{
			QClipboard *clipboard = QApplication::clipboard();
			clipboard->setText(
				"%Error bar matrix for plots\n"
				"function [xx, yy]=error_bar(x,y,lex,rex)\n"
				" k=1;\n"
				" for i=1:rows(x)\n"
				"  for j=1:columns(x)\n"
				"   yy(1,k)=y(i,j);\n"
				"   yy(2,k)=y(i,j);\n"
				"   xx(1,k)=x(i,j)-lex(i,j);\n"
				"   xx(2,k)=x(i,j)+rex(i,j);\n"
				"   k=k+1;\n"
				"  end\n"
				" end\n"
				"end\n"
			);
		}
		else
		{
			//octave_connection->command_enter(_axis_command);
		}
	}
}

void Operations::axis_callback()
{
	copy_clipboard_ok=false;
	
	//Se muestra el cuadro de di�ogo.
	QDialog dialog(main_window);
	dialog.setWindowTitle("Axis");
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QHBoxLayout *hlayout= new QHBoxLayout();
	layout.addLayout(hlayout);
	
	QLabel x_min_axis("x-min:",&dialog);
	hlayout->addWidget(&x_min_axis);
	x_min_axis.show();
	
	QLineEdit x_min_lineedit(&dialog);
	hlayout->addWidget(&x_min_lineedit);
	x_min_lineedit.show();
	
	QLabel x_max_axis("x-max:",&dialog);
	hlayout->addWidget(&x_max_axis);
	x_max_axis.show();
	
	QLineEdit x_max_lineedit(&dialog);
	hlayout->addWidget(&x_max_lineedit);
	x_max_lineedit.show();
	
	QCheckBox x_autoscale("Autoscale x",&dialog);
	x_autoscale.setCheckState(Qt::Checked);
	hlayout->addWidget(&x_autoscale);
	x_autoscale.show();
	
	QCheckBox x_tic("Tic marks x",&dialog);
	x_tic.setCheckState(Qt::Checked);
	hlayout->addWidget(&x_tic);
	x_tic.show();
	
	hlayout= new QHBoxLayout();
	layout.addLayout(hlayout);
	
	QLabel y_min_axis("y-min:",&dialog);
	hlayout->addWidget(&y_min_axis);
	y_min_axis.show();
	
	QLineEdit y_min_lineedit(&dialog);
	hlayout->addWidget(&y_min_lineedit);
	y_min_lineedit.show();
	
	QLabel y_max_axis("y-max:",&dialog);
	hlayout->addWidget(&y_max_axis);
	y_max_axis.show();
	
	QLineEdit y_max_lineedit(&dialog);
	hlayout->addWidget(&y_max_lineedit);
	y_max_lineedit.show();
	
	QCheckBox y_autoscale("Autoscale y",&dialog);
	y_autoscale.setCheckState(Qt::Checked);
	hlayout->addWidget(&y_autoscale);
	y_autoscale.show();
	
	QCheckBox y_tic("Tic marks y",&dialog);
	y_tic.setCheckState(Qt::Checked);
	hlayout->addWidget(&y_tic);
	y_tic.show();
	
	hlayout= new QHBoxLayout();
	layout.addLayout(hlayout);
	
	QLabel z_min_axis("z-min:",&dialog);
	hlayout->addWidget(&z_min_axis);
	z_min_axis.show();
	
	QLineEdit z_min_lineedit(&dialog);
	hlayout->addWidget(&z_min_lineedit);
	z_min_lineedit.show();
	
	QLabel z_max_axis("z-max:",&dialog);
	hlayout->addWidget(&z_max_axis);
	z_max_axis.show();
	
	QLineEdit z_max_lineedit(&dialog);
	hlayout->addWidget(&z_max_lineedit);
	z_max_lineedit.show();
	
	QCheckBox z_autoscale("Autoscale z",&dialog);
	z_autoscale.setCheckState(Qt::Checked);
	hlayout->addWidget(&z_autoscale);
	z_autoscale.show();
	
	QCheckBox z_tic("Tic marks z",&dialog);
	z_tic.setCheckState(Qt::Checked);
	hlayout->addWidget(&z_tic);
	z_tic.show();
	
	QCheckBox show_grid("Show grid", &dialog);
	layout.addWidget(&show_grid);
	show_grid.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout.addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	if (dialog.result()==QDialog::Accepted)
	{
		QString _axis_command;
		QTextStream axis_stream(&_axis_command);
		axis_stream << "axis([";
		if( !x_min_lineedit.text().isEmpty() ) axis_stream << x_min_lineedit.text() << ",";
		else axis_stream << "0,";
		if( !x_max_lineedit.text().isEmpty() ) axis_stream << x_max_lineedit.text() << ",";
		else axis_stream << "0,";
		if( !y_min_lineedit.text().isEmpty() ) axis_stream << y_min_lineedit.text() << ",";
		else axis_stream << "0,";
		if( !y_max_lineedit.text().isEmpty() ) axis_stream << y_max_lineedit.text() << ",";
		else axis_stream << "0,";
		if( !z_min_lineedit.text().isEmpty() ) axis_stream << z_min_lineedit.text() << ",";
		else axis_stream << "0,";
		if( !z_max_lineedit.text().isEmpty() ) axis_stream << z_max_lineedit.text() << "]";
		else axis_stream << "0]";
		
		if(x_autoscale.checkState()==Qt::Checked ) axis_stream << ",\"autox\"";
		if(y_autoscale.checkState()==Qt::Checked ) axis_stream << ",\"autoy\"";
		if(z_autoscale.checkState()==Qt::Checked ) axis_stream << ",\"autoz\"";
		
		if(x_tic.checkState()==Qt::Checked ||  y_tic.checkState()==Qt::Checked || z_tic.checkState()==Qt::Checked)
		{
			axis_stream << ",\"tic";
			if(x_tic.checkState()==Qt::Checked) axis_stream << "x";
			if(y_tic.checkState()==Qt::Checked) axis_stream << "y";
			if(z_tic.checkState()==Qt::Checked) axis_stream << "z";
			axis_stream << "\"";
		}
		
		axis_stream << ");\n";
		
		if(show_grid.checkState()==Qt::Checked )
		{
			axis_stream << "grid(\"on\");\n";
		}
		else
		{
			axis_stream << "grid(\"off\");\n";
		}	
			
		if(copy_clipboard_ok)
		{
			QClipboard *clipboard = QApplication::clipboard();
			clipboard->setText(_axis_command);
		}
		else
		{
			octave_connection->command_enter(_axis_command);
		}
	}
	
}

void  Operations::title_label_callback()
{
	copy_clipboard_ok=false;
	
	//Se muestra el cuadro de di�ogo.
	QDialog dialog(main_window);
	dialog.setWindowTitle("Title and Labels");
	QVBoxLayout layout;
	dialog.setLayout(&layout);
	
	QGroupBox labels_frame("Labels", &dialog);
	layout.addWidget(&labels_frame);
	labels_frame.show();
	
	QVBoxLayout labels_layout;
	labels_frame.setLayout(&labels_layout);
	
	QHBoxLayout *hlayout= new QHBoxLayout();
	labels_layout.addLayout(hlayout);
	
	QLabel x_label_label("x label:",&labels_frame);
	hlayout->addWidget(&x_label_label);
	x_label_label.show();
	
	QLineEdit x_label_lineedit(&labels_frame);
	hlayout->addWidget(&x_label_lineedit);
	x_label_lineedit.show();
	
	QCheckBox x_label("Show x label",&labels_frame);
	//x_label.setCheckState(Qt::UnChecked);
	hlayout->addWidget(&x_label);
	x_label.show();
	
	hlayout= new QHBoxLayout();
	labels_layout.addLayout(hlayout);
	
	QLabel y_label_label("y label:",&labels_frame);
	hlayout->addWidget(&y_label_label);
	y_label_label.show();
	
	QLineEdit y_label_lineedit(&labels_frame);
	hlayout->addWidget(&y_label_lineedit);
	y_label_lineedit.show();
	
	QCheckBox y_label("Show y label",&labels_frame);
	//y_label.setCheckState(Qt::Checked);
	hlayout->addWidget(&y_label);
	y_label.show();
	
	hlayout= new QHBoxLayout();
	labels_layout.addLayout(hlayout);
	
	QLabel z_label_label("z label:",&labels_frame);
	hlayout->addWidget(&z_label_label);
	z_label_label.show();
	
	QLineEdit z_label_lineedit(&labels_frame);
	hlayout->addWidget(&z_label_lineedit);
	z_label_lineedit.show();
	
	QCheckBox z_label("Show z label",&labels_frame);
	//z_label.setCheckState(Qt::Checked);
	hlayout->addWidget(&z_label);
	z_label.show();
	
	
	QGroupBox title_frame("Title", &dialog);
	layout.addWidget(&title_frame);
	title_frame.show();
	
	QVBoxLayout title_layout;
	title_frame.setLayout(&title_layout);
	
	hlayout= new QHBoxLayout();
	title_layout.addLayout(hlayout);
	
	QLabel title_label("Title:",&title_frame);
	hlayout->addWidget(&title_label);
	title_label.show();
	
	QLineEdit title_lineedit(&title_frame);
	hlayout->addWidget(&title_lineedit);
	title_lineedit.show();
	
	QHBoxLayout buttons_layout;
	layout.addLayout(&buttons_layout);
	QPushButton ok_button("Ok", &dialog);
	buttons_layout.addWidget(&ok_button);
	ok_button.show();
	connect(&ok_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	
	QPushButton copy_clipboard_button("Copy to clipboard", &dialog);
	buttons_layout.addWidget(&copy_clipboard_button);
	copy_clipboard_button.show();
	connect(&copy_clipboard_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(&copy_clipboard_button,SIGNAL(clicked()),this,SLOT(copy_clipboard_callback()));
	
	QPushButton cancel_button("Cancel", &dialog);
	buttons_layout.addWidget(&cancel_button);
	cancel_button.show();
	connect(&cancel_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	dialog.exec();
	
	if (dialog.result()==QDialog::Accepted)
	{
		QString _axis_command;
		QTextStream axis_stream(&_axis_command);
		
		if(x_label.checkState()==Qt::Checked ||  y_label.checkState()==Qt::Checked || z_label.checkState()==Qt::Checked)
		{
			axis_stream << "axis(\"label";
			if(x_label.checkState()==Qt::Checked) axis_stream << "x";
			if(y_label.checkState()==Qt::Checked) axis_stream << "y";
			if(z_label.checkState()==Qt::Checked) axis_stream << "z";
			axis_stream << "\");";
		}
		
		if(x_label.checkState()==Qt::Checked ||  y_label.checkState()==Qt::Checked || z_label.checkState()==Qt::Checked)
		{
			axis_stream << "\n";
			if(x_label.checkState()==Qt::Checked)
				axis_stream << "xlabel(\"" << x_label_lineedit.text() << "\");\n";
			if(y_label.checkState()==Qt::Checked)
				axis_stream << "ylabel(\"" << y_label_lineedit.text() << "\");\n";
			if(z_label.checkState()==Qt::Checked)
				axis_stream << "zlabel(\"" << z_label_lineedit.text() << "\")\n";
		}
		
		if(!title_lineedit.text().isEmpty())
		{
			axis_stream << "title(\"" << title_lineedit.text() << "\");\n";
		}
		
		if(copy_clipboard_ok)
		{
			QClipboard *clipboard = QApplication::clipboard();
			clipboard->setText(_axis_command);
		}
		else
		{
			octave_connection->command_enter(_axis_command);
		}
	}
	
}

// void Operations::to_eps_callback()
// {
// 	QString file = QFileDialog::getSaveFileName(main_window,"Choose a file", ".","Postscript (*.eps *.ps)");
// 	
// 	if(file.isEmpty()) return;
// 	
// 	QString command;
// 	QTextStream command_stream(&command);
// 	
// 	command_stream << "__gnuplot_set__ term post\n__gnuplot_set__ output \"" << file << "\"\nreplot\n__gnuplot_set__ term x11";
// 	
// 	octave_connection->command_enter(command);
// }
// 
// void Operations::to_pdf_callback()
// {
// 	QString file = QFileDialog::getSaveFileName(main_window,"Choose a file", ".","Postscript (*.pdf)");
// 	
// 	if(file.isEmpty()) return;
// 	
// 	file.replace(" ","\\ ");
// 	
// 	QString command;
// 	QTextStream command_stream(&command);
// 	
// 	command_stream << "__gnuplot_set__ term post\n__gnuplot_set__ output \"|ps2pdf - " << file << " \"\nreplot\n__gnuplot_set__ term x11";
// 	
// 	octave_connection->command_enter(command);
// }
// 
// void Operations::to_png_callback()
// {
// 	QString file = QFileDialog::getSaveFileName(main_window,"Choose a file", ".","Postscript (*.png)");
// 	
// 	if(file.isEmpty()) return;
// 	
// 	QString command;
// 	QTextStream command_stream(&command);
// 	
// 	command_stream << "__gnuplot_set__ term png transparent\n__gnuplot_set__ output \"" << file << "\"\nreplot\n__gnuplot_set__ term x11";
// 	
// 	octave_connection->command_enter(command);
// }

void Operations::show_projects_dialog()
{
	Projects projects(main_window);
	if( QDialog::Accepted==projects.exec() )
	{
		main_window->getSession()->setProjectName(projects.projectName());
		QMap<QString,QString> configuration;
		main_window->setWindowTitle("QtOctave ["+session->getProjectName()+"]");
		configuration["session_name"]=projects.projectName();
		set_config(configuration);
	}
}



#include "general_menu.h"

void Operations::general_config_callback()
{
	General_Menu dialog(main_window);
	
	int ok=dialog.exec();
	if(ok==QDialog::Accepted)
	{
		dialog.apply_config();
	}
}

void Operations::hist_callback()
{
	QStringList parameters_labels;
	parameters_labels << "Vector of values" << "Vector with the centers of the bins or number with the number of bins" << "Normal";
	QStringList parameters;
	parameters << "" << "" << "";
	QStringList output_labels;
	QStringList output;
	QString help(
			"Given a second scalar argument, use that as the number of bins.<br>"
			"Given a second vector argument, use that as the centers of the bins, with the width of the bins determined from the adjacent values in the vector.<br>"
			"If third argument is provided, the histogram is normalised such that the sum of the bars is equal to norm.<br>"
		    );
	
	compex_comand("Histogram", "hist", parameters_labels, parameters, output_labels, output,help,false,true);
}

void Operations::bar_callback()
{
	QStringList parameters_labels;
	parameters_labels << "x" << "y";
	QStringList parameters;
	parameters << "" << "";
	QStringList output_labels;
	QStringList output;
	QString help(
			"Given two vectors of x-y data, bar produces a bar graph.<br>"
			"If only one argument is given, it is taken as a vector of y-values and the x coordinates are taken to be the indices of the elements."
		    );
	
	compex_comand("Bar Graph", "bar", parameters_labels, parameters, output_labels, output,help,false,true);
}

void Operations::contour_callback()
{
	QStringList parameters_labels;
	parameters_labels << "x" << "y" << "z";
	QStringList parameters;
	parameters << "" << "" << "";
	QStringList output_labels;
	QStringList output;
	QString help(
			"Make a contour plot of the three-dimensional surface described by z.<br>"
			"z must be a 2x2 matrix."
		    );
	
	compex_comand("contour plot of the three-dimensional", "contour", parameters_labels, parameters, output_labels, output,help,false,true);
}

void Operations::mesh_callback()
{
	QStringList parameters_labels;
	parameters_labels << "x" << "y" << "z";
	QStringList parameters;
	parameters << "" << "" << "";
	QStringList output_labels;
	QStringList output;
	QString help(
			"Plot a mesh given matrices x, and y from meshdom and a matrix z corresponding to the x and y coordinates of the mesh. If x and y are vectors, then a typical vertex is (x(j), y(i), z(i,j)). Thus, columns of z correspond to different x values and rows of z correspond to different y values."
		    );
	
	compex_comand("Plot of the three-dimensional", "mesh", parameters_labels, parameters, output_labels, output,help,false,true);
}

void Operations::suma_callback()
{
	QStringList parameters_labels;
	QStringList parameters;
	QStringList output_labels;
	QStringList output;
	QString help( "Use + for matrix addition." );
	
	menu_window("A+B",parameters_labels, parameters, output_labels, output,help);
}

void Operations::producto_callback()
{
	QStringList parameters_labels;
	QStringList parameters;
	QStringList output_labels;
	QStringList output;
	QString help( "Use * for matrix product." );
	
	menu_window("A*B",parameters_labels, parameters, output_labels, output,help);
}

void Operations::exponente_callback()
{
	QStringList parameters_labels;
	QStringList parameters;
	QStringList output_labels;
	QStringList output;
	QString help( "Use ** for matrix exponential." );
	
	menu_window("A**B",parameters_labels, parameters, output_labels, output,help);
}

void Operations::exit_callback()
{
	//octave_connection->command_enter("quit");
	//QApplication::quit();
	main_window->close();
}

void Operations::cd_callback()
{
	QString dir=QFileDialog::getExistingDirectory ( main_window, "Select dir");
	if(dir.isEmpty()) return;
	
	QString command("cd \"");
	command+=dir+"\"";
	octave_connection->command_enter(command);
}

void Operations::qtoctave_pkg_callback()
{
	QStringList arguments;
	QString working_directory;
	bool ok=QProcess::startDetached("qtoctave_pkg", arguments, working_directory);
	if(!ok)
	{
		QMessageBox::critical(main_window, 
				tr("qtoctave_pkg not found"), 
				tr("qtoctave_pkg can not be executed. Please, install qtoctave_pkg or put it in PATH") 
				);
	}
}



