CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# The name of our project is "SIMPLERCS".  CMakeLists files in this project can
# refer to the root source directory of the project as ${SIMPLERCS_SOURCE_DIR} and
# to the root binary directory of the project as ${SIMPLERCS_BINARY_DIR}.

project (simplercs)

#include ("options.txt")

#Uncoment to Verbose mode
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

#ADD_DEFINITIONS(-DUSER_CONFIG)

add_subdirectory (src)

install(
	DIRECTORY design
	DESTINATION share/doc/qtoctave-utils/simple_rcs
	PATTERN ".svn" EXCLUDE
)