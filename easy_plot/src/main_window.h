#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QtSvg>
#include <QSvgWidget>
#include <QMainWindow>
#include "ui_design_main_window.h"
#include "gnuplot_connection.h"
#include "ui_view_angle.h"

using namespace Ui;

class MainWindow: public QMainWindow
{
	Q_OBJECT
	
	QSvgWidget svg;
	Ui_DesignMainWindow ui;
	
	QWidget *view_angle;
	Ui_ViewAngle *view_angle_ui;
	
	void make_connections();
	public:
	MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0);
	GnuplotConnection *gnuplot;
	public slots:
	void plot(QByteArray svg_contents);
	void title_font_cb();
	void labels_font_cb();
	void tics_font_cb();
	void legend_font_cb();
	void actionExport_to_callback();
	void x11WindowTitle(QString title);
	void set_title_cb();
	void set_x_label_cb();
	void set_y_label_cb();
	void set_z_label_cb();
	void set_x_grid_cb(bool ok);
	void set_y_grid_cb(bool ok);
	void set_z_grid_cb(bool ok);
	void set_x2_grid_cb(bool ok);
	void set_y2_grid_cb(bool ok);
	void set_view_angle_cb();
	void set_x_angle_cb(int angle);
	void set_z_angle_cb(int angle);
	void print_cb();
};



#endif
