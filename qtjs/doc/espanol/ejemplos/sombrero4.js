function terminar()
{
	print("sombrero("+widget.comboBox.currentText+")");
	widget.close()
}

function cambiar_valor(x)
{
	widget.label.text=widget.comboBox.currentText;
}

widget.pushButton.clicked.connect(terminar);
widget.comboBox.activated.connect(cambiar_valor);
widget.comboBox.currentIndex=2;
