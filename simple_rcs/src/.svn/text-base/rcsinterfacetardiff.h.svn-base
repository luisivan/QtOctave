#ifndef __RCSINTERFACETARDIFF_H__
#define __RCSINTERFACETARDIFF_H__

#include "rcsinterface.h"

class RcsInterfaceTarDiff:public RcsInterface
{
	public:
	
	bool add(QString file);
	
	bool extract(QString file, QString version);
	
	QString properties(QString version);

	QString diff(QString version1, QString version2);
	
	QStringList versions();
	
	bool setComment(QString version, QString comment);
	
	QHash<QString, QString> getComment();
	
	private:
	
	QHash<QString, QString> readCommentsFromPropertiesFile(QString file);
	
	void writeCommentsToPropertiesFile(QString file, QHash<QString, QString> list);
};

#endif

