FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")

SET(QT_USE_QTXML 1)

include(UseQt4)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
#include_directories(${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${QT_QTSVG_INCLUDE_DIR})

FILE(GLOB_RECURSE src_moc_headers *.h  )
FILE(GLOB_RECURSE src_sources *.cpp )
FILE(GLOB_RECURSE src_ui *.ui )

# run uic on .ui files:
QT4_WRAP_UI( src_ui_headers ${src_ui} )

# run moc on these files:
QT4_WRAP_CPP( src_moc_sources ${src_moc_headers} )
 
add_executable(simplercs 
	${src_sources} ${src_moc_sources} ${src_ui_headers} ${src_moc_headers}
)

SET_TARGET_PROPERTIES(simplercs
	PROPERTIES INSTALL_RPATH ":usr/lib"
	BUILD_WITH_INSTALL_RPATH true
	INSTALL_RPATH_USE_LINK_PATH true
)

TARGET_LINK_LIBRARIES(simplercs ${Qt4_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )


install	(
	TARGETS simplercs
	RUNTIME DESTINATION bin
	)

