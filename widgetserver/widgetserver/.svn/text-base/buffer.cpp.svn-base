#include "buffer.h"

Buffer::Buffer(QObject *parent)
	:QObject(parent)
{
	
}

bool Buffer::canReadLine()
{
	mutex.lock();
	bool ok=buffer.contains("\n");
	mutex.unlock();
	return ok;
}

QString Buffer::readLine()
{
	mutex.lock();
	int pos=buffer.indexOf("\n");
	QString line(buffer.left(pos+1));
	
	buffer.remove(0,pos+1);
	mutex.unlock();
	return line;
}

void Buffer::write(const QByteArray & data)
{
	mutex.lock();
	buffer+=QString(data);
	mutex.unlock();
}
