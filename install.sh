
cmake "-DCMAKE_INSTALL_PREFIX:PATH=/usr/local" .

#cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -march=pentium4 -mtune=pentium4 -Wall -pipe" 	"-DCMAKE_INSTALL_PREFIX:PATH=/usr/local/opt/binaries/qtoctave-0.8.2" "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

#cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -Wall -pipe" 	'-DCMAKE_INSTALL_PREFIX:PATH=./qtoctave-0.9.1' "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON" .

make

make install
