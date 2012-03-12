#!/usr/bin/bash

if [[ -e $1.ui ]]; then

octave  --persist  --eval `qtjs $1.ui $1.js`

else

cat<<EOF
Debe escribir el nombre del ejemplo que desea ejecutar. Por ejemplo, para ejecutar los ejemplos sombrero2.ui y sombrero2.js, debe teclear en su terminal:

$0 sombrero2

EOF
fi