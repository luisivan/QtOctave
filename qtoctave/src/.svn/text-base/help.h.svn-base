/* Copyright (C) 2006,2007,2008 P.L. Lucas
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

#ifndef __HELP_H__
#define __HELP_H__

#include "basewidget.h"
#include <qtextbrowser.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtoolbar.h>
#include <QVBoxLayout>
#include <QPushButton>

/**Shows help in simple html. Users can do searches.
*/
class Help:public BaseWidget
{
	Q_OBJECT
	public:
	Help(QWidget * parent = 0);
	void setSource(const QString source, QString anchor="");
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	~Help();
	private:
	QVBoxLayout *vlayout;
	QToolBar *toolbar;
	QTextBrowser *browser;
	QLineEdit *lineedit;
	QCheckBox *check_box;
	QPushButton *search_button;
	QString source;
	void create_buttons();
	public slots:
	void back();
	void forward();
	void find();
	void go_home();
	void textChanged ( const QString & text );
};
	
#endif
