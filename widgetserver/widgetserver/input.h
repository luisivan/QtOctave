#ifndef __INPUT_H__
#define __INPUT_H__

#include <QThread>
#include "process.h"
#include "buffer.h"

class Input:public QThread
{
	Q_OBJECT
	private:
		Process *process;
		FILE *in, *out;
	public:
		Input(FILE *in, FILE *out, Process *process, QObject * parent = 0);
		void run();
	public slots:
		void write_to_output(QString e);
	signals:
		void readyRead(QString e);
};

#endif
