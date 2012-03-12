/* Copyright (C) 2008 Alejandro Álvarez
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
#ifndef PKG_BIND_H
#define PKG_BIND_H

#include <QString>
#include <QSet>

class PkgBind: private QObject
{
  Q_OBJECT
 private:
  QString invokeCmd;
  QSet<QString> commands;
  static PkgBind *instance;

  /* Constructor
   * Singleton
   */
  PkgBind();
 public:
  /* Get the unique instance
   * or create it if there isn't any
   */
  static PkgBind *getInstance();

  /* Load the command list from a file
   */
  void loadCommandList();

  /* Check if a symbol is defined
   * as a funciont included in some package
   */
  bool checkSymbol(const QString &s);

  /* Invoke the package manager
   * for install the package with the command 
   * "cmd"
   */
  void invokePackageManager(const QString &s);

 public slots:
  void invokeResponse(int result);
};

#endif
