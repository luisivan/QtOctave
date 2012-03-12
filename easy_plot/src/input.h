#ifndef __INPUT_H__
#define __INPUT_H__

#include <QThread>
#include <QVector>

#include "gnuplot_connection.h"

/**Thread to read data from input.*/
class InputThread:public QThread
{
	Q_OBJECT
	enum State {NONE, SET, PLOT} state;
	void parse_plot(QByteArray line);
	void parse_set(QByteArray line);
	void parse_input(QByteArray line);
	QRegExp re_number,
		re_plot_set_output,
		re_plot_set_term,
		re_plot_set_X11,
		re_plot_set_X11_title;
	GnuplotConnection *gnuplot;
	
	void write_to_gnuplot(QByteArray command);
	
	public:
	InputThread();
	void run();
	void setGnuplotConnection(GnuplotConnection *gnuplot);
	signals:
	void x11WindowTitle(QString title);
	void write_to_gnuplot_signal(QByteArray command);
};


#endif
