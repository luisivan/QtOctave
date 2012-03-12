function start()
{
	print("sombrero("+widget.lineEdit.text+")");
	widget.close()
}

widget.pushButton.clicked.connect(start);