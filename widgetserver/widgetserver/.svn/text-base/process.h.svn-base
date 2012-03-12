#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <QProcess>
#include <QMap>
#include <QWidget>
#include <QBuffer>


class Widget: public QObject
{
	Q_OBJECT
	public:
	QString name;
	QWidget *widget;
	enum Type {NOWIDGET,WINDOW,P,BUTTON,LABEL,VBOX,LINEEDIT,TEXTEDIT,HTML,LIST,MENUBAR,WORKSPACE};
	Type type;
	enum State {NORMAL,TEXT,HEADERS,ITEM};
	QList<State> state;
	public slots:
		void clicked();
	signals:
		void event(QString e);
};


class Process: public QObject
{
	Q_OBJECT
	private:
		bool debug_ok;
		//Reading state
		QList<Widget*> state;
		//Add widget to parent widget
		void add_widget(Widget *parent, Widget *child);
		//Process widget
		bool window_process(QString line);
		bool p_process(QString line);
		bool button_process(QString line);
		bool label_process(QString line);
		bool vbox_process(QString line);
		bool lineedit_process(QString line);
		bool textedit_process(QString line);
		bool html_process(QString line);
		bool list_process(QString line);
		bool menubar_process(QString line);
		bool workspace_process(QString line);
		bool quit_process(QString line);
		QString end; //String used for archived text tag end
		bool texttag_process(QString tagname, QString & end, QList<Widget::State> & state, QString line, QString & text);
	protected:
		int n_line;
	public:
		Process();
		~Process();
		QMap<QString, Widget*> widgets;
		void setDebug(bool debug_ok);
	public slots:
		void write_to_process(QString e);
		void process(QString line);
	signals:
		void write_to_output(QString e);
};

class ExternProcess:public QProcess
{
	Q_OBJECT
		Process *process;
	public:
		ExternProcess(QString command, Process *process);
	public slots:
		void read_stdout();
		void write_to_process(QString e);
};


#endif
