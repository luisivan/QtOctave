function start()
{
	print("sombrero("+widget.lineEdit.text+")");
	widget.close()
}

function change_value(x)
{
	widget.lineEdit.text=widget.horizontalSlider.value;
}

widget.pushButton.clicked.connect(start);
widget.lineEdit.returnPressed.connect(start);
widget.horizontalSlider.valueChanged.connect(change_value);