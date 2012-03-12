/* Copyright (C) 2006 P.L. Lucas
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

#include "help.h"
#include <qaction.h>
#include <qlabel.h>
#include <qdir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QFile>
#include <QFileInfo>
#include "config.h"

Help::Help( QWidget * parent):BaseWidget(parent)
{
	widget_type=HELP;
	setWindowIcon( QIcon( QString( ICON_PATH )+"/help.png" ) );
	setWindowTitle(tr("Help"));
	vlayout=new QVBoxLayout();
	vlayout->setSpacing(0);
	vlayout->setMargin(0);
	centralWidget()->setLayout(vlayout);
	create_buttons();
	browser=new QTextBrowser(this);
	vlayout->addWidget(browser);
	browser->show();
}


void Help::setSource(const QString source, QString anchor)
{
	QFileInfo src(source);
	
	browser->setSource(  QUrl::fromLocalFile( src.canonicalFilePath() )  );
	this->source=source;
	browser->scrollToAnchor(anchor);
}


void Help::create_buttons()
{
	toolbar = addToolBar(tr("Navigation"));
	//toolbar->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
	
	QAction *_back = new QAction( QIcon( QString( ICON_PATH )+"/back.png" ), tr("&Back"), this );
	connect( _back, SIGNAL( triggered() ) , this, SLOT( back() ) );
	
	toolbar->addAction(_back);
	
	QAction *_forward = new QAction(QPixmap( QString( ICON_PATH )+"/forward.png"),tr("&Forward"),this);
	connect( _forward, SIGNAL( triggered() ) , this, SLOT( forward() ) );
	
	toolbar->addAction(_forward);
	
	QAction *_go_home = new QAction(QPixmap( QString( ICON_PATH )+"/gohome.png"),tr("&Go home"),this);
	connect( _go_home, SIGNAL( triggered() ) , this, SLOT( go_home() ) );
	
	toolbar->addAction(_go_home);
	
	search_button=new QPushButton(QPixmap( QString( ICON_PATH )+"/find.png"), "Search:", this);
	toolbar->addWidget(search_button);
	search_button->show();
	connect(search_button,SIGNAL(clicked()),this,SLOT(find()));
	
	lineedit=new QLineEdit(toolbar);
	toolbar->addWidget(lineedit);
	lineedit->show();
	connect(lineedit,SIGNAL(returnPressed()),this,SLOT(find()));
	connect( lineedit, SIGNAL( textChanged ( const QString &) ) , this, SLOT( textChanged ( const QString &) ) );
	
	check_box=new QCheckBox("Search in this page.", toolbar);
	toolbar->addWidget(check_box);
	check_box->show();
	
	vlayout->addWidget(toolbar);
}

void Help::back()
{
	browser->backward();
}

void Help::forward()
{
	browser->forward();
}

void Help::go_home()
{
	browser->home();
}


/**Simula un grep para el directorio dado.*/
static void grep(QString & out, QDir & path, const QString text)
{
	QStringList list=path.entryList(QStringList("*.html"));
	QByteArray target_word=text.toLocal8Bit();
	QString line;
	
	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) 
	{
		QString filename=path.path()+"/"+(*it);
		
		QFile in(filename);
		if(!in.open(QFile::ReadOnly)) continue;
		
		
		int start=-1, end=-1;
		QRegExp tag_re("<.*>");
		
		while (!in.atEnd())
		{
			line=in.readLine();
			
			line.replace(tag_re, "");
			
			while( (start=line.indexOf('<'))>=0 )
			{
				while( (end=line.indexOf('>'))<0 )
				{
					line+=in.readLine();
				}
				line.replace(tag_re, "");
			}
			
			if( line.contains(target_word) )
			{
				line.replace(target_word, "<b>"+target_word+"</b>");
				out+="<a href=\""+QUrl::fromLocalFile(filename).toString ()+"\">"+filename+"</a><br>\n";
				out+=QString(line).trimmed()+QString("<br><br>\n");
				break;
			}
		}
		
		in.close();
	}
}


void Help::find()
{
	if(check_box->isChecked())
	{
		bool found = browser->find(lineedit->text());
		if(found)
		{
			browser->ensureCursorVisible();
		}
		return;
	}
	QFileInfo fileinfo(source);
	QDir path(fileinfo.absoluteDir());
	QString out("<html>Results for <b>"+QString(lineedit->text())+"</b><br><br>\n");
	grep(out,path,QString(lineedit->text()));
	out+="</html>";
	browser->setHtml(out);
}

void Help::textChanged ( const QString & text )
{
	emit dynamic_help_required(text);
}

Help::~Help ( )
{
}


BaseWidget *Help::copyBaseWidget(QWidget * parent )
{
	Help *bw=new Help(parent);
	bw->setSession(session);
	bw->setSource(source);
	
	bw->browser->setHtml(browser->toHtml());
	
	return bw;
}






