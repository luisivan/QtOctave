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

#include <QTextBrowser>
#include "parser.h"
#include <QStackedWidget>
#include <QTabBar>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QToolButton>

class WebInfo:public QWidget
{
	Q_OBJECT
	QTextBrowser *web;
	Parser parser;
	QStackedWidget *stack;
	QTabBar *tabs;
	QPushButton *closeTabButton;
	QPushButton *searchButton;
	QLineEdit *searchLineEdit;
	QCheckBox *searchCheckBox;
	
	QToolButton *zoomInButton;
	QToolButton *zoomOutButton;
	QFont fontWeb;
	
	QTextBrowser *addNewTab(QString name);
	void closeTab(int index);
	
	public:
	WebInfo(QWidget *parent = 0);
	void setInfoPath(QString infoPath);
	
	void loadNode(QString nodeName);
	
	public slots:
	void linkClicked_cb( const QUrl & link );
	void currentTabChanged_cb( int index );
	void closeTab_cb();
	void search_cb();
	void zoomIn_cb();
	void zoomOut_cb();
	  
};