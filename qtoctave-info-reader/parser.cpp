/* Copyright (C) 2009 P.L. Lucas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */

#include "parser.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QRegExp>
#include <QProcess>
#include <QBuffer>

Parser::Parser(QObject *parent):QObject(parent)
{
	compilersMap.insert("bz2",  "bzip2 -dc \"%1\"");
	compilersMap.insert("gz",   "gzip -dc \"%1\"");
	compilersMap.insert("lzma", "lzma -dc \"%1\"");
	compilersMap.insert("xz",   "xz -dc \"%1\"");
	compilersMap.insert("Z",    "gunzip -c \"%1\"");
}

void Parser::setInfoPath(QString infoPath)
{
	this->infoPath=infoPath;
	
	infoFiles.clear();
	
	QFileInfo info(infoPath);
	
	QString path=info.absolutePath();
	QString fileName=info.fileName();
	
	QDir infoDir(path);
	QStringList filter;
	filter.append(fileName+"*");
	
	infoFiles=infoDir.entryInfoList(filter, QDir::Files);
	
	for(int i=0;i<infoFiles.size();i++)
	{
		printf("Path:%s\n", infoFiles.at(i).absoluteFilePath().toLocal8Bit().data() );
	}
	
	parseInfoMap();
}

QString Parser::getInfoPath()
{
	return infoPath;
}

QIODevice *Parser::openFile(QFileInfo & fileInfo)
{
	QIODevice *iodevice=NULL;
	printf("[Parser::openFile] suffix %s file %s\n",fileInfo.suffix().toLocal8Bit().data(), fileInfo.absoluteFilePath().toLocal8Bit().data());
	if( compilersMap.contains( fileInfo.suffix() ) )
	{
		QProcess gzip;
		gzip.start( compilersMap.value( fileInfo.suffix() ).arg(fileInfo.absoluteFilePath()) );
		
		if (!gzip.waitForFinished())
			return NULL;

     		QByteArray result = gzip.readAll();
     		
     		QBuffer *io=new QBuffer(this);
     		io->setData(result);
        	
        	if (!io->open(QIODevice::ReadOnly | QIODevice::Text))
       			return NULL;
        	
		iodevice=io;
	}
	else
	{
		QFile *io=new QFile(fileInfo.absoluteFilePath());
   		if (!io->open(QIODevice::ReadOnly | QIODevice::Text))
       			return NULL;
       		iodevice=io;
       	}
       	
       	return iodevice;
}

int Parser::isRef(QString node)
{
	if(refMap.contains(node))
	{
		RefMapItem ref=refMap[node];
		
		return ref.pos-nodeMap[ref.node].pos;
	}
	
	return -1;
}

QString Parser::searchNode(QString node)
{
	QFileInfo fileInfo;
	QString ref;
	
	printf("[Parser::searchNode] Nodo >%s<\n", node.toLocal8Bit().data());
	
	if(refMap.contains(node))
	{
		ref=node;
		node=refMap[ref].node;
	}
	
	if(nodeMap.contains(node))
	{
		printf("Nodo %s encontrado\n", node.toLocal8Bit().data());
		int pos=nodeMap[node].pos;
		int realPos;
		
		realPosition(pos, fileInfo, realPos);
		
		printf("Archivo: %s\nPosición: %d\nPosición real: %d\n", fileInfo.absoluteFilePath().toLocal8Bit().data(), pos, realPos);
		
		QIODevice *io=openFile(fileInfo);
		if(io==NULL) return QString();
		
		seek(io, realPos);
		
		QString text=getNextNode(io);
         	if( !text.isEmpty() ) return text;
		
		io->close();
		delete io;
	}
	
	return QString();
}

QString Parser::searchNode(QString node, QIODevice * io)
{
	while(!io->atEnd())
	{
		QString text=getNextNode(io);
		if(  node == getNodeName(text)  )
		{
			return text;
		}
	}
	
	return QString();
}

