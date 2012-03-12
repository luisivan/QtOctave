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

#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__
#include "mainwindow.h"
#include "octave_connection.h"
#include "basewidget.h"
#include "table.h"
#include <QDialog>
#include <QLineEdit>
#include <QTreeWidget>
#include <QComboBox>
#include <QCheckBox>


/**This class is used to show plots menus.
 */
class Plot:public QDialog
{
	Q_OBJECT
	public:
	/**Menu type.*/
	enum Type {PLOT,SEMILOGY, SEMILOGX, LOGLOG, POLAR};
	Plot(QWidget *parent=0, Plot::Type type=PLOT);
	/**Command to send to Octave.
	 * @return QString representing command. This command is used by Octave to plot.
	 */
	QString command();
	
	private:
	QLineEdit *x_lineedit, *y_lineedit, *error_x_lineedit, *error_y_lineedit, *title_lineedit;
	QVBoxLayout *vlayout;
	QComboBox *style_combo, *color_combo, *pointStyle_combo;
	QTreeWidget *plots_tree;
	QCheckBox *no_title;
	Plot::Type type;
	
	public slots:
	void add_callback();
	void remove_callback();
	void copy_clipboard_callback();
};


/**Connects menu signals with callbacks.
*/

class Operations:public QObject
{
	Q_OBJECT
public:
	Operations( QObject * parent = 0, BaseWidget **active_widget=0,
		       			MainWindow *main_window=0 );
	void setOctaveConnection(OctaveConnection *octave_connection);
	void setMatrix(QString matrix);
	
	/**Sets session.*/
	inline void setSession(Session *session) {this->session=session;}
	
	enum MenuResult {ACCEPTED, COPY_CLIPBOARD, CANCEL}; //Se usa para saber el tipo de salida de un men
	
	/**Shows simple dialog with one label and line edit.
	 * @param title Dialog title.
	 * @param label Dialog label.
	 * @param input Value of line edit
	 * @return true if user press ok button.
	 */
	bool input_dialog(QString title, QString label, QString &input);
	/**Shows simple dialog with one label and line edit, and executes _command with line edit value.
	 * @param title Dialog title.
	 * @param label Dialog label.
	 * @param _command Command to execute.
	 * @return true if user press ok button.
	 */
	void simple_comand(QString title, QString label, QString _command);
	/**Shows dialog with labels and line edits.
	 * @param title Dialog title.
	 * @param parameters_labels Dialog labels for input parameters.
	 * @param parameters Value of line edits for input parameters.
	 * @param output_labels Dialog labels for output parameters.
	 * @param output Value of line edits for output parameters.
	 * @param help Html text with help.
	 * @param accept_blank_parameters If user lefts parameters in blank, this option lets them. Otherwise function returns CANCEL.
	 * @return Result of dialog.
	 */
	MenuResult menu_window(QString title, QStringList parameters_labels, QStringList &parameters, QStringList output_labels, QStringList &output, QString help=QString(), bool accept_blank_parameters=false);
	
	/**Shows dialog with labels and line edits, and executes _command with line edit value..
	 * @param title Dialog title.
	 * @param _command Command to execute.
	 * @param parameters_labels Dialog labels for input parameters.
	 * @param parameters Value of line edits for input parameters.
	 * @param output_labels Dialog labels for output parameters.
	 * @param output Value of line edits for output parameters.
	 * @param help Html text with help.
	 * @param is_table If focused window is a table window, asigs value to table.
	 * @param accept_blank_parameters If user lefts parameters in blank, this option lets them. Otherwise function returns.
	 */
	void compex_comand(QString title, QString _command, QStringList parameters_labels, QStringList &parameters, QStringList output_labels, QStringList &output, QString help=QString(), bool is_table=false, bool accept_blank_parameters=false);
	
public slots:
	/**Makes copy_clipboard_ok true. This callback function deals with menu_window and input_dialog methods.
	 * @see copy_clipboard_ok, menu_window, input_dialog.
	 */
	void copy_clipboard_callback();
	
	//File operations
	void exit_callback();
	void cd_callback();
	
	//Operaciones con matrices
	void inverse_callback();
	void determinant_callback();
	void eigenvalues_callback();
	void transpose_callback();
	void submatrix_callback();
	
	//Operaciones estad�ticas
	void mean_callback();
	void median_callback();
	void std_callback();
	void cov_callback();
	void corrcoef_callback();
	
	//Plot menu
	void plot_callback();
	void axis_callback();
	void title_label_callback();
	//void to_eps_callback();
	//void to_pdf_callback();
	//void to_png_callback();
	void semilogy_callback();
	void semilogx_callback();
	void log_log_callback();
	void polar_callback();
	void error_bars_callback();
	/**Callback for histogram menu. Shows dialog for histogram.*/
	void hist_callback();
	/**Callback for bar graph menu. Shows dialog for bar graph.*/
	void bar_callback();
	/**Callback for contour menu. Shows dialog for contour plot of three-dimensional surface.*/
	void contour_callback();
	/**Callback for mesh menu. Shows dialog for plot of three-dimensional surface.*/
	void mesh_callback();
	/**Callback for A+B menu.*/
	void suma_callback();
	/**Callback for A*B menu.*/
	void producto_callback();
	/**Callback for A**B menu.*/
	void exponente_callback();
	
	//General actions
	void show_projects_dialog();
	
	//Config actions
	void general_config_callback();

	//Run qtoctave_pkg
	void qtoctave_pkg_callback();

private:
	OctaveConnection *octave_connection;
	QString matrix;
	BaseWidget **active_widget;
	MainWindow *main_window;
	Session *session;
	
	bool copy_clipboard_ok;
};
#endif
