INSTALL.

QtOctave requires:
* Qt4 library and Qt4 devel packages. Version 4.6.0 or better.
* Octave program. Version 3.2.0 or better. (http://www.gnu.org/software/octave/)
* cmake program. (http://www.cmake.org/)
* a C++ - compiler, eg. the GNU compiler (http://gcc.gnu.org/)

For building QtOctave from source execute:

tar xvzf qtoctave-xxx.tar.gz
cd qtoctave-xxx
mkdir build
cd build
cmake ..
# if you want a specific install-path (for example your $HOME directory, if you are not root), 
# you can do "cmake ..  -DCMAKE_INSTALL_PREFIX:PATH=/some/path"
make
make install

Maybe you should be root user when installing (it depends on install path).