QString Parser::getNextNode(QIODevice * io)
{
	QString text;
	
	while (!io->atEnd()) {
		QByteArray line = io->readLine();
		if(line.at(0) == 31) break;
		else text.append(line);
	}
	
	printf("[Parser::getNextNode] Node: %s\n", text.toLocal8Bit().data());
	
	
	return text;
}

static QString get1stLine(QString text)
{
	int n=text.indexOf("\n");
	
	if(n<0) return QString();
	
	QString firstLine=text.left(n);
	
	return firstLine;
}

static QString parserNode(QString text, QString nodeName)
{
	QString firstLine=get1stLine(text);
	
	QStringList nodes=firstLine.split(",");
	
	for(int i=0;i<nodes.size();i++)
	{
		QString node=nodes.at(i).trimmed();
		
		if(node.startsWith(nodeName))
		{
			return node.remove(0,nodeName.size()).trimmed();
		}
	}
	
	return QString();
}

QString Parser::getNodeName(QString text)
{
	return parserNode(text, "Node:");
}

QString Parser::getNodeUp(QString text)
{
	return parserNode(text, "Up:");
}

QString Parser::getNodeNext(QString text)
{
	return parserNode(text, "Next:");
}

QString Parser::getNodePrev(QString text)
{
	return parserNode(text, "Prev:");
}

static void replaceLinks(QString &text)
{
	QRegExp re("(\\*[N|n]ote|\n\\*)([ |\n]+)([^:]+):([^:\\.,]*)([:,\\.])");
	
	int i=0, f;
	
	while( ( i=re.indexIn(text,i) ) != -1 )
	{
		QString type=re.cap(1);
		QString note=re.cap(3);
		QString urlLink=re.cap(4);
		QString link=re.cap(4);
		
		printf("[replaceLinks] >%s< >%s< >%s< >%s<\n", re.cap(0).trimmed().toLocal8Bit().data(),type.trimmed().toLocal8Bit().data(), note.toLocal8Bit().data(), link.toLocal8Bit().data());
		
		if(urlLink.isEmpty()) urlLink=note;
		urlLink=urlLink.trimmed();
		urlLink.replace("\n"," ");
		urlLink.replace(QRegExp("  +")," ");
		urlLink.replace("<b>","");
		urlLink.replace("</b>","");
		urlLink=QUrl::toPercentEncoding(urlLink,"","'");
		
		
		QString href;
		if(type=="\n*") href="\n<img src=':/images/bookmark.png'/>";
		else href="<img src=':/images/bookmark.png'/>";//type==*note
		href+=re.cap(2)+"<a href='"+urlLink+"'>"+note+":"+link+re.cap(5)+"</a>";
		
		f = re.matchedLength();
		text.replace(i,f,href);
		
		//printf("[replaceLinks] replaced=%d original=%d\n", (note+":"+link+re.cap(5)).size(), re.cap(0).size());
		
		i+=href.size();
	}
}

static void replaceColons(QString &text)
{
	QRegExp re("`([^']+)'");
	
	int i=0, f;
	
	while( ( i=re.indexIn(text,i) ) != -1 )
	{
		QString t=re.cap(1);
		QString bold="<b>`"+t+"</b>'";
		
		f = re.matchedLength();
		text.replace(i,f,bold);
		
		i+=bold.size();
	}
}

static void infoToHtml(QString &text)
{
	text.replace("&", "&amp;");
	text.replace("<", "&lt;");
	text.replace(">", "&gt;");
	
	text.replace("\n* Menu:", "\n<b>Menu:</b>");
	text.replace("*See also:*", "<i>*See also:*</i>");
	replaceColons(text);
	replaceLinks(text);
}

