FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")

SET(QT_USE_QTXML 1)

include(UseQt4)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
include_directories(${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${QT_QTSVG_INCLUDE_DIR})

FILE(GLOB_RECURSE xmlwidget_moc_headers *.h  )
FILE(GLOB_RECURSE xmlwidget_sources *.cpp )
FILE(GLOB_RECURSE xmlwidget_ui *.ui )

# run uic on .ui files:
QT4_WRAP_UI( xmlwidget_ui_headers ${xmlwidget_ui} )

# run moc on these files:
QT4_WRAP_CPP( xmlwidget_moc_sources ${xmlwidget_moc_headers} )
 
add_executable(xmlwidget 
	${xmlwidget_sources} ${xmlwidget_moc_sources} ${xmlwidget_ui_headers} ${xmlwidget_moc_headers}
)

SET_TARGET_PROPERTIES(xmlwidget
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(xmlwidget 
	${Qt4_LIBRARIES} 
	${QT_QTCORE_LIBRARY} 
	${QT_QTGUI_LIBRARY} 
	${QT_QTXML_LIBRARY} 
)


install	(
	TARGETS xmlwidget
	RUNTIME DESTINATION bin
	)


