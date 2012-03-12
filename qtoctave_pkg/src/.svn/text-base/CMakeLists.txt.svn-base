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
include_directories(${QT_QTNETWORK_INCLUDE_DIR})

FILE(GLOB_RECURSE qtoctave_pkg_moc_headers *.h  )
FILE(GLOB_RECURSE qtoctave_pkg_sources *.cpp )
FILE(GLOB_RECURSE qtoctave_pkg_ui *.ui )
FILE(GLOB_RECURSE qtoctave_pkg_qrc *.qrc )


# run uic on .ui files:
QT4_WRAP_UI( qtoctave_pkg_ui_headers ${qtoctave_pkg_ui} )

# run moc on these files:
QT4_WRAP_CPP( qtoctave_pkg_moc_sources ${qtoctave_pkg_moc_headers} )

# run rcc on these files:
QT4_ADD_RESOURCES ( qtoctave_pkg_qrc_sources ${qtoctave_pkg_qrc} ) 
 
add_executable(qtoctave_pkg 
	${qtoctave_pkg_sources} ${qtoctave_pkg_moc_sources} ${qtoctave_pkg_ui_headers} ${qtoctave_pkg_moc_headers} ${qtoctave_pkg_qrc_sources} 
)

SET_TARGET_PROPERTIES(qtoctave_pkg
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(qtoctave_pkg 
	${Qt4_LIBRARIES} 
	${QT_QTCORE_LIBRARY} 
	${QT_QTGUI_LIBRARY} 
	${QT_QTXML_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
)


install	(
	TARGETS qtoctave_pkg
	RUNTIME DESTINATION bin
	)


