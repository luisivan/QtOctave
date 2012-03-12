CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# The name of our project is "EASY_PLOT".  CMakeLists files in this project can
# refer to the root source directory of the project as ${EASY_PLOT_SOURCE_DIR} and
# to the root binary directory of the project as ${EASY_PLOT_BINARY_DIR}.

project (easy_plot)

#include ("options.txt")

set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_subdirectory (src)
