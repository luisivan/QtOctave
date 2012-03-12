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

 /*! \mainpage QtOctave Index Page
 *
 * \section intro_sec Introduction
 *
 * QtOctave is a simple front-end for Octave. Octave is a Matlab like calculus application. <br>
 * QtOctave main page: <a href="qtoctave.wordpress.com">qtoctave.wordpress.com</a>
 *
 * \section modules_sec QtOctave modules.
 * QtOctave uses this module structure:<br>
 * Main module point of <a href="../images/modules_main.png">view</a><br>
 * Visual tools point of view <a href="../images/modules_basewidget.png">view</a><br>
 *
 * Main module controls all modules and interconnect them.<br>
  * BaseWidget module is a base class for windows shown in QtOctave.
  * 
 * \subsection operations_sec Operations module.
  * 
  * Operations module connects menus signals with callbacks. Contains some classes as Plot class and General_Menu class.
 */

/** @file config.h
 * This file contains application's config parameters. Configuration is automaticly load and save from CONFIG_PATH/config.rc and from user home/.qtoctave.rc
 */

#ifdef USER_CONFIG
#include "configure.h"
#endif

#ifndef HELP_PATH
#define HELP_PATH "./octave_doc/index.html"
#endif

#ifndef ICON_PATH
#define ICON_PATH "./images"
#endif

#ifndef FONT_PATH
#define FONT_PATH "./fonts"
#endif

#ifndef QTOCTAVE_HELP_PATH
#define QTOCTAVE_HELP_PATH "./qtoctave_doc/index.html"
#endif

#ifndef CONFIG_PATH
#define CONFIG_PATH "./"
#endif

#ifndef LANG_PATH
#define LANG_PATH "./lang"
#endif

#ifndef PKG_CMD_PATH
#define PKG_CMD_PATH "./config_files/pkg-commands.list"
#endif

#define PKG_ENABLED

#ifndef CONFIG_H
#define CONFIG_H

//#undef QT_NO_DEBUG

#include <QString>
#include <QMap>
#include "session.h"
#include <stdio.h>

static QMap<QString, QString> config;

/**Gets config of parameter.
 * @param parameter Parameter name.
 * @return A QString with parameter value.
 */
const QString get_config(QString parameter);

/**Sets config of parameter.
 * @param configuration Add a parameter with value. QMap key is parameter name. QMap value is parameter value.
 */
void set_config(QMap<QString,QString> configuration);



#endif
