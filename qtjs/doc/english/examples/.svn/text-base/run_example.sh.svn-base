#!/usr/bin/bash

if [[ -e $1.ui ]]; then

octave  --persist  --eval `qtjs $1.ui $1.js`

else

cat<<EOF
You must write name of example. If you want to run sombrero2.ui and sombrero2.js, you must write:

$0 sombrero2

EOF
fi