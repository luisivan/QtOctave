######################################################################
# Automatically generated by qmake (2.01a) s�b ene 19 01:05:46 2008
######################################################################

TEMPLATE = app
TARGET = easy_plot
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += gnuplot_connection.h input.h main_window.h
FORMS += design_main_window.ui export_to_dialog.ui view_angle.ui
SOURCES += gnuplot_connection.cpp input.cpp main.cpp main_window.cpp
CONFIG += qt thread
QT += svg xml