QString Parser::nodeTextToHtml(QString text, int anchorPos, QString anchor)
{
	QString nodeName=getNodeName(text);
	QString nodeUp=getNodeUp(text);
	QString nodeNext=getNodeNext(text);
	QString nodePrev=getNodePrev(text);
	
	if(anchorPos>-1)
	{
		QString text1=text.left(anchorPos);
		QString text2=text.mid(anchorPos);
		
		int n=text1.indexOf("\n");
		text1.remove(0,n);
		
		infoToHtml(text1);
		infoToHtml(text2);
		
		text=text1+"<a name='"+anchor+"' /><img src=':/images/stop.png'>"+text2;
	}
	else 
	{
		int n=text.indexOf("\n");
		text.remove(0,n);
		infoToHtml(text);
	}
	
	QString navigationLinks= QString(
		"<img src=':/images/star.png'/> <b>Node:</b> %1<br>"
		"<b>Prev:</b> <a href='%2'>%3</a><br>"
		"<b>Next:</b> <a href='%4'>%5</a><br>"
		"<b>Up:</b> <a href='%6'>%7</a><br>\n"
		)
		.arg(nodeName)
		.arg(QString(QUrl::toPercentEncoding(nodePrev,"","'")))
		.arg(nodePrev)
		.arg(QString(QUrl::toPercentEncoding(nodeNext,"","'")))
		.arg(nodeNext)
		.arg(QString(QUrl::toPercentEncoding(nodeUp,"","'")))
		.arg(nodeUp);
	
	
	text.prepend("<hr>\n<pre>");
	text.append("</pre>\n<hr><hr>\n");
	
	text.prepend(navigationLinks);
	text.append(navigationLinks);
	
	text.prepend("<html><body>\n");
	text.append("</body></html>\n");
	
	return text;
}

void Parser::parseInfoMap()
{
	QRegExp re("(Node|Ref): ([^\\0177]+)\\0177(\\d+)\n");
	QRegExp reFiles("([^:]+): (\\d+)\n");
	int foundCount=0;
	
	for(int i=0;i<infoFiles.size();i++)
	{
		QFileInfo fileInfo=infoFiles.at(i);
		
		QIODevice *io=openFile(fileInfo);
		if(io==NULL) continue;
		
		QString nodeText;
		while ( !(nodeText=getNextNode(io)).isEmpty() && foundCount<2)
		{
			QString firstLine=get1stLine(nodeText);
			if( firstLine.startsWith("Tag") )
			{
			foundCount++;
			int pos = 0;
			QString lastNode;

			 while ((pos = re.indexIn(nodeText, pos)) != -1) {
				QString type=re.cap(1);
				QString node=re.cap(2);
				int index=re.cap(3).toInt();
				
				printf("%s: >%s< >%d<\n",type.toLocal8Bit().data(),
				node.toLocal8Bit().data(), index);
				
				if(type=="Node")
				{
					NodeMapItem item;
					//item.fileInfo=fileInfo;
					item.pos=index;
					nodeMap[node]=item;
					lastNode=node;
				}
				else if(type=="Ref")
				{
					RefMapItem item;
					item.node=lastNode;
					item.pos=index;
					refMap[node]=item;
				}
				pos += re.matchedLength();
			 }
			 
			 break;
			 }
			 else if( firstLine.startsWith("Indirect:") )
			 {
			 foundCount++;
			 int pos = 0;
			 QString lastNode;

			 while ((pos = reFiles.indexIn(nodeText, pos)) != -1) {
				QString fileCap=reFiles.cap(1).trimmed();
				int index=reFiles.cap(2).toInt();
				
				printf(">%s<: >%d<\n",fileCap.toLocal8Bit().data(), index);
				printf("%s: >%d<\n",fileCap.toLocal8Bit().data(), index);
				
				InfoFileItem item;
				for(int j=0;j<infoFiles.size();j++)
				{
					QFileInfo info=infoFiles.at(j);
					printf("%s == %s\n",
						info.fileName().toLocal8Bit().data(), 
						fileCap.toLocal8Bit().data()
						);
					if(info.fileName().startsWith(fileCap))
					{
						item.fileInfo=info;
						printf(">> %s\n",info.fileName().toLocal8Bit().data());
						break;
					}
				}
				item.realSize=index;
				
				infoFileRealSizeList.append(item);
				
				pos += reFiles.matchedLength();
			 }
			 
			 }
		}
		
		io->close();
		delete io;
	}
}

