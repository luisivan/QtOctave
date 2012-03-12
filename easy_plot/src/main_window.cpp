#include "main_window.h"

#include <QFontDialog>
#include <QInputDialog>
#include <QSvgGenerator>


MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags):QMainWindow(parent,flags)
{
	ui.setupUi(this);
	setCentralWidget(&svg);
	svg.show();
	view_angle=NULL;
	view_angle_ui=NULL;
	
	make_connections();
}

void MainWindow::make_connections()
{
	connect(ui.actionTitle, SIGNAL( triggered()), this, SLOT( title_font_cb()) );
	connect(ui.actionLabels, SIGNAL( triggered()), this, SLOT( labels_font_cb()) );
	connect(ui.actionTics, SIGNAL( triggered()), this, SLOT( tics_font_cb()) );
	connect(ui.actionLegend, SIGNAL( triggered()), this, SLOT( legend_font_cb()) );
	connect(ui.actionExport_to, SIGNAL( triggered()), this, SLOT( actionExport_to_callback()) );
	
	connect(ui.actionSet_title, SIGNAL( triggered()), this, SLOT( set_title_cb()) );
	connect(ui.actionX_label, SIGNAL( triggered()), this, SLOT( set_x_label_cb()) );
	connect(ui.actionY_label, SIGNAL( triggered()), this, SLOT( set_y_label_cb()) );
	connect(ui.actionZ_label, SIGNAL( triggered()), this, SLOT( set_z_label_cb()) );
	
	connect(ui.actionX_grid, SIGNAL( triggered(bool)), this, SLOT( set_x_grid_cb(bool)) );
	connect(ui.actionY_grid, SIGNAL( triggered(bool)), this, SLOT( set_y_grid_cb(bool)) );
	connect(ui.actionZ_grid, SIGNAL( triggered(bool)), this, SLOT( set_z_grid_cb(bool)) );
	connect(ui.actionX2_grid, SIGNAL( triggered(bool)), this, SLOT( set_x2_grid_cb(bool)) );
	connect(ui.actionY2_grid, SIGNAL( triggered(bool)), this, SLOT( set_y2_grid_cb(bool)) );
	
	connect(ui.actionView_Angle, SIGNAL( triggered()), this, SLOT( set_view_angle_cb()) );
	
	connect(ui.actionPrint, SIGNAL( triggered()), this, SLOT( print_cb()) );
}

void MainWindow::plot(QByteArray svg_contents)
{
	svg.load(svg_contents);
	show() ;
}

void MainWindow::legend_font_cb()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
	if (ok)
	{
		gnuplot->setFont(font.family(), font.pointSize());
		gnuplot->write((gnuplot->svgTerminal()+"\n"));
		gnuplot->replot();
	}
}

