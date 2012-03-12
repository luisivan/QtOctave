
#cmake "-DCMAKE_INSTALL_PREFIX:PATH=/usr/local" .

cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -march=pentium4 -mtune=pentium4 -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/home/lucas/usr" "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

#cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O0 -s -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/home/lucas/usr" .

#make install
