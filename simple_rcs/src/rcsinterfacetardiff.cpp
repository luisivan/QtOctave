#include "rcsinterfacetardiff.h"
#include <QTemporaryFile>
#include <QDir>
#include <QFileInfo>
#include <QDate>
#include <QTime>
#include <QProcess>
#include <stdio.h>
#include <stdlib.h>

static int _system_(const QString &command, const QString &dir)
{

	//printf("Comando: %s %s\n", dir.toLocal8Bit().data(), command.toLocal8Bit().data());
	
	QProcess p;
	p.setWorkingDirectory(dir);
	p.start(command);
	
	if(p.waitForFinished())
	{
		//printf("Comando finalizado\n");
	}
	
	//printf("Estado de salida %d\n", p.exitStatus());
	
	char buf[1024];
	qint64 lineLength;
	while( (lineLength = p.readLine(buf, sizeof(buf))) > -1)
	{
		//printf("Salida: %s\n", buf);
	}
	
	p.close();
	
	return p.exitCode();
}

static QStringList system_string(const QString &command)
{
	QStringList result;
	
	FILE *in=popen(command.toLocal8Bit().data() ,"r");
	
	if(in!=NULL)
	{
		char buf[1024];
		
		while(!feof(in))
		{
			if(fgets(buf, 1024, in)!=NULL)
			{
				result << QString::fromLocal8Bit(buf).replace("\n", "");
				//puts(buf);
			}
		}
	
		pclose(in);
	}
	
	return result;
}


static QStringList read_file(const QString &file)
{
	QStringList result;
	
	FILE *in=fopen(file.toLocal8Bit().data() ,"r");
	
	if(in!=NULL)
	{
		char buf[1024];
		
		while(!feof(in))
		{
			if(fgets(buf, 1024, in)!=NULL)
			{
				result << QString::fromLocal8Bit(buf);
			}
		}
	
		fclose(in);
	}
	
	return result;
}

static QString temp_file()
{
	QString name;
	QTemporaryFile file;
	//file.setAutoRemove(false);
	if (file.open()) {
		name=file.fileName();
	}
	return name;
}

static QString make_temp_dir()
{
	QString dir=temp_file();
	
	dir=QFileInfo(dir).fileName();
	
	QDir temp=QDir::temp();
	
	//printf("Temp dir %s\n", temp.path().toLocal8Bit().data() );
	
	if(!temp.mkdir(dir))
	{
		printf("[%s:make_temp_dir] Error: Dir %s can not be make.\n", __FILE__, dir.toLocal8Bit().data() );
		exit(0);
	}
	
	temp.cd(dir);
	
	return temp.path();
}

static void remove_temp_dir(QString temporal)
{
	QString dir=QFileInfo(temporal).fileName();
	
	QDir d(temporal);
	
	QStringList list=d.entryList();
	
	for(int i=0;i<list.size();i++)
	{
		if( list[i]=="." || list[i]==".." ) continue;
		QString name=d.filePath(list[i]);
		QFile::remove(name);
		printf("[%s:remove_temp_dir] file %s removed.\n", __FILE__, name.toLocal8Bit().data() );
	}
	
	d.cdUp();
	
	if(!d.rmdir (dir))
	{
		printf("[%s:remove_temp_dir] Error: Dir %s can not be remove.\n", __FILE__, dir.toLocal8Bit().data() );
		exit(0);
	}
}

bool RcsInterfaceTarDiff::add(QString file)
{
	//printf("añadiendo archivo.\n");
	
	QString temp=make_temp_dir();
	
	QDate date=QDate::currentDate();
	QTime time=QTime::currentTime();
	QString version=date.toString("yyyy-MM-dd ")+time.toString("hh:mm:ss.zzz");
	
	
	QFile::copy(file, temp+"/"+ version);
	QString command, gzip;
	
	if(QFile::exists(repository))
	{
		QFile::copy(repository, temp+"/repo.tar.gz");
		command+="tar -uf repo.tar \""+version+"\"";
		//printf("El fichero exists.\n");
		gzip+="gzip -d repo.tar.gz";
		_system_(gzip, temp);
	}
	else
	{
		//printf("El fichero no exists.\n");
		command+="tar -cf repo.tar '"+version+"'";
	}
	
	int ok=_system_(command, temp);
	gzip="gzip repo.tar";
	_system_(gzip, temp);
	
	QFile::remove(repository);
	
	QFile::copy(temp+"/repo.tar.gz",repository);
	
// 	QFile::remove(temp+"/repo.tar.gz");
// 	QFile::remove( temp+"/"+ version);
	
	remove_temp_dir(temp);
	
	return ok!=0;
}
	
