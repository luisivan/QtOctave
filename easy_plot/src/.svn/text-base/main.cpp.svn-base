#include <QApplication>
#include "gnuplot_connection.h"
#include "main_window.h"
#include "input.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	app.setQuitOnLastWindowClosed(false);
	
	GnuplotConnection gnuplot;
	{
		QStringList arguments;
		for(int i=1;i<argc;i++)
		{
			arguments << argv[i];
		}
		gnuplot.start("gnuplot", arguments);
		gnuplot.waitForStarted();
		if(gnuplot.state()==QProcess::NotRunning)
		{
			fprintf(stderr, "Error %d starting gnuplot\n",gnuplot.error());
			return 1;
		}
		else
		{
			//printf("Gnuplot Running\n");
		}
		gnuplot.write("set term svg\n");
	}
	
	MainWindow mainwindow;
	mainwindow.gnuplot= &gnuplot;
	MainWindow::connect(&gnuplot, SIGNAL(plot(QByteArray)), &mainwindow, SLOT(plot(QByteArray)) );
	mainwindow.show();
	
	InputThread input;
	input.setGnuplotConnection(&gnuplot);
	
	MainWindow::connect(&input, SIGNAL(x11WindowTitle(QString)), &mainwindow, SLOT(x11WindowTitle(QString)));
	
	input.start();
	
	
	
	app.exec();
}
