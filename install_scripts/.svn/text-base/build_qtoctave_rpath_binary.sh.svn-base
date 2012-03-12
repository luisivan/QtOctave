#!/bin/bash

if [[ -z $1 ]]; then
cat<<EOF
$0 version

Compila el código de qtoctave para que sea un binario transportable.

version número de la versión
EOF
exit 0
fi

version=$1

echo
echo "Creando la versión" $version
echo

mkdir binaries

#install_path=`pwd`/binaries

install_path=/usr/local/opt/binaries/$version

echo $install_path

echo
echo "Compilando qtoctave"
echo

cd qtoctave

sh clean.sh

cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -pipe"  "-DCMAKE_INSTALL_PREFIX:PATH=$install_path" .

make install


#
# echo
# echo "Compilando qtoctave independiente de instalación"
# echo
#
# sh clean.sh
# cmake "-DCMAKE_CXX_FLAGS_RELEASE:STRING=-O2 -s -pipe"  "-DCMAKE_INSTALL_PREFIX:PATH=../" .
#make

cd ..

echo
echo "Copiando el fichero qtoctave generado a su destino"
echo

cp -R $install_path/* binaries/



echo
echo "Copiando ficheros de instalacion"
echo

####################################################################
#Herramienta de instalación
####################################################################

cat<<EOF > binaries/install.sh
#!/bin/bash

echo Installing...

mkdir -p $install_path
cp -R * $install_path
EOF

echo 'if [[ $? -ne 0 ]]; then' >> binaries/install.sh
cat<<EOF >> binaries/install.sh
	echo "Error: Files can not be copied to $install_path"
	echo "Maybe you will need be a root user"
	exit 1
fi

cp --parents share/applications/qtoctave.desktop /usr/local/
cp --parents share/icons/hicolor/64x64/apps/qtoctave.png /usr/local/
EOF

for file in `ls binaries/bin`; do

cat<<EOF >> binaries/install.sh

cat<<EOL > /usr/local/bin/$file
#!/bin/bash

exec $install_path/bin/ld-linux.so.2 --library-path $install_path/bin/  $install_path/bin/$file

EOL

chmod +x /usr/local/bin/$file

EOF
done

cat<<EOF >> binaries/install.sh

echo
echo Execute qtoctave to start QtOctave
echo

EOF

chmod +x binaries/install.sh

####################################################################
#Herramienta de desinstalación
####################################################################

echo '#!/bin/bash' >  binaries/uninstall-$version.sh
echo 'echo Uninstallig' $version >> binaries/uninstall-$version.sh

for file in `ls binaries/bin/`; do
cat<<EOL >> binaries/uninstall-$version.sh
rm -f /usr/local/bin/$file

rm -f /usr/local/share/applications/qtoctave.desktop
rm -f /usr/local/share/icons/hicolor/64x64/apps/qtoctave.png

EOL
echo 'if [[ $? -ne 0 ]]; then' >> binaries/uninstall-$version.sh
cat<<EOL >> binaries/uninstall-$version.sh
	echo "Error: File /usr/local/bin/$file can not be removed"
	echo "Maybe you will need be a root user"
fi
EOL
done

echo "exec rm -Rf $install_path" >> binaries/uninstall-$version.sh

chmod +x binaries/uninstall-$version.sh


echo
echo "Copiando bibliotecas"
echo

make
./libs_copy binaries/bin/qtoctave binaries/bin

echo
echo "Copiando licencias"
echo

cp qtoctave/LI* binaries/

echo
echo "Creando ficheros readme.txt y leeme.txt"
echo

cat<<EOL >> binaries/leeme.txt
QtOctave es un front-end para Octave.

Para realizar la instalación ejecutar el archivo "install.sh" como usuario root.

En Ubuntu en un terminal habría que ejecutar:

sudo install.sh

En otras distribuciones:

su
install.sh

Para realizar la desinstalación ejecutar el archivo "$install_path/uninstall-$version.sh" como usuario root.
EOL


cat<<EOL >> binaries/readme.txt
QtOctave is a simple front-end for Octave.

To install execute "install.sh" as root user.

In Ubuntu, write in terminal:

sudo install.sh

In others:

su
install.sh

To uninstall execute as root user "$install_path/uninstall-$version.sh"

EOL


echo
echo "Comprimiendo $version.tar.bz2"
echo

mv binaries $version

rm -f $version.binaries.tar.bz2
tar -cf $version.binaries.tar $version
bzip2 -9 $version.binaries.tar
#gzip -9 $version.binaries.tar

echo
echo Finalizado
echo
