
#cmake "-DCMAKE_INSTALL_PREFIX:PATH=/usr/local" .

#cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -march=native -mtune=native -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/opt/qtoctave-0.9.1" "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

#cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O0 -s -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/home/lucas/usr" .

cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/tmp/qtoctave" "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

#make install
