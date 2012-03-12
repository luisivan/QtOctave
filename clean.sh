#!/bin/bash

function clean_files()
{
	for p in "$@" ; do
		echo "Cleaning " "$p"
		for a in `find . -name "$p"`; do
			echo $a
			rm -Rf $a
		done
	done
}

echo "Cleaning files"

make clean

clean_files CMakeFiles _CPack_Packages CMakeCache.txt cmake_install.cmake install_manifest.txt CPackConfig.cmake CPackSourceConfig.cmake '*~' clean Makefile

rm -Rf src

