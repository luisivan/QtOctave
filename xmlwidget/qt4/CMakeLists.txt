CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# The name of our project is "XMLWIDGET".  CMakeLists files in this project can
# refer to the root source directory of the project as ${XMLWIDGET_SOURCE_DIR} and
# to the root binary directory of the project as ${XMLWIDGET_BINARY_DIR}.

project (xmlwidget)

#include ("options.txt")

set(CMAKE_VERBOSE_MAKEFILE TRUE)

#configure_file ( configure.h src/configure.h )

#ADD_DEFINITIONS(-DUSER_CONFIG)

add_subdirectory (src)