void MainWindow::tics_font_cb()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
	if (ok)
	{
		QByteArray command="set xtics font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		command+="set ytics font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		command+="set ztics font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::labels_font_cb()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
	if (ok)
	{
		QByteArray command="set xlabel font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		command+="set ylabel font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		command+="set zlabel font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::title_font_cb()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
	if (ok)
	{
		QByteArray command="set title font \""+font.family().toLocal8Bit()+","+QByteArray::number(font.pointSize())+"\"\n";
		//printf("%s\n", command.toLocal8Bit().data());
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::x11WindowTitle(QString title)
{
	setWindowTitle(title);
}


void MainWindow::set_title_cb()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Set title"), tr("Title:"), QLineEdit::Normal, QString(), &ok);
	if (ok)
	{
		QByteArray command="set title \""+text.toLocal8Bit()+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::set_x_label_cb()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Set x label"), tr("x label:"), QLineEdit::Normal, QString(), &ok);
	if (ok)
	{
		QByteArray command="set xlabel \""+text.toLocal8Bit()+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::set_y_label_cb()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Set y label"), tr("y label:"), QLineEdit::Normal, QString(), &ok);
	if (ok)
	{
		QByteArray command="set ylabel \""+text.toLocal8Bit()+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::set_z_label_cb()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Set z label"), tr("z label:"), QLineEdit::Normal, QString(), &ok);
	if (ok)
	{
		QByteArray command="set zlabel \""+text.toLocal8Bit()+"\"\n";
		gnuplot->write(command);
		gnuplot->replot();
	}
}

void MainWindow::set_x_grid_cb(bool ok)
{
	QByteArray command;
	if(ok) command="set grid xtics\n";
	else command="set grid noxtics\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_y_grid_cb(bool ok)
{
	QByteArray command;
	if(ok) command="set grid ytics\n";
	else command="set grid noytics\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_z_grid_cb(bool ok)
{
	QByteArray command;
	if(ok) command="set grid ztics\n";
	else command="set grid noztics\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_x2_grid_cb(bool ok)
{
	QByteArray command;
	if(ok) command="set grid x2tics\n";
	else command="set grid nox2tics\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_y2_grid_cb(bool ok)
{
	QByteArray command;
	if(ok) command="set grid y2tics\n";
	else command="set grid noy2tics\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_view_angle_cb()
{
	if(view_angle==NULL)
	{
		view_angle_ui=new Ui_ViewAngle();
		view_angle=new QWidget();
		view_angle_ui->setupUi(view_angle);
		
		connect(view_angle_ui->ok_pushButton, SIGNAL(clicked()), view_angle, SLOT(close()) );
		connect(view_angle_ui->x_angle_horizontalSlider, SIGNAL(valueChanged ( int )), this, SLOT(set_x_angle_cb(int)));
		connect(view_angle_ui->z_angle_horizontalSlider, SIGNAL(valueChanged ( int )), this, SLOT(set_z_angle_cb(int)));
	}
	
	view_angle->show();
}

void MainWindow::set_x_angle_cb(int angle)
{
	view_angle_ui->x_angle_label->setText(QString::number(angle));
	QByteArray command=
		"set view "
		+QByteArray::number(view_angle_ui->x_angle_horizontalSlider->value())+
		", "
		+QByteArray::number(view_angle_ui->z_angle_horizontalSlider->value())+
		"\n";
	gnuplot->write(command);
	gnuplot->replot();
}

void MainWindow::set_z_angle_cb(int angle)
{
	view_angle_ui->z_angle_label->setText(QString::number(angle));
	QByteArray command=
		"set view "
		+QByteArray::number(view_angle_ui->x_angle_horizontalSlider->value())+
		", "
		+QByteArray::number(view_angle_ui->z_angle_horizontalSlider->value())+
		"\n";
	gnuplot->write(command);
	gnuplot->replot();
}

#include "ui_export_to_dialog.h"

enum ExportFormat {PIXMAP, PICTURE, PRINTER, PDF, SVG};

void MainWindow::actionExport_to_callback()
{
	QDialog dialog;
	ExportToDialog export_to_dialog;
	export_to_dialog.setupUi(&dialog);
	QListWidget *list = export_to_dialog.formats_listWidget;
	QListWidgetItem *item;
	//Pixmaps formats
	QList<QByteArray> formats=QImageWriter::supportedImageFormats();
	for(int i=0;i<formats.size();i++)
	{
		QString text(formats[i]);
		item=new QListWidgetItem(text,list);
		item->setData(1,QVariant(PIXMAP));
	}
	//Vector formats
	formats= QPicture::outputFormats();
	for(int i=0;i<formats.size();i++)
	{
		QString text(formats[i]);
		item=new QListWidgetItem(text,list);
		item->setData(1,QVariant(PICTURE));
	}
	
	item=new QListWidgetItem("ps",list);
	item->setData(1,QVariant(PRINTER));
	
	item=new QListWidgetItem("pdf",list);
	item->setData(1,QVariant(PDF));
	
	item=new QListWidgetItem("svg",list);
	item->setData(1,QVariant(SVG));
	
	int ok=dialog.exec();
	if(ok==QDialog::Rejected) return;
	
	item =list->currentItem();
	int format=item->data(1).toInt();
	
	QPainter plot;
	switch(format)
	{
		case PIXMAP:
		{
			bool ok;
			int h, w = QInputDialog::getInteger(this, tr("Width"), tr("Width:"), 300, 0, 2147483647, 1, &ok);
			if(!ok) return;
			h=QInputDialog::getInteger(this, tr("Height"), tr("Height:"), 200, 0, 2147483647, 1, &ok);
			if(!ok) return;
			QString s = QFileDialog::getSaveFileName(this, "Choose a filename to save");
			if(s.isEmpty()) return;
			QImage image(w,h,QImage::Format_RGB32);
			plot.begin(&image);
			svg. renderer()->render(&plot);
			plot.end();
			image.save(s,item->data(0).toString().toLocal8Bit().data());
		}
		break;
		case PICTURE:
		{
			bool ok;
			int h, w = QInputDialog::getInteger(this, tr("Width"), tr("Width:"), 300, 0, 2147483647, 1, &ok);
			if(!ok) return;
			h=QInputDialog::getInteger(this, tr("Height"), tr("Height:"), 200, 0, 2147483647, 1, &ok);
			if(!ok) return;
			QString s = QFileDialog::getSaveFileName(this, "Choose a filename to save");
			if(s.isEmpty()) return;
			QPicture image;
			const QRect r(0,0,w,h);
			image.setBoundingRect(r);
			plot.begin(&image);
			svg. renderer()->render(&plot);
			plot.end();
			image.save(s,item->data(0).toString().toLocal8Bit().data());
		}
		break;
		case PRINTER:
		{
			QPrinter p;
			QPrintDialog printDialog(&p, this);
			if (printDialog.exec() != QDialog::Accepted) return;
			plot.begin(&p);
			svg. renderer()->render(&plot);
			plot.end();
		}
		break;
		case PDF:
		{
			QPrinter p;
			QPrintDialog printDialog(&p, this);
			p.setOutputFormat(QPrinter::PdfFormat);
			if (printDialog.exec() != QDialog::Accepted) return;
			
			plot.begin(&p);
			svg. renderer()->render(&plot);
			plot.end();
		}
		break;
		case SVG:
		{
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("*.svg"));
			QSvgGenerator p;
			p.setFileName(fileName);
			plot.begin(&p);
			svg. renderer()->render(&plot);
			plot.end();
		}
	}
}

void MainWindow::print_cb()
{
	QPainter plot;
	QPrinter p;
	QPrintDialog printDialog(&p, this);
	if (printDialog.exec() != QDialog::Accepted) return;
	plot.begin(&p);
	svg. renderer()->render(&plot);
	plot.end();
}
