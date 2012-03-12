#!/bin/bash

echo '<!DOCTYPE RCC><RCC version="1.0"><qresource>' > images.qrc
for a in images/*; do
printf '<file>%s</file>\n' $a >> images.qrc
done
echo '</qresource></RCC>' >> images.qrc

qmake -project

