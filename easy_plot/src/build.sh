qmake -project -o easy_plot.pro
echo "CONFIG += qt thread" >> easy_plot.pro
echo "QT += svg xml" >> easy_plot.pro
qmake
#make
