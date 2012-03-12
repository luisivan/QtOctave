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

#ifndef __WINDOW_LIST_H__
#define __WINDOW_LIST_H__

#include <QComboBox>
#include <QMdiArea>

/** Shows window list asociated to QMdiArea. When some item is selected, asociated window is maximized.
*/

class WindowList:public QComboBox
{
	Q_OBJECT
	QMdiArea *work_space;
	public:
	WindowList( QMdiArea *work_space, QWidget * parent =0);
	void showPopup ();
	public slots:
	void itemClicked_callback(int item);
	void window_activated_callback(QMdiSubWindow *);
};

#endif