bool RcsInterfaceTarDiff::extract(QString file, QString version)
{
	QString command="tar -xzOf '"+repository+"' '"+version+"' > '"+file+"'";
	
	int ok=system(command.toLocal8Bit().data());
	
	return ok!=0;
}

QString RcsInterfaceTarDiff::properties(QString version)
{
	QString command="tar -tzvf '"+repository+"' '"+version+"'";
	
	QStringList result=system_string(command);
	
	return result.join("\n");
}

QString RcsInterfaceTarDiff::diff(QString version1, QString version2)
{
	QString file1=temp_file();
	QString file2=temp_file();
	
	extract(file1, version1);
	extract(file2, version2);
	
	QStringList s1=read_file(file1);
	QStringList s2=read_file(file1);
	
	int len1=0, len2=0, len;
	
	for(int i=0;i<s1.size();i++)
	{
		if(len1<s1[i].size()) len1=s1[i].size();
	}
	
	for(int i=0;i<s2.size();i++)
	{
		if(len2<s2[i].size()) len2=s2[i].size();
	}
	
	len=len1+len2+10;
	
	QString command="diff -y -t -W "+QString::number(len)+" '"+file1+"' '"+file2+"'";
	
	QStringList result=system_string(command);
	
	
	
	//printf("Ejecutado diff\n");
	
	QFile::remove(file1);
	QFile::remove(file2);
	
	//Se construye la salida según los resultados de diff
	QString html;
	len2=len/2;
	
	html+="<html>"
	"<HEAD>"
		"<META HTTP-EQUIV='CONTENT-TYPE' CONTENT='text/html; charset=utf-8'>"
		"<TITLE>SimpleRcs</TITLE>"
	"</HEAD>"
	"<body>";
	
	html+="<b>Showing differences between:</b><br>";
		
	
	html+="<TABLE BORDER=1 BORDERCOLOR='#000000' CELLPADDING=3 CELLSPACING=0>"
		"<tr>"
			"<td></td>"
			"<td><b>"+version1+"</b></td>"
			"<td></td>"
			"<td></td>"
			"<td><b>"+version2+"</b></td>"
		"</tr>\n";
	
	int size=result.size();
	int line1_num=0;
	int line2_num=0;
	for(int i=0;i<size;i++)
	{
		QString line=result[i];
		len=line.size();
		QString ch;
		if(len2<len)
		{
			ch=line.mid(len1+2,5).trimmed();
		}
		else
		{
			ch=line.right(2).trimmed();
		}
		//printf("ch=%s\n", ch.toLocal8Bit().data());
		
		QString line1=line.left(len2-1).trimmed();
		QString line2=line.mid(len2+1).trimmed();
		QString color;
		
		line1=line1.replace("&", "&amp;");
		line1=line1.replace("<", "&lt;");
		line1=line1.replace(">", "&gt;");
		line1=line1.replace(" ", "&nbsp;");
		line1=line1.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
		
		line2=line2.replace("&", "&amp;");
		line2=line2.replace("<", "&lt;");
		line2=line2.replace(">", "&gt;");
		line2=line2.replace(" ", "&nbsp;");
		line2=line2.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
		
		line="";
		
		if( ch=="|" )
		{
			color="#23ff23";
			line1_num++;
			line2_num++;
		}
		else if(ch=="(" || ch==")" || ch=="/")
		{
			color="#ff0000";
			line1_num++;
			line2_num++;
		}
		else if( ch=="<" )
		{
			color="#ff0000";
			line1_num++;
			ch="&lt;";
		}
		else if( ch==">" )
		{
			color="#ffa000";
			line2_num++;
			ch="&gt;";
		}
		else
		{
			color="#ffffff";
			line1_num++;
			line2_num++;
		}
		
		line="<tr>"
			"<td BGCOLOR='"+color+"'>"+QString::number(line1_num)+"</td>"
			"<td BGCOLOR='"+color+"'>"+line1+"</td>"
			"<td BGCOLOR='"+color+"'>"+ch+"</td>"
			"<td BGCOLOR='"+color+"'>"+QString::number(line2_num)+"</td>"
			"<td BGCOLOR='"+color+"'>"+line2+"</td>"
		"</tr>";
		
		
		html+=line+"\n";
	}
	
	html+="</table></body></html>";
	
	//printf("%s", html.toLocal8Bit().data());
	
	return html;
}

