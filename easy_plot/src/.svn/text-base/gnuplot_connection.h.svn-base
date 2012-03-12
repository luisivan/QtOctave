#ifndef __GNUPLOT_CONNECTION__
#define __GNUPLOT_CONNECTION__

#include <QProcess>
#include <QMutex>


/** Connection with Gnuplot.
 */
class GnuplotConnection:public QProcess
{
	Q_OBJECT
	QString svg_contents;
	enum State {NONE, SVG} parse_state;
	QByteArray svg_terminal;
	public:
	GnuplotConnection(QObject *parent=0);
	//void write( QByteArray text);
	void replot();
	void setFont(QString name, int size);
	QByteArray data;
	QMutex data_mutex;
	inline QByteArray svgTerminal() {return svg_terminal;}
	public slots:
	void standardOutputReady();
	void gnuplotFinished(int exitCode, QProcess::ExitStatus exitStatus );
	void write_command(QByteArray command);
	void data_clear();
	void add_data(QByteArray line);
	signals:
	void plot(QByteArray svg_contents);
};


#endif
