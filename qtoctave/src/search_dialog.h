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

#ifndef __SEARCH_DIALOG_H__
#define __SEARCH_DIALOG_H__
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include "config.h"

class SearchDialog: public QDialog
{
  Q_OBJECT
 private:
  QLineEdit *search, *replace;
  QCheckBox *searchRegExp, *caseSensitiveCB, *wholeWordsCB,
    *fromCursorCB;
 public:
  // Constructor
  SearchDialog(QWidget *parent=0);
  ~SearchDialog();

  // Strings
  inline QString searchString()  { return search->text(); }
  inline QString replaceString() { return replace->text(); }

  // Options
  inline bool searchStringIsRegExp()  { return searchRegExp->checkState();    }
  inline bool caseSensitive()         { return caseSensitiveCB->checkState(); }
  inline bool wholeWords()            { return wholeWordsCB->checkState();    }
  inline bool fromCursor()            { return fromCursorCB->checkState();    }

  // Signals
  signals:
  void search_signal();
  void replace_signal();
};

#endif
