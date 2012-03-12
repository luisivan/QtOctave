FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")

SET(QT_USE_QTSVG 1)
SET(QT_USE_QTXML 1)

include(UseQt4)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
include_directories(${QT_QTXML_INCLUDE_DIR})
include_directories(${QT_QTSVG_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(ui_design_main_window.h design_main_window.ui)
QT4_WRAP_UI(ui_export_to_dialog.h export_to_dialog.ui)
QT4_WRAP_UI(ui_view_angle.h view_angle.ui)

#QT4_GENERATE_MOC(gnuplot_connection.h gnuplot_connection.moc)
#QT4_GENERATE_MOC(input.h input.moc)
#QT4_GENERATE_MOC(main_window.h main_window.moc)

QT4_WRAP_CPP(moc_gnuplot_connection gnuplot_connection.h)
QT4_WRAP_CPP(moc_input input.h)
QT4_WRAP_CPP(moc_main_window main_window.h)

#QT4_AUTOMOC(gnuplot_connection.cpp gnuplot_connection.h input.cpp input.h main.cpp main_window.cpp main_window.h)

#ADD_DEFINITIONS(-D_REENTRANT)

add_executable(easy_plot 
	gnuplot_connection.cpp gnuplot_connection.h
	input.cpp input.h
	main.cpp
	main_window.cpp main_window.h
	ui_view_angle.h ui_export_to_dialog.h ui_design_main_window.h
	moc_gnuplot_connection.cxx moc_input.cxx moc_main_window.cxx
)

TARGET_LINK_LIBRARIES(easy_plot ${Qt4_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSQL_LIBRARY} ${QT_QTSVG_LIBRARY})

SET_TARGET_PROPERTIES(easy_plot
        PROPERTIES INSTALL_RPATH ":usr/lib"
        BUILD_WITH_INSTALL_RPATH true
        INSTALL_RPATH_USE_LINK_PATH true
)

install	(
		TARGETS easy_plot
		RUNTIME DESTINATION bin
	)

