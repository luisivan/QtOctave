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

#include <QStringList>
#include <QIODevice>
#include <QFileInfoList>
#include <QHash>


/**Position of node in info files. 
 */
struct RefMapItem
{
	/**Node name.*/
	QString node;	
	int pos;
};

struct NodeMapItem
{
	//QFileInfo fileInfo;
	int pos;
};

struct InfoFileItem
{
	QFileInfo fileInfo;
	int realSize;
};


/**This class gets nodes and searchs inside of `info files'. 
 * <p>Each info file has nodes. Every node has the documentation.
 * Info files contains a map with position of each node.</p>
 * <p>What is position?
 * There is a simple answer:
 * If you make a queue with info files, position will be the number of bytes
 * from begining to node position.</p>
 * <p>
 * But is not so easy. There is headers, and qtinfo must not take these headers into account.
 * </p>
 * <p>
 * This class also translates info files to html.
 * </p>
 */
class Parser:public QObject
{
	Q_OBJECT
	QString infoPath;
	QFileInfoList infoFiles;
	
	QString searchNode(QString node, QIODevice * io);
	
	QString getNextNode(QIODevice * io);
	
	QString getNodeName(QString text);
	QString getNodeUp(QString text);
	QString getNodeNext(QString text);
	QString getNodePrev(QString text);
	
	QHash<QString, NodeMapItem> nodeMap;
	QHash<QString, RefMapItem> refMap;
	QList<InfoFileItem> infoFileRealSizeList;
	
	/**List of suported compressor and commands to uncompress files to stdout with them.*/
	QHash<QString, QString> compilersMap;
	
	/**Parses info files and gets map of node positions.
	 */
	void parseInfoMap();
	
	/**Open info files and uncompress them.
	 */
	QIODevice *openFile(QFileInfo & fileInfo);
	
	/**Calculates real position of nodes.
	 *  \param pos position from info file.
	 *  \param fileInfo returns file what contains that position.
	 *  \param realPos returns real position inside of fileInfo.
	 */
	void realPosition(int pos, QFileInfo & fileInfo, int & realPos);
	
	/**Seeks to position pos.
	 */
	void seek(QIODevice *io, int pos);

	public:
	Parser(QObject *parent = 0);
	
	void setInfoPath(QString infoPath);
	QString getInfoPath();
	
	QString searchNode(QString node);
	
	QString globalSearch(QString text, int maxFounds);
	
	/**Checks if this node is reference. If node is reference, it will be returned its position
	 * in text, else  it will be returned -1.
	 */
	int isRef(QString node);
	
	/**Translates text of node to Html. If anchorPos is not -1, then anchor is inserted in that
	 * position.
	 */
	QString nodeTextToHtml(QString text, int anchorPos=-1, QString anchor=QString());
};