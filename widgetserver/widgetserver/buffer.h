#ifndef __BUFFER_H_
#define __BUFFER_H_

#include <QObject>
#include <QString>
#include <QMutex>

class Buffer:public QObject
{
	Q_OBJECT
	private:
		QString buffer;
		QMutex mutex;
	public:
		Buffer(QObject *parent=0);
		bool canReadLine();
		QString readLine();
		void write(const QByteArray & data);
};

#endif
