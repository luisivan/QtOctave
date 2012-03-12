
#cmake "-DCMAKE_INSTALL_PREFIX:PATH=/tmp/qtoctave-0.9.1" .

cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -march=i686 -mtune=i686 -Wall -pipe" "-DCMAKE_INSTALL_PREFIX:PATH=/opt/home/Applications/qtoctave" "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

#make install
