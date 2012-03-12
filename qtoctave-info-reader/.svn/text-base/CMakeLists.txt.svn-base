FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")

include(UseQt4)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB_RECURSE qtoctave_info_reader_moc_headers *.h  )
FILE(GLOB_RECURSE qtoctave_info_reader_sources *.cpp )
FILE(GLOB_RECURSE qtoctave_info_reader_ui *.ui )
FILE(GLOB_RECURSE qtoctave_info_reader_qrc *.qrc )


# run uic on .ui files:
QT4_WRAP_UI( qtoctave_info_reader_ui_headers ${qtoctave_info_reader_ui} )

# run moc on these files:
QT4_WRAP_CPP( qtoctave_info_reader_moc_sources ${qtoctave_info_reader_moc_headers} )

# run rcc on these files:
QT4_ADD_RESOURCES ( qtoctave_info_reader_qrc_sources ${qtoctave_info_reader_qrc} ) 
 
add_executable(qtoctave-info-reader 
	${qtoctave_info_reader_sources} ${qtoctave_info_reader_moc_sources} ${qtoctave_info_reader_ui_headers} ${qtoctave_info_reader_moc_headers} ${qtoctave_info_reader_qrc_sources} 
)

SET_TARGET_PROPERTIES(qtoctave-info-reader
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(qtoctave-info-reader 
	${Qt4_LIBRARIES} 
	${QT_QTCORE_LIBRARY} 
	${QT_QTGUI_LIBRARY} 
)


install	(
	TARGETS qtoctave-info-reader
	RUNTIME DESTINATION bin
	)


