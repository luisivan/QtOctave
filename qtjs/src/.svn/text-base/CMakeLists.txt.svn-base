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
include_directories(${QT_QTSCRIPT_INCLUDE_DIR})
include_directories(${QT_QTUITOOLS_INCLUDE_DIR})

FILE(GLOB_RECURSE qtjs_moc_headers *.h  )
FILE(GLOB_RECURSE qtjs_sources *.cpp )
FILE(GLOB_RECURSE qtjs_ui *.ui )

# run uic on .ui files:
QT4_WRAP_UI( qtjs_ui_headers ${qtjs_ui} )

# run moc on these files:
QT4_WRAP_CPP( qtjs_moc_sources ${qtjs_moc_headers} )
 
add_executable(qtjs 
	${qtjs_sources} ${qtjs_moc_sources} ${qtjs_ui_headers} ${qtjs_moc_headers}
)

SET_TARGET_PROPERTIES(qtjs
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(qtjs 
	${Qt4_LIBRARIES} 
	${QT_QTCORE_LIBRARY} 
	${QT_QTGUI_LIBRARY} 
	${QT_QTXML_LIBRARY} 
	#${QT_QTSVG_LIBRARY} 
	${QT_QTSCRIPT_LIBRARY}
	${QT_QTUITOOLS_LIBRARY}
)


install	(
	TARGETS qtjs
	RUNTIME DESTINATION bin
	)


