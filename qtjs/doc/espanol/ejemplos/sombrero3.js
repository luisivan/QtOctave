function terminar()
{
	print("sombrero("+widget.lineEdit.text+")");
	widget.close()
}

function cambiar_valor(x)
{
	widget.lineEdit.text=widget.horizontalSlider.value;
}

widget.pushButton.clicked.connect(terminar);
widget.lineEdit.returnPressed.connect(terminar);
widget.horizontalSlider.valueChanged.connect(cambiar_valor);