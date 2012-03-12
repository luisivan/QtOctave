function start()
{
	print("sombrero("+widget.comboBox.currentText+")");
	widget.close()
}

function change_value(x)
{
	widget.label.text=widget.comboBox.currentText;
}

widget.pushButton.clicked.connect(start);
widget.comboBox.activated.connect(change_value);
widget.comboBox.currentIndex=2;