void Parser::realPosition(int pos, QFileInfo & fileInfo, int & realPos)
{
	int header=-1, sum=0;

	for(int i=0;i<infoFileRealSizeList.size();i++)
	{
		InfoFileItem item=infoFileRealSizeList.at(i);
		
		printf("File: %s\nReal size: %d\n", item.fileInfo.absoluteFilePath().toLocal8Bit().data(), item.realSize);
		
		if(header==-1)
		{
			fileInfo=item.fileInfo;
			header=item.realSize;
		}
		
		if( pos<(item.realSize) )
			break;
		
		fileInfo=item.fileInfo;
		sum=item.realSize;
	}
	
	printf("Header: %d\n", header);
	
	realPos=pos-sum+header+2;
}

void Parser::seek(QIODevice *io, int pos)
{
	char ch;
	while (!io->atEnd() && pos>0) {
		io->getChar(&ch);
		pos--;
	}
}

static void replace(QString &text, QRegExp re, QString after)
{
	int pos = 0;

	 while ((pos = re.indexIn(text, pos)) != -1)
	 {
	 	QString cap=text.mid(pos,re.matchedLength());
	 	QString a(after);
	 	a=a.arg(cap);
	 	text.remove(pos,re.matchedLength());
	 	text.insert(pos,a);
	 	pos += a.size();
	 }
}

QString Parser::globalSearch(QString text, int maxFounds)
{
	QString results;
	QStringList words=text.split(" ",QString::SkipEmptyParts);
	
	QString reProgram("("+words.at(0));
	for(int i=1;i<words.size();i++) reProgram+="|"+words.at(i);
	reProgram+=")";
	
	QRegExp re(reProgram,Qt::CaseInsensitive);
	
	results.append("<html><body>\n<h1>Search results</h1>\n<b>Results for:</b> ");
	results.append(text);
	results.append("<br>\n");
	
	for(int i=0;i<infoFiles.size();i++)
	{
		QFileInfo fileInfo=infoFiles.at(i);
		
		QIODevice *io=openFile(fileInfo);
		if(io==NULL) continue;
		
		QString nodeText;
		while ( !(nodeText=getNextNode(io)).isEmpty() )
		{
			QString firstLine=get1stLine(nodeText);
			QString node=getNodeName(nodeText);
			if(node.isEmpty()) continue;
			
			int n=nodeText.indexOf("\n");
			nodeText.remove(0,n);
			
			int pos = 0;
			int founds=0;
			
			for(;founds<words.size() && nodeText.indexOf(words.at(founds))>=0;founds++)
			{}

			if(founds<words.size()) continue;
			founds=0;
			
			 while ((pos = re.indexIn(nodeText, pos)) != -1 && founds<maxFounds)
			 {
				int lineStart, lineEnd;
				lineStart=nodeText.lastIndexOf("\n", pos);
				lineEnd=nodeText.indexOf("\n", pos);
				
				QString line=nodeText.mid(lineStart, lineEnd-lineStart).trimmed();
				
				if(founds==0)
				{
					results.append(
						"<br>\n<img src=':/images/bookmark.png'> <a href='"
						+QString(QUrl::toPercentEncoding(node,"","'"))+
						"'>");
					results.append(node);
					results.append(	"</a><br>\n");
				}
				
				replace(line, re, "<i>%1</i>");
				results.append(line);
				results.append(	"<br>\n");
				
				founds++;
				
				pos += re.matchedLength();
			 }
		}
		
		io->close();
		delete io;
	}
	
	results.append(	"</body></html>");
	
	return results;
}
