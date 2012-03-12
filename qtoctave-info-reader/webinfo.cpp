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

#include "webinfo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


WebInfo::WebInfo(QWidget *parent):QWidget(parent)
{
	fontWeb=font();
	
	QVBoxLayout *layout=new QVBoxLayout(this);
	setLayout(layout);
	
	QHBoxLayout *hboxLayout=new QHBoxLayout();
	layout->addLayout(hboxLayout);
	
	closeTabButton=new QPushButton(this);
	closeTabButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	closeTabButton->setIcon(QIcon(":/images/stop.png"));
	hboxLayout->addWidget(closeTabButton);
	
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab_cb()));
	
	tabs=new QTabBar(this);
	tabs->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	tabs->setExpanding(false);
	hboxLayout->addWidget(tabs);
	
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged_cb(int)));
	
	zoomInButton=new QToolButton(this);
	zoomInButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	zoomInButton->setIcon(QIcon(":/images/zoom-in.png"));
	hboxLayout->addWidget(zoomInButton);
	connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn_cb()));
	
	zoomOutButton=new QToolButton(this);
	zoomOutButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	zoomOutButton->setIcon(QIcon(":/images/zoom-out.png"));
	hboxLayout->addWidget(zoomOutButton);
	connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut_cb()));
	
	stack=new QStackedWidget(this);
	layout->addWidget(stack);
	
	hboxLayout=new QHBoxLayout();
	layout->addLayout(hboxLayout);
	
	searchButton=new QPushButton(this);
	searchButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	searchButton->setIcon(QIcon(":/images/question.png"));
	hboxLayout->addWidget(searchButton);
	connect(searchButton, SIGNAL(clicked()), this, SLOT(search_cb()));
	
	searchLineEdit=new QLineEdit(this);
	hboxLayout->addWidget(searchLineEdit);
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(search_cb()));
	
	searchCheckBox=new QCheckBox(tr("Global search"));
	hboxLayout->addWidget(searchCheckBox);
	
	resize(600,600);
}

void WebInfo::setInfoPath(QString infoPath)
{
	parser.setInfoPath(infoPath);
	loadNode("Top");
}

void WebInfo::loadNode(QString nodeName)
{
	//Check if node has been already opened.
	for(int i=0;i<tabs->count();i++)
	{
		if(nodeName==tabs->tabText(i))
		{
			tabs->setCurrentIndex(i);
			return;
		}
	}
	
	QString text=parser.searchNode(nodeName);
	
	int i=parser.isRef(nodeName);
	
	web=addNewTab(nodeName);
	
	// web->setPlainText( text );
	web->setHtml( parser.nodeTextToHtml(text, i-1, "anchor") );
	
	
	if(i!=-1)
	{
		web->scrollToAnchor("anchor");
	}
}

void WebInfo::linkClicked_cb(const QUrl & link)
{
	QString node=link.toString();
	
	loadNode(node);
}

void WebInfo::currentTabChanged_cb( int index )
{
	QVariant data=tabs->tabData(index);
	web=(QTextBrowser*)(data.value<void*>());
	
	stack->setCurrentIndex( stack->indexOf(web) );

	if(web->font()!=fontWeb) web->setFont(fontWeb);
}

QTextBrowser *WebInfo::addNewTab(QString name)
{
	web=new QTextBrowser(this);
	web->setOpenLinks(false);
	web->show();
	
	connect(web, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(linkClicked_cb(const QUrl &)) );
	
	disconnect(tabs, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged_cb(int)));
	
	int ns=stack->addWidget(web);
	stack->setCurrentIndex(ns);
	
	int nt=tabs->addTab(name);
	tabs->setCurrentIndex(nt);
	QVariant data;
	data.setValue((void*)web);
	tabs->setTabData( nt, data );
	
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged_cb(int)));
	
	if(web->font()!=fontWeb) web->setFont(fontWeb);
	
	return web;
}

void WebInfo::closeTab_cb()
{
	int index=tabs->currentIndex();
	if(tabs->tabText(index)!="Top")
		closeTab(index);
}

void WebInfo::closeTab(int index)
{
	QVariant data=tabs->tabData(index);
	QWidget *w=(QWidget*)(data.value<void*>());
	
	stack->removeWidget(w);
	delete w;
	
	tabs->removeTab(index);
}

void WebInfo::search_cb()
{
	if(searchCheckBox->isChecked())
	{
		// Global search
		QString results=parser.globalSearch(searchLineEdit->text(), 5);
		web=addNewTab("Results for: "+searchLineEdit->text());
		// web->setPlainText( results );
		web->setHtml( results );
	}
	else
	{
		// Local search
		web->find(searchLineEdit->text());
	}
}

void WebInfo::zoomIn_cb()
{
	fontWeb.setPointSize(fontWeb.pointSize()+1);
	web->setFont(fontWeb);
}

void WebInfo::zoomOut_cb()
{
	fontWeb.setPointSize(fontWeb.pointSize()-1);
	web->setFont(fontWeb);
}

