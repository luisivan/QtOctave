CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# The name of our project is "QTOCTAVE".  CMakeLists files in this project can
# refer to the root source directory of the project as ${QTOCTAVE_SOURCE_DIR} and
# to the root binary directory of the project as ${QTOCTAVE_BINARY_DIR}.

project (qtoctave)

#include ("options.txt")

#SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/qtoctave/cmake-modules/)

IF (NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release)
ENDIF (NOT CMAKE_BUILD_TYPE)

#Uncoment to Verbose mode
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

ADD_DEFINITIONS(-DUSER_CONFIG)

FIND_PACKAGE(Qt4 REQUIRED)

MESSAGE (STATUS "Use file: ${QT_USE_FILE}")
MESSAGE (STATUS "Qt version : ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

SET(QT_USE_QTXML 1)

include(UseQt4)

if( NOT ${QT_VERSION_MAJOR} EQUAL 4 OR NOT ${QT_VERSION_MINOR} GREATER 5   )
	MESSAGE (FATAL_ERROR 
			"Qt version 4.6 or 4.7 required.\n"
			"Qt version installed is: ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}\n"
			"Please install required version.\n"
		)
endif( NOT ${QT_VERSION_MAJOR} EQUAL 4 OR NOT ${QT_VERSION_MINOR} GREATER 5   )

add_subdirectory (qtoctave_pkg/src)
add_subdirectory (xmlwidget/qt4)
add_subdirectory (easy_plot)
add_subdirectory (simple_rcs)
add_subdirectory (qtjs)
add_subdirectory (qtoctave-info-reader)
add_subdirectory (qtoctave)


# install	(
# 	TARGETS src/config_files/qtoctave
# 	RUNTIME DESTINATION bin
# 	)



