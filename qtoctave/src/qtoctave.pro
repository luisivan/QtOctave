######################################################################
# IMPORTANT:
# ----------
#
# This is used for translations
# If you want to build the source, please, use CMAKE instead
#
######################################################################

TRANSLATIONS = lang/qtoctave_es.ts
CODECFORTR   = UTF-8
CODECFORSRC  = UTF-8

# Input
HEADERS += autocomplete.h \
           basewidget.h \
           codeedit.h \
           command_list.h \
           config.h \
           editor.h \
           generate_menu.h \
           help.h \
           main.h \
           mainwindow.h \
           navigator.h \
           numberedtextview.h \
           octave_connection.h \
           operations.h \
           search_dialog.h \
           session.h \
           syntax.h \
           table.h \
           terminal.h \
           variables_list.h \
           window_list.h \
           dynamic_help/dynamic_help.h \
           operations/general_menu.h \
           operations/table_plot.h \
           projects/projects.h \
           svgcanvas/svgcanvas.h
FORMS += table_ui.ui \
         operations/general_menu.ui \
         projects/newprojectdialog.ui \
         projects/projects.ui \
         svgcanvas/export_to_dialog.ui
SOURCES += autocomplete.cpp \
           basewidget.cpp \
           codeedit.cpp \
           command_list.cpp \
           config.cpp \
           editor.cpp \
           generate_menu.cpp \
           help.cpp \
           main.cpp \
           mainwindow.cpp \
           navigator.cpp \
           numberedtextview.cpp \
           octave_connection.cpp \
           search_dialog.cpp \
           session.cpp \
           syntax.cpp \
           table.cpp \
           terminal.cpp \
           variables_list.cpp \
           window_list.cpp \
           dynamic_help/dynamic_help.cpp \
           operations/general_menu.cpp \
           operations/operations.cpp \
           operations/plot.cpp \
           operations/table_plot.cpp \
           projects/projects.cpp \
           svgcanvas/svgcanvas.cpp