QStringList RcsInterfaceTarDiff::versions()
{
	QString command="tar -tzf '"+repository+"'";
	QStringList list;
	
	list=system_string(command);
	
	for(int i=0; i<list.size(); i++)
	{
		if(list[i]=="Properties.txt")
		{
			list.removeAt(i);
		}
	}
	
	return list;
}


QHash<QString, QString> RcsInterfaceTarDiff::readCommentsFromPropertiesFile(QString file)
{
	QHash<QString, QString> list;
	QFile in(file);
	
	if(!in.open(QIODevice::ReadOnly)) return list;
	
	 while (!in.atEnd())
	 {
		QByteArray line = in.readLine();
		int i=line.indexOf("->");
		if(i>-1)
		{
			QByteArray version=line.left(i).trimmed();
			QByteArray comment=line.right(line.size()-i-2).trimmed();
			
			list[version]=QString::fromLocal8Bit(comment.data());
		}
	}
	
	in.close();
	
	return list;
}


void RcsInterfaceTarDiff::writeCommentsToPropertiesFile(QString file, QHash<QString, QString> list)
{
	QFile out(file);
	
	if(!out.open(QIODevice::WriteOnly)) return;
	
	QList<QString> versions=list.keys();
	
	for(int i=0;i<versions.size();i++)
	{
		QString line=versions[i]+" -> "+list[versions[i]]+"\n";
		out.write(line.toLocal8Bit());
	}
	
	out.close();
}


bool RcsInterfaceTarDiff::setComment(QString version, QString comment)
{
	QString temp=make_temp_dir();
	
	QString command, gzip, properties_file;
	
	int ok;
	
	if(QFile::exists(repository))
	{
		QFile::copy(repository, temp+"/repo.tar.gz");
		gzip="gzip -d repo.tar.gz";
		ok=_system_(gzip, temp);
		command="tar -xf repo.tar ";
		ok=_system_(command, temp);
	}
	
	properties_file=temp+"/Properties.txt";
	
	QHash<QString, QString> list=readCommentsFromPropertiesFile(properties_file);
	
	list[version]=comment;
	
	writeCommentsToPropertiesFile(properties_file, list);
	
	
	QFile::remove(temp+"/repo.tar");
	
	//system(QString("ls -l '"+temp+"' ").toLocal8Bit().data());
	
	QDir d(temp);
	
	QStringList files=d.entryList();
	
	for(int i=0;i<files.size();i++)
	{
		if( files[i]=="." || files[i]==".." || files[i]=="repo.tar") continue;
		QString name=files[i];
		
		if(i==2) command="tar -cf repo.tar \""+name+"\" ";
		else command="tar -rf repo.tar \""+name+"\" ";
		
		ok=_system_(command, temp);
		
		//system(QString("cd '"+temp+"'; "+command).toLocal8Bit().data());
	}
	
	gzip="gzip repo.tar";
	ok=_system_(gzip, temp);
	
	//system(QString("tar -tzvf '"+temp+"/repo.tar.gz' ").toLocal8Bit().data());
	
	
	QFile::remove(repository);
	
	QFile::copy(temp+"/repo.tar.gz",repository);
	
	remove_temp_dir(temp);
	
	return ok!=0;
}

QHash<QString, QString> RcsInterfaceTarDiff::getComment()
{
	QString file=temp_file();
	
	extract(file, "Properties.txt");
	
	QHash<QString, QString> list=readCommentsFromPropertiesFile(file);
	
	QFile::remove(file);
	
	return list;
}

